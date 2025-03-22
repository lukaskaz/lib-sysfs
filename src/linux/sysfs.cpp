#include "sysfs/interfaces/linux/sysfs.hpp"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <source_location>

namespace sysfs::lnx
{

using namespace std::chrono_literals;
using namespace std::string_literals;

struct Sysfs::Handler
{
  public:
    explicit Handler(const configexportrw_t& config) :
        path{std::get<0>(config)}, type{std::get<1>(config)},
        num{std::to_string(std::get<2>(config))}, logif{std::get<3>(config)}

    {
        create();
    }

    explicit Handler(const configrw_t& config) :
        path{std::get<0>(config)}, logif{std::get<1>(config)}
    {}

    ~Handler()
    {
        if (!type.empty())
            remove();
    }

    bool write(const std::filesystem::path& name, const std::string& val,
               bool parent = false) const
    {
        const auto file = getfilename(name, parent);
        auto retries{accessattemptsmax};
        while (retries--)
        {
            if (std::ofstream ofs{file}; ofs.is_open())
            {
                ofs << val << std::flush;
                log(logs::level::debug, "Written[" + num + "]: '" + val +
                                            "' to '" + name.native() + "'");
                return true;
            }
            setdelay(1ms);
        }
        throw std::runtime_error("Cannot open sysfs output file [" + num +
                                 "] " + std::string(file));
    }

    bool read(const std::filesystem::path& name, std::string& val,
              bool parent = false) const
    {
        const auto file = getfilename(name, parent);
        auto retries{accessattemptsmax};
        while (retries--)
        {
            if (std::ifstream ifs{file}; ifs.is_open())
            {
                ifs >> val;
                log(logs::level::debug, "Read[" + num + "]: '" + val +
                                            "' from '" + name.native() + "'");
                return true;
            }
            setdelay(1ms);
        }
        throw std::runtime_error("Cannot open sysfs input file [" + num + "] " +
                                 std::string(file));
    }

    bool writetest(const std::filesystem::path& name, const std::string& val,
                   bool parent = false) const
    {
        const auto file = getfilename(name, parent);
        if (std::string ret; write(name, val) && read(name, ret) && ret == val)
            return true;
        throw std::runtime_error("Cannot set value for sysfs file [" + num +
                                 "] " + std::string(file));
    }

    bool elevread(const std::filesystem::path& name, std::string& val,
                  bool parent = false) const
    {
        elevate(name, "o", "r");
        return read(name, val, parent);
    }

    bool elevwrite(const std::filesystem::path& name, const std::string& val,
                   bool parent = false) const
    {
        elevate(name, "o", "w");
        return write(name, val, parent);
    }

    bool elevate(const std::filesystem::path& name, const std::string& owner,
                 const std::string& perm) const
    {
        auto command = "sudo chmod " + owner + "+" + perm + " " + name.native();
        if (!std::system(command.c_str()))
        {
            log(logs::level::debug, "File " + name.native() +
                                        " elevated to: '" + owner + "+" + perm +
                                        "'");
            return true;
        }
        else
            log(logs::level::warning, "Cannot elevate file " + name.native() +
                                          " to: '" + owner + "+" + perm +
                                          "', due to: " + strerror(errno));
        return false;
    }

  private:
    const std::filesystem::path path;
    const std::string type;
    const std::string num;
    const uint32_t accessattemptsmax{100};
    const std::shared_ptr<logs::LogIf> logif;

    bool create()
    {
        const auto node = getnodename();
        const auto nodename{std::string(node)};
        if (!std::filesystem::exists(node))
        {
            if (write("export", num, true))
            {
                auto retries{accessattemptsmax};
                while (retries--)
                {
                    if (std::filesystem::exists(node) &&
                        !std::filesystem::is_empty(node))
                    {
                        log(logs::level::info, "Created node: " + nodename);
                        return true;
                    }
                    setdelay(1ms);
                }
            }
            throw std::runtime_error("Cannot create sysfs node " + nodename);
        }
        throw std::runtime_error("Sysfs node already exist " + nodename);
    }

    bool remove()
    {
        const auto node = getnodename();
        const auto nodename{std::string(node)};
        if (std::filesystem::exists(node))
        {
            if (write("unexport", num, true))
            {
                auto retries{accessattemptsmax};
                while (retries--)
                {
                    if (!std::filesystem::exists(node))
                    {
                        log(logs::level::info, "Removed node: " + nodename);
                        return true;
                    }
                    setdelay(1ms);
                }
            }
            throw std::runtime_error("Cannot remove sysfs node " + nodename);
        }
        throw std::runtime_error("Sysfs node not exist " + nodename);
    }

    std::filesystem::path getnodename() const
    {
        return path / (type + num);
    }

    std::filesystem::path getfilename(const std::string& name,
                                      bool parent) const
    {
        if (!parent && !type.empty())
            return getnodename() / name;
        return path / name;
    }

    void setdelay(std::chrono::milliseconds time) const
    {
        usleep((uint32_t)time.count() * 1000);
    }

    void log(
        logs::level level, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(level, std::string{loc.function_name()}, msg);
    }
};

Sysfs::Sysfs(const config_t& config)
{
    handler = std::visit(
        [](const auto& config) -> decltype(Sysfs::handler) {
            if constexpr (!std::is_same<const std::monostate&,
                                        decltype(config)>())
            {
                return std::make_unique<Sysfs::Handler>(config);
            }
            throw std::runtime_error(
                std::source_location::current().function_name() +
                "-> config not supported"s);
        },
        config);
}
Sysfs::~Sysfs() = default;

bool Sysfs::read(const std::filesystem::path& name, std::string& val)
{
    return handler->read(name, val);
}

bool Sysfs::write(const std::filesystem::path& name, const std::string& val)
{
    return handler->write(name, val);
}

bool Sysfs::writetest(const std::filesystem::path& name, const std::string& val)
{
    return handler->writetest(name, val);
}

bool Sysfs::elevread(const std::filesystem::path& name, std::string& val)
{
    return handler->elevread(name, val);
}

bool Sysfs::elevwrite(const std::filesystem::path& name, const std::string& val)
{
    return handler->elevwrite(name, val);
}

bool Sysfs::elevate(const std::filesystem::path& name, const std::string& owner,
                    const std::string& perm)
{
    return handler->elevate(name, owner, perm);
}

} // namespace sysfs::lnx
