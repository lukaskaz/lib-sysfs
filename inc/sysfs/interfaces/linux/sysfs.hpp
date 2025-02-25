#pragma once

#include "sysfs/factory.hpp"

#include <tuple>
#include <unordered_set>
#include <variant>

namespace sysfs::lnx
{

enum class mounttype
{
    normal,
    inverted
};

using configrw_t = std::string;
using configexportrw_t =
    std::tuple<std::string, std::string, std::unordered_set<uint32_t>>;
using config_t = std::variant<std::monostate, configrw_t, configexportrw_t>;

class Sysfs : public SysfsIf
{
  public:
    ~Sysfs();
    std::string read(const std::string&) override;
    bool read(const std::string&,
              std::vector<std::pair<uint32_t, std::string>>&) override;
    bool read(uint32_t, const std::string&, std::string&) override;
    bool write(const std::string&, const std::string&) override;
    bool write(const std::string&,
               const std::vector<std::pair<uint32_t, std::string>>&) override;
    bool write(uint32_t, const std::string&, const std::string&) override;
    bool writetest(
        const std::string&,
        const std::vector<std::pair<uint32_t, std::string>>&) override;
    bool writetest(uint32_t, const std::string&, const std::string&) override;

  private:
    friend class sysfs::Factory;
    explicit Sysfs(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace sysfs::lnx
