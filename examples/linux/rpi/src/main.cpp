#include "log/interfaces/console/log.hpp"
#include "sysfs/interfaces/linux/sysfs.hpp"

#include <iostream>

int main(int argc, [[maybe_unused]] char** argv)
{
    try
    {
        if (argc == 2)
        {
            std::cout << "First scenario -> reading file\n";
            using namespace sysfs::lnx;
            auto loglvl =
                (bool)atoi(argv[1]) ? logs::level::debug : logs::level::info;
            std::string path{"/sys/class/thermal"};
            auto logif = logs::Factory::create<logs::console::Log,
                                               logs::console::config_t>(loglvl);
            auto iface =
                sysfs::Factory::create<Sysfs, configrw_t>({path, logif});

            std::string val;
            iface->read("thermal_zone0/temp", val);
            std::cout << "Read temperature value: " << val << "\n";
            std::cout << "First scenario DONE -> destroying iface\n";
        }
        if (argc == 3)
        {
            std::cout << "Second scenario -> export/write/read\n";
            using namespace sysfs::lnx;
            auto loglvl =
                (bool)atoi(argv[1]) ? logs::level::debug : logs::level::info;
            std::string path{"/sys/class/gpio/"};
            auto logif = logs::Factory::create<logs::console::Log,
                                               logs::console::config_t>(loglvl);
            auto iface = sysfs::Factory::create<Sysfs, configexportrw_t>(
                {path, "gpio", 415, logif});

            std::string val;
            iface->read("direction", val);
            std::cout << "Gpio direction: " << val << "\n";
            iface->write("direction", "out");
            iface->read("direction", val);
            std::cout << "Gpio direction: " << val << "\n";

            iface->read("value", val);
            std::cout << "Gpio value: " << val << "\n";
            std::cout << "Press [enter]" << std::flush;
            getchar();
            val = val == "0" ? "1" : val;
            iface->writetest("value", val);
            iface->read("value", val);
            std::cout << "Gpio value: " << val << "\n";
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
