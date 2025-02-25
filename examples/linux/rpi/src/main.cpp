#include "sysfs/interfaces/linux/sysfs.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, [[maybe_unused]] char** argv)
{
    try
    {
        if (argc == 2)
        {
            std::cout << "First scenario -> reading file\n";
            using namespace sysfs::lnx;
            std::string path{"/sys/class/thermal"};
            auto iface = sysfs::Factory::create<Sysfs, configrw_t>(path);

            auto val = iface->read("thermal_zone0/temp");
            std::cout << "Read temperature value: " << val << "\n";
            std::cout << "First scenario DONE -> destroying iface\n";
        }
        if (argc == 3)
        {
            std::cout << "Second scenario -> export/write/read\n";
            using namespace sysfs::lnx;
            std::string path{"/sys/class/gpio/"};
            auto iface = sysfs::Factory::create<Sysfs, configexportrw_t>(
                {path, "gpio", {415, 419, 420}});

            std::vector<std::pair<uint32_t, std::string>> val;
            iface->read("direction", val);
            std::ranges::for_each(val, [](const auto& v) {
                std::cout << "Gpio direction: " << v.first << "->" << v.second
                          << "\n";
            });
            iface->write("direction", {{415, "out"}, {420, "out"}});
            iface->write(419, "direction", "out");
            val.clear();
            iface->read("direction", val);
            std::ranges::for_each(val, [](const auto& v) {
                std::cout << "Gpio direction: " << v.first << "->" << v.second
                          << "\n";
            });

            val.clear();
            iface->read("value", val);
            std::ranges::for_each(val, [](const auto& v) {
                std::cout << "Gpio  value: " << v.first << "->" << v.second
                          << "\n";
            });
            std::cout << "Press [enter]" << std::flush;
            getchar();
            iface->writetest("value", {{415, "1"}, {420, "1"}});
            val.clear();
            iface->read("value", val);
            std::ranges::for_each(val, [](const auto& v) {
                std::cout << "Gpio  value: " << v.first << "->" << v.second
                          << "\n";
            });

            std::string ret;
            iface->write(419, "value", "1");
            iface->read(419, "value", ret);
            std::cout << "Gpio  value: " << 419 << " -> " << ret << "\n";

            std::cout
                << "Second scenario DONE -> destroying/unexporting iface\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
