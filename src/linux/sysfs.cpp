#include "sysfs/interfaces/linux/sysfs.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <source_location>

namespace sysfs::lnx
{

using namespace std::chrono_literals;
using namespace std::string_literals;

struct Sysfs::Handler
{
  public:
    explicit Handler(const configexportrw_t& config) :
        path{std::get<0>(config)}, type{std::get<1>(config)}, nums{std::get<2>(
                                                                  config)}
    {
        std::ranges::for_each(nums, [this](auto num) { create(num); });
    }

    explicit Handler(const configrw_t& config) : path{config}
    {}

    ~Handler()
    {
        if (!nums.empty())
            std::ranges::for_each(nums, [this](auto num) { remove(num); });
    }

    std::string read(const std::string& name)
    {
        std::string ret;
        read(0, name, ret);
        return ret;
    }

    bool write(const std::string& name, const std::string& val)
    {
        return write(0, name, val);
    }

    bool read(const std::string& name,
              std::vector<std::pair<uint32_t, std::string>>& vals) const
    {
        std::ranges::for_each(nums, [this, &name, &vals](auto num) {
            if (std::string val; read(num, name, val))
                vals.emplace_back(num, val);
        });
        return true;
    }

    bool read(uint32_t num, const std::string& name, std::string& val,
              bool parent = false) const
    {
        auto file = getfilename(num, name, parent);
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

    bool write(const std::string& name,
               const std::vector<std::pair<uint32_t, std::string>>& vals) const
    {
        std::ranges::for_each(vals, [this, &name](const auto& val) {
            write(std::get<uint32_t>(val), name, std::get<std::string>(val));
        });
        return true;
    }

    bool write(uint32_t num, const std::string& name, const std::string& val,
               bool parent = false) const
    {
        auto file = getfilename(num, name, parent);
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

    bool writetest(
        const std::string& name,
        const std::vector<std::pair<uint32_t, std::string>>& vals) const
    {
        std::ranges::for_each(vals, [this, &name](const auto& val) {
            writetest(std::get<uint32_t>(val), name,
                      std::get<std::string>(val));
        });
        return true;
    }

    bool writetest(uint32_t num, const std::string& name,
                   const std::string& val, bool parent = false) const
    {
        auto file = getfilename(num, name, parent);
        if (std::string ret;
            write(num, name, val) && read(num, name, ret) && ret == val)
            return true;
        throw std::runtime_error("Cannot set value for sysfs file " +
                                 std::string(file));
    }

  private:
    const std::filesystem::path path;
    const std::string type;
    const std::unordered_set<uint32_t> nums;
    const uint32_t accessattemptsmax{100};

    bool create(uint32_t num)
    {
        auto node = getfilename(num, {}, false);
        if (write(num, "export", std::to_string(num), true))
        {
            auto retries{accessattemptsmax};
            while (retries--)
            {
                if (std::filesystem::exists(node) &&
                    !std::filesystem::is_empty(node))
                    return true;
                setdelay(1ms);
            }
        }
        throw std::runtime_error("Cannot create sysfs node " +
                                 std::string(node));
    }

    bool remove(uint32_t num)
    {
        auto node = getfilename(num, {}, false);
        if (write(num, "unexport", std::to_string(num), true))
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

    std::filesystem::path getfilename(uint32_t num, const std::string& name,
                                      bool parent) const
    {
        std::filesystem::path filepath{path};
        if (!parent && !type.empty())
        {
            if (nums.contains(num))
                filepath += type + std::to_string(num);
            else
                throw std::runtime_error(
                    std::source_location::current().function_name() +
                    "-> sysfs node number not supported"s +
                    std::to_string(num));
        }
        return filepath / name;
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

std::string Sysfs::read(const std::string& name)
{
    return handler->read(name);
}

bool Sysfs::read(const std::string& name,
                 std::vector<std::pair<uint32_t, std::string>>& vals)
{
    return handler->read(name, vals);
}

bool Sysfs::read(uint32_t num, const std::string& name, std::string& val)
{
    return handler->read(num, name, val);
}

bool Sysfs::write(const std::string& name, const std::string& val)
{
    return handler->write(name, val);
}

bool Sysfs::write(const std::string& name,
                  const std::vector<std::pair<uint32_t, std::string>>& vals)
{
    return handler->write(name, vals);
}

bool Sysfs::write(uint32_t num, const std::string& name, const std::string& val)
{
    return handler->write(num, name, val);
}

bool Sysfs::writetest(const std::string& name,
                      const std::vector<std::pair<uint32_t, std::string>>& vals)
{
    return handler->writetest(name, vals);
}

bool Sysfs::writetest(uint32_t num, const std::string& name,
                      const std::string& val)
{
    return handler->writetest(num, name, val);
}

} // namespace sysfs::lnx
