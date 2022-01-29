# pOOBs4-buildroot

## Introduction

This repository contains [buildroot] [external tree][br-external] for building a minimal Linux image to host [pOOBs4] web server and emulate its exfathax USB.

## Building

Clone the repository and buildroot from my personal repository (will be pinned to buildroot 2022.02 when it's released):

```sh
git clone https://github.com/Shivelight/pOOBs4-buildroot
git clone -b pOOBs4 --depth 1 https://github.com/Shivelight/buildroot
```

Configure buildroot to use BR2_EXTERNAL tree and start building. Replace `<boardname>` with your board defconfig available in [configs/](configs) directory or from the [Supported Board](#supported-board) section:

```sh
cd buildroot
make BR2_EXTERNAL=../pOOBs4-buildroot/ <boardname>_poobs4_defconfig
make
```

The final image is saved here `output/images/sdcard.img`.

### Supported Board

⚠️ _If you have a board that is not listed, feel free to contribute!_ ⚠️

Below is the currently supported board with defconfig ready use. 

- Orange Pi Zero / Orange Pi Zero LTS [ `orangepi_zero_poobs4_defconfig` ]
- Raspberry Pi Zero W [ `not yet` ]
- Raspberry Pi Zero 2 W [ `not yet` ]
- your board?

_Note: You need a board with a USB OTG port for exfathax emulation. All boards listed should have a built-in USB OTG port unless stated otherwise._

<sub><sup>`not yet`: I'm willing to build and test, but currently don't have any and still waiting for restock :(</sup></sub>

## Installing

Download the image for your board from the release page or [build](#building) it yourself. Then write the image to your SD card using `dd`:

```sh
sudo dd if=output/images/sdcard.img of=/dev/sdX bs=4M
```

Alternatively, you can use:

- [USBImager](https://bztsrc.gitlab.io/usbimager/) on Windows/Mac/Linux 
- [balenaEtcher](https://www.balena.io/etcher/) on Windows/Mac/Linux 
- [Rufus](https://rufus.ie) on Windows

## Updating

This section is here to warn you to backup any customization you made or, better yet, fork this repo and do your customization there. Writing image to SD card destroy its existing content.

To update see [Installing](#installing).

## Running

Insert SD card and plug the board into PS4 using the USB OTG port. You don't need another power cable; your board will draw power from PS4. It may take 30 seconds to boot up for the first time. Afterward, it should only take ≤5 seconds.

_Note: SuperSpeed USB (USB 3.1 Gen 1), which PS4 uses, has a maximum power output of 5V/0.9A. Consider this._

### Connecting PS4 to the board

Follow these steps to connect your PS4 to the board:

1. On your PS4 go to `Settings` -> `Network` -> `Set Up Internet Connection` -> `Use Wi-Fi` -> `Custom`
2. Select **pOOBs4** on the list
3. When asked for the password, input: **12345678**
4. Now, for each step, select:
	1. IP Address Settings: Automatic
	2. DHCP Host Name: Do Not Specify
	3. DNS Settings: Automatic
	4. MTU Settings: Automatic
	5. Proxy Server: Do Not Use
5. You are set!

### Running the exploit

You can run the actual pOOBs4 exploit either by visiting http://10.0.0.1/ from the PS4 browser or from `Settings` -> `User's Guide/Helpful Info` -> `User's Guide`. The host is based on [Leeful's 9v4](https://github.com/Leeful/leeful.github.io/tree/master/9v4) (w/ GoldHEN v2.0b2), slightly modified to use the USB emulation.

#### Payload

Use [Payload Guest][payload-guest].

## Customization

### Accessing the board

You can SSH/SFTP to the board using the `root` user; the board IP is set to `10.0.0.1` (wlan) by default.

```sh
ssh root@10.0.0.1
```

The `root` user does not have a password. You can set a new password if you want using `passwd`. 

[pOOBs4]: https://github.com/ChendoChap/pOOBs4
[payload-guest]: https://github.com/Al-Azif/ps4-payload-guest
[buildroot]: https://buildroot.org
[br-external]: https://buildroot.org/downloads/manual/manual.html#outside-br-custom

## License

This project is licensed under the **GPL-2.0 license**.

See [LICENSE](LICENSE) for more information.
