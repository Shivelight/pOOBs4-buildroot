#!/bin/sh

sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' output/target/etc/ssh/sshd_config
sed -i 's/#PermitEmptyPasswords no/PermitEmptyPasswords yes/' output/target/etc/ssh/sshd_config

exit 0
