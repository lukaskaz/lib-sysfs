#pragma once

#include "sysfs/interfaces/sysfs.hpp"

#include <memory>

namespace sysfs
{

class Factory
{
  public:
    template <typename T, typename C>
    static std::shared_ptr<SysfsIf> create(const C& config)
    {
        return std::shared_ptr<T>(new T(config));
    }
};

} // namespace sysfs
