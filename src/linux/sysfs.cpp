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
        path{std::get<0>(config)}, num{std::get<1>(config)},
        target{std::get<2>(config)}, directory{path}
    {
        create(path / (target + num));
    }

    explicit Handler(const configrw_t& config) :
        path{config}, num{}, directory{path}
    {}

    ~Handler()
    {
        if (!target.empty())
            remove(directory);
    }

    bool write(const std::string& name, const std::string& val) const
    {
        const auto file = directory / name;
        auto retries{accessattemptsmax};
        while (retries--)
        {
            if (std::ofstream ofs{file}; ofs.is_open())
            {
                ofs << val << std::flush;
                return true;
            }
            setdelay(1ms);
        }
        throw std::runtime_error("Cannot open sysfs output file " +
                                 std::string(file));
    }

    bool read(const std::string& name, std::string& val) const
    {
        const auto file = directory / name;
        auto retries{accessattemptsmax};
        while (retries--)
        {
            if (std::ifstream ifs{file}; ifs.is_open())
            {
                ifs >> val;
                return true;
            }
            setdelay(1ms);
        }
        throw std::runtime_error("Cannot open sysfs input file " +
                                 std::string(file));
    }

    bool writetest(const std::string& name, const std::string& val) const
    {
        const auto file = directory / name;
        if (std::string ret; write(name, val) && read(name, ret) && ret == val)
            return true;
        throw std::runtime_error("Cannot set value for sysfs file " +
                                 std::string(file));
    }

  private:
    const std::filesystem::path path;
    const std::string num;
    const std::string target;
    std::filesystem::path directory;
    const uint32_t accessattemptsmax{100};

    bool create(std::filesystem::path node)
    {
        if (write("export", num))
        {
            auto retries{accessattemptsmax};
            while (retries--)
            {
                if (std::filesystem::exists(node) &&
                    !std::filesystem::is_empty(node))
                {
                    directory = node;
                    return true;
                }
                setdelay(1ms);
            }
        }
        throw std::runtime_error("Cannot create sysfs node " +
                                 std::string(node));
    }

    bool remove(std::filesystem::path node)
    {
        directory = path;
        if (write("unexport", num))
        {
            auto retries{accessattemptsmax};
            while (retries--)
            {
                if (!std::filesystem::exists(node))
                    return true;
                setdelay(1ms);
            }
        }
        throw std::runtime_error("Cannot remove sysfs node " +
                                 std::string(node));
    }

    void setdelay(std::chrono::milliseconds time) const
    {
        usleep((uint32_t)time.count() * 1000);
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
