#pragma once

#include "logs/interfaces/logs.hpp"
#include "sysfs/factory.hpp"

#include <tuple>
#include <variant>

namespace sysfs::lnx
{

enum class mounttype
{
    normal,
    inverted
};

using configrw_t = std::tuple<std::string, std::shared_ptr<logs::LogIf>>;
using configexportrw_t = std::tuple<std::string, std::string, uint32_t,
                                    std::shared_ptr<logs::LogIf>>;
using config_t = std::variant<std::monostate, configrw_t, configexportrw_t>;

class Sysfs : public SysfsIf
{
  public:
    ~Sysfs();
    bool read(const std::string&, std::string&) override;
    bool write(const std::string&, const std::string&) override;
    bool writetest(const std::string&, const std::string&) override;

  private:
    friend class sysfs::Factory;
    explicit Sysfs(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace sysfs::lnx
