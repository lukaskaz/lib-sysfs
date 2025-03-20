#pragma once

#include <string>

namespace sysfs
{

std::string str(const auto& value)
{
    if constexpr (std::is_same<const std::string&, decltype(value)>())
        return value;
    else
        return std::to_string(value);
}

} // namespace sysfs
