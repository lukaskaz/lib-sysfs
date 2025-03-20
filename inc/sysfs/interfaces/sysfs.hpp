#pragma once

#include <filesystem>
#include <string>

namespace sysfs
{

class SysfsIf
{
  public:
    virtual ~SysfsIf() = default;
    virtual bool read(const std::filesystem::path&, std::string&) = 0;
    virtual bool write(const std::filesystem::path&, const std::string&) = 0;
    virtual bool writetest(const std::filesystem::path&,
                           const std::string&) = 0;
    virtual bool elevread(const std::filesystem::path&, std::string&) = 0;
    virtual bool elevwrite(const std::filesystem::path&,
                           const std::string&) = 0;
    virtual bool elevate(const std::filesystem::path&, const std::string&,
                         const std::string&) = 0;
};

} // namespace sysfs
