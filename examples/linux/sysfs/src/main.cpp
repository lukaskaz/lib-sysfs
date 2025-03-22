#include "logs/interfaces/console/logs.hpp"
#include "logs/interfaces/group/logs.hpp"
#include "logs/interfaces/storage/logs.hpp"
#include "sysfs/helpers.hpp"
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

            auto logconsole = logs::Factory::create<logs::console::Log,
                                                    logs::console::config_t>(
                {loglvl, logs::time::hide, logs::tags::hide});
            auto logstorage = logs::Factory::create<logs::storage::Log,
                                                    logs::storage::config_t>(
                {loglvl, logs::time::show, logs::tags::show, {}});
            auto logif =
                logs::Factory::create<logs::group::Log, logs::group::config_t>(
                    {logconsole, logstorage});

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

            auto logconsole = logs::Factory::create<logs::console::Log,
                                                    logs::console::config_t>(
                {loglvl, logs::time::hide, logs::tags::hide});
            auto logstorage = logs::Factory::create<logs::storage::Log,
                                                    logs::storage::config_t>(
                {loglvl, logs::time::show, logs::tags::show, {}});
            auto logif =
                logs::Factory::create<logs::group::Log, logs::group::config_t>(
                    {logconsole, logstorage});

            auto iface = sysfs::Factory::create<Sysfs, configexportrw_t>(
                {path, "gpio", 592, logif});

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
            uint32_t numval = val == "0" ? 1 : 0;
            iface->writetest("value", sysfs::str(numval));
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
