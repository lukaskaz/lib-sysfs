#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sysfs
{

class SysfsIf
{
  public:
    virtual ~SysfsIf() = default;
    virtual std::string read(const std::string&) = 0;
    virtual bool read(const std::string&,
                      std::vector<std::pair<uint32_t, std::string>>&) = 0;
    virtual bool read(uint32_t, const std::string&, std::string&) = 0;
    virtual bool write(const std::string&, const std::string&) = 0;
    virtual bool
        write(const std::string&,
              const std::vector<std::pair<uint32_t, std::string>>&) = 0;
    virtual bool write(uint32_t, const std::string&, const std::string&) = 0;
    virtual bool
        writetest(const std::string&,
                  const std::vector<std::pair<uint32_t, std::string>>&) = 0;
    virtual bool writetest(uint32_t, const std::string&,
                           const std::string&) = 0;
};

} // namespace sysfs
