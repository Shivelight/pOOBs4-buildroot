#!/bin/sh

# Symlinks AP6212 NVRAM config
ln -sfr output/target/lib/firmware/brcm/brcmfmac43430-sdio.AP6212.txt output/target/lib/firmware/brcm/brcmfmac43430-sdio.txt

exit 0
