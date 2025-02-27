#include "sysfs/interfaces/linux/sysfs.hpp"

#include <chrono>
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

    bool write(const std::string& name, const std::string& val,
               bool parent = false) const
    {
        const auto file = getfilename(name, parent);
        auto retries{accessattemptsmax};
        while (retries--)
        {
            if (std::ofstream ofs{file}; ofs.is_open())
            {
                ofs << val << std::flush;
                log(logging::type::debug,
                    "Written: '" + val + "' to '" + name + "'");
                return true;
            }
            setdelay(1ms);
        }
        throw std::runtime_error("Cannot open sysfs output file " +
                                 std::string(file));
    }

    bool read(const std::string& name, std::string& val,
              bool parent = false) const
    {
        const auto file = getfilename(name, parent);
        auto retries{accessattemptsmax};
        while (retries--)
        {
            if (std::ifstream ifs{file}; ifs.is_open())
            {
                ifs >> val;
                log(logging::type::debug,
                    "Read: '" + val + "' from '" + name + "'");
                return true;
            }
            setdelay(1ms);
        }
        throw std::runtime_error("Cannot open sysfs input file " +
                                 std::string(file));
    }

    bool writetest(const std::string& name, const std::string& val,
                   bool parent = false) const
    {
        const auto file = getfilename(name, parent);
        if (std::string ret; write(name, val) && read(name, ret) && ret == val)
            return true;
        throw std::runtime_error("Cannot set value for sysfs file " +
                                 std::string(file));
    }

  private:
    const std::filesystem::path path;
    const std::string type;
    const std::string num;
    const uint32_t accessattemptsmax{100};
    const std::shared_ptr<logging::LogIf> logif;

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
                        log(logging::type::debug, "Created node: " + nodename);
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
                        log(logging::type::debug, "Removed node: " + nodename);
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
        logging::type type, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(type, std::string{loc.function_name()}, msg);
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

bool Sysfs::read(const std::string& name, std::string& val)
{
    return handler->read(name, val);
}

bool Sysfs::write(const std::string& name, const std::string& val)
{
    return handler->write(name, val);
}

bool Sysfs::writetest(const std::string& name, const std::string& val)
{
    return handler->writetest(name, val);
}

} // namespace sysfs::lnx
