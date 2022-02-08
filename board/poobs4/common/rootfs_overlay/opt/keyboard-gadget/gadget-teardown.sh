#!/bin/sh

cd $CONFIGFS_HOME/usb_gadget/keyboardgadget         # cd to gadget dir
echo "" > UDC                                       # disable the gadget
rm configs/c.1/hid.usb0                             # remove function from config
rmdir configs/c.1/strings/0x409                     # remove strings from config
rmdir configs/c.1                                   # remove config
rmdir functions/hid.usb0                            # remove function (function module is not unloaded)
rmdir strings/0x409                                 # remove strings from gadget
cd ..
rmdir keyboardgadget                                # remove the gadget
