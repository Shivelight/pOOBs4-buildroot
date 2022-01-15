#!/bin/sh

if ! grep -Fq "iface wlan0 inet static" output/target/etc/network/interfaces ; then
	echo "
# for wlan0
auto wlan0
iface wlan0 inet static 
  hostname \$(hostname)
  address 10.0.0.1
  netmask 255.0.0.0
  network 10.0.0.0
  broadcast 10.255.255.255" >> output/target/etc/network/interfaces
fi

exit 0
