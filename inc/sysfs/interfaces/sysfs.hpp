#pragma once

#include <cstdint>
#include <string>

namespace sysfs
{

class SysfsIf
{
  public:
    virtual ~SysfsIf() = default;
    virtual bool read(const std::string&, std::string&) = 0;
    virtual bool write(const std::string&, const std::string&) = 0;
    virtual bool writetest(const std::string&, const std::string&) = 0;
};

} // namespace sysfs
