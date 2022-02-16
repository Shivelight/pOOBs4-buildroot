#!/bin/sh

# Symlinks AP6212 NVRAM config
ln -sf brcmfmac43430-sdio.AP6212.txt $TARGET_DIR/lib/firmware/brcm/brcmfmac43430-sdio.txt

exit 0
