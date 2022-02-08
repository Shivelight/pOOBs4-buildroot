#!/bin/sh

sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' $TARGET_DIR/etc/ssh/sshd_config
sed -i 's/#PermitEmptyPasswords no/PermitEmptyPasswords yes/' $TARGET_DIR/etc/ssh/sshd_config

exit 0
