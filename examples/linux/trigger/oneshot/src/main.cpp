#include "logs/interfaces/console/logs.hpp"
#include "logs/interfaces/group/logs.hpp"
#include "logs/interfaces/storage/logs.hpp"
#include "trigger/interfaces/linux/oneshot/trigger.hpp"

#include <iostream>

int main(int argc, [[maybe_unused]] char** argv)
{
    try
    {
        if (argc == 2)
        {
            std::cout << "First scenario -> deploying oneshot trigger\n";
            using namespace trigger::lnx::oneshot;
            auto loglvl =
                (bool)atoi(argv[1]) ? logs::level::debug : logs::level::info;

            auto logconsole = logs::Factory::create<logs::console::Log,
                                                    logs::console::config_t>(
                {loglvl, logs::time::hide, logs::tags::hide});
            auto logstorage = logs::Factory::create<logs::storage::Log,
                                                    logs::storage::config_t>(
                {loglvl, logs::time::show, logs::tags::show, {}});
            auto logif =
                logs::Factory::create<logs::group::Log, logs::group::config_t>(
                    {logconsole, logstorage});

            auto iface = trigger::Factory::create<Trigger, config_t>({logif});

            std::cout << "Trigger created, to run press [enter]" << std::flush;
            iface->run();

            std::cout << "Trigger activated, to exit press [enter]"
                      << std::flush;
            getchar();

            std::cout << "First scenario DONE -> destroying iface\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
