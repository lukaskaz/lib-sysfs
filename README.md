# lib-sysfs
Library for using sys filesystem (read, write, [un]export, oneshot triggers, periodic triggers)
<br><br>
**First example (sysfs read) of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 0<br>
First scenario -> reading file<br>
Read temperature value: 53450<br>
First scenario DONE -> destroying iface<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 0<br>
First scenario -> reading file<br>
Read temperature value: 52900<br>
First scenario DONE -> destroying iface<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 1<br>
First scenario -> reading file<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[]: '51800' from 'thermal_zone0/temp'<br>
Read temperature value: 51800<br>
First scenario DONE -> destroying iface<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 1<br>
First scenario -> reading file<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[]: '52350' from 'thermal_zone0/temp'<br>
Read temperature value: 52350<br>
First scenario DONE -> destroying iface<br>
<br>
**Second example (sysfs export + read + write + unexport) of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 0 2<br>
Second scenario -> export/write/read<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/gpio/gpio415<br>
Gpio direction: out<br>
Gpio direction: out<br>
Gpio value: 0<br>
Press [enter]<br>
Gpio value: 1<br>
Second scenario DONE -> destroying/unexporting iface<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/gpio/gpio415<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/rpi/prog 1 2<br>
Second scenario -> export/write/read<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[415]: '415' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/gpio/gpio415<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[415]: 'out' from 'direction'<br>
Gpio direction: out<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[415]: 'out' to 'direction'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[415]: 'out' from 'direction'<br>
Gpio direction: out<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[415]: '0' from 'value'<br>
Gpio value: 0<br>
Press [enter]<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[415]: '1' to 'value'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[415]: '1' from 'value'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::read(const std::string&, std::string&, bool) const] Read[415]: '1' from 'value'<br>
Gpio value: 1<br>
Second scenario DONE -> destroying/unexporting iface<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[415]: '415' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/gpio/gpio415<br>
<br>
**Third example (oneshot trigger) of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/trigger/oneshot/prog 0<br>
First scenario -> deploying oneshot trigger<br>
[INFO] Created oneshot trigger: /sys/bus/iio/devices/iio_sysfs_trigger/trigger2 -> sysfstrig1<br>
Trigger created, to run press [enter]Trigger activated, to exit press [enter]<br>
First scenario DONE -> destroying iface<br>
[INFO] Removed oneshot trigger: /sys/bus/iio/devices/iio_sysfs_trigger/trigger2 -> sysfstrig1<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/trigger/oneshot/prog 1<br>
First scenario -> deploying oneshot trigger<br>
[DBG ] File /sys/bus/iio/devices/iio_sysfs_trigger/trigger0/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'sysfstrig0' from '/sys/bus/iio/devices/iio_sysfs_trigger/trigger0/name'<br>
[DBG ] Found existing os timer trigger0:sysfstrig0<br>
[DBG ] File /sys/bus/iio/devices/iio_sysfs_trigger/trigger1/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'sysfstrig5' from '/sys/bus/iio/devices/iio_sysfs_trigger/trigger1/name'<br>
[DBG ] Found existing os timer trigger1:sysfstrig5<br>
[DBG ] Found available os trigger id: 1<br>
[DBG ] File /sys/bus/iio/devices/iio_sysfs_trigger/add_trigger elevated to: 'o+w'<br>
[DBG ] Written[]: '1' to '/sys/bus/iio/devices/iio_sysfs_trigger/add_trigger'<br>
[DBG ] File /sys/bus/iio/devices/iio_sysfs_trigger/trigger2/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'sysfstrig1' from '/sys/bus/iio/devices/iio_sysfs_trigger/trigger2/name'<br>
[DBG ] Found os trigger directory name: trigger2<br>
[DBG ] File /sys/bus/iio/devices/iio_sysfs_trigger/trigger2/trigger_now elevated to: 'o+w'<br>
[INFO] Created oneshot trigger: /sys/bus/iio/devices/iio_sysfs_trigger/trigger2 -> sysfstrig1<br>
Trigger created, to run press [enter][DBG ] Written[]: '1' to '/sys/bus/iio/devices/iio_sysfs_trigger/trigger2/trigger_now'<br>
[DBG ] Trigger sysfstrig1 activated<br>
Trigger activated, to exit press [enter]<br>
First scenario DONE -> destroying iface<br>
[DBG ] File /sys/bus/iio/devices/iio_sysfs_trigger/remove_trigger elevated to: 'o+w'<br>
[DBG ] Written[]: '1' to '/sys/bus/iio/devices/iio_sysfs_trigger/remove_trigger'<br>
[INFO] Removed oneshot trigger: /sys/bus/iio/devices/iio_sysfs_trigger/trigger2 -> sysfstrig1<br>
<br>
**Forth example (periodic trigger) of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/trigger/periodic/prog 10 0<br>
First scenario -> deploying periodic trigger<br>
[INFO] Created periodic trigger: /sys/bus/iio/devices/trigger7 -> cnffstrig3<br>
Trigger created, to run press [enter]Trigger activated, to exit press [enter]<br>
First scenario DONE -> destroying iface<br>
[INFO] Removed periodic trigger: /sys/bus/iio/devices/trigger7 -> cnffstrig3<br>
pi@raspberrypi:\~/git/lib-sysfs/build $ ./examples/linux/trigger/periodic/prog 0.5 1<br>
First scenario -> deploying periodic trigger<br>
[DBG ] Found existing hr timer dir cnffstrig2<br>
[DBG ] Found existing hr timer dir cnffstrig1<br>
[DBG ] Found existing hr timer dir cnffstrig0<br>
[DBG ] Found available hr trigger id: 3<br>
[DBG ] File /sys/kernel/config/iio/triggers/hrtimer/ elevated to: 'o+w'<br>
[DBG ] File /sys/bus/iio/devices/trigger6/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'cnffstrig2' from '/sys/bus/iio/devices/trigger6/name'<br>
[DBG ] File /sys/bus/iio/devices/trigger4/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'cnffstrig0' from '/sys/bus/iio/devices/trigger4/name'<br>
[DBG ] File /sys/bus/iio/devices/trigger2/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'sysfstrig1' from '/sys/bus/iio/devices/trigger2/name'<br>
[DBG ] File /sys/bus/iio/devices/trigger0/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'sysfstrig0' from '/sys/bus/iio/devices/trigger0/name'<br>
[DBG ] File /sys/bus/iio/devices/trigger7/name elevated to: 'o+r'<br>
[DBG ] Read[]: 'cnffstrig3' from '/sys/bus/iio/devices/trigger7/name'<br>
[DBG ] Found hr trigger directory name: trigger7<br>
[DBG ] File /sys/bus/iio/devices/trigger7/sampling_frequency elevated to: 'o+w'<br>
[DBG ] Written[]: '0.500000' to '/sys/bus/iio/devices/trigger7/sampling_frequency'<br>
[INFO] Created periodic trigger: /sys/bus/iio/devices/trigger7 -> cnffstrig3<br>
Trigger created, to run press [enter]Trigger activated, to exit press [enter]<br>
First scenario DONE -> destroying iface<br>
[INFO] Removed periodic trigger: /sys/bus/iio/devices/trigger7 -> cnffstrig3<br>
