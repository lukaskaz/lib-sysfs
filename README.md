# lib-sysfs
Library for using sys filesystem
<br><br>
**First example of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 0<br>
First scenario -> reading file<br>
Read temperature value: 55650<br>
First scenario DONE -> destroying iface<br>
pi@raspberrypi:~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 0<br>
First scenario -> reading file<br>
Read temperature value: 55650<br>
First scenario DONE -> destroying iface<br>
pi@raspberrypi:~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 1<br>
First scenario -> reading file<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: '57300' from 'thermal_zone0/temp'<br>
Read temperature value: 57300<br>
First scenario DONE -> destroying iface<br>
pi@raspberrypi:~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 1<br>
First scenario -> reading file<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: '55650' from 'thermal_zone0/temp'<br>
Read temperature value: 55650<br>
First scenario DONE -> destroying iface<br>
<br>
**Second example of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 0 1<br>
Second scenario -> export/write/read<br>
Gpio direction: out<br>
Gpio direction: out<br>
Gpio value: 0<br>
Press [enter]<br>
Gpio value: 1<br>
Second scenario DONE -> destroying/unexporting iface<br>
pi@raspberrypi:~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 1 1<br>
Second scenario -> export/write/read<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written: '415' to 'export'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/gpio/gpio415<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: 'out' from 'direction'<br>
Gpio direction: out<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written: 'out' to 'direction'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: 'out' from 'direction'<br>
Gpio direction: out<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: '0' from 'value'<br>
Gpio value: 0<br>
Press [enter]<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written: '1' to 'value'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: '1' from 'value'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read: '1' from 'value'<br>
Gpio value: 1<br>
Second scenario DONE -> destroying/unexporting iface<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written: '415' to 'unexport'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/gpio/gpio415<br>
