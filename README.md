# pOOBs4-buildroot

## Introduction

This repository contains [buildroot] [external tree][br-external] for building a minimal Linux image to host [pOOBs4] web server and emulate its exfathax USB.

## Building

Clone the repository and download/clone buildroot 2022.02:

```sh
git clone --recurse-submodules https://github.com/Shivelight/pOOBs4-buildroot
git clone -b 2022.02 --depth 1 https://github.com/buildroot/buildroot
```
Alternatively, you can shallow clone:

```sh
git clone --depth 1 --recurse-submodules --shallow-submodules https://github.com/Shivelight/pOOBs4-buildroot.git
git clone -b 2022.02 --depth 1 https://github.com/buildroot/buildroot
```
Configure buildroot to use BR2_EXTERNAL tree and start building. Replace `<your_board_defconfig>` with your board defconfig available in [configs/](configs) directory or from the [Supported Board](#supported-board) section:

```sh
cd buildroot
make BR2_EXTERNAL=../pOOBs4-buildroot/ <your_board_defconfig>
make
```

The final image is saved here `output/images/sdcard.img`.

### Supported Board

⚠️ _If you have a board that is not listed, feel free to contribute!_ ⚠️

Below is the currently supported board with defconfig ready to use.

- Banana Pi M2 Zero [ `bananapi_m2_zero_poobs4_defconfig` ]
- Orange Pi Zero / Orange Pi Zero LTS [ `orangepi_zero_poobs4_defconfig` ]
- Orange Pi Zero Plus2 [ `orangepi_zero_plus2_poobs4_defconfig` ]
- Raspberry Pi 4 Model B [ `raspberrypi4_poobs4_defconfig` ]
- Raspberry Pi Zero W [ `raspberrypi0w_poobs4_defconfig` ]
- Raspberry Pi Zero 2 W [ `raspberrypizero2w_poobs4_defconfig` ]
- your board?

_Note: You need a board with a USB OTG port for exfathax emulation. All boards listed should have a built-in USB OTG port unless stated otherwise._

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

### Bring your own exploit host

If the default host is not your taste, you are free to use your favorite host. All you need to do is to include this snippet and call the right script at the right time, usually before `alert();` and after the exploit is done.

```javascript
function CallCgi(script) {
    var xmlHttpRequest = new XMLHttpRequest();
    // GET request is not working on PS4?
    xmlHttpRequest.open("POST", "/cgi-bin/" + script, true);
    xmlHttpRequest.send();
}
```

Example:

```javascript
...
chain.run();
// Load exfathax emulation
CallCgi("load_mass_storage");
alert("\n\n⚠⚠⚠ Emulating exfathax USB ⚠⚠⚠\nClick OK when you see the 'USB unsupported' popup notification.");
{
	for (var i = 1; i < NUM_KQUEUES; i += 2) {
		chain.fcall(window.syscalls[6], kqueues[i]);
	}
}
chain.run();
// Unload exfathax emulation
CallCgi("unload_mass_storage");
if (chain.syscall(23, 0).low == 0) {
	return;
}
alert("Exploit Failed! You can try again but it is advisable to reboot instead.");
p.write8(0, 0);
return;
...
```

#### Installing your host

Delete everything inside the `httpd` root directory (default: `/var/www/html`) but keep:

- `cgi-bin` directory
- `exfathax.img` /`exfathax_pico.img`
- `404.html`
- `redirect.manifest`

then copy your custom host. That's it.

Alternatively, you can keep the default host intact by changing `httpd` root directory in `/etc/httpd.conf`. Copy the files and directory listed above to your **new** `httpd` root. You may need to update the scripts for it to be working on the new root directory.

#### Available script

- `load_mass_storage`
- `unload_mass_storage`

You can add your own, the scripts are located at [/var/www/html/cgi-bin](board/poobs4/common/rootfs_overlay/var/www/html/cgi-bin)

## Versioning

Inspired by semver `major.minor.patch`, I decided to use `core.board.patch`.

- `core`: core functionality, increase when a new function is introduced.
- `board`: total supported board, does not reset when `core` is incremented.
- `patch`: incremented when core function or board specific bug fix/change is introduced.

## Other Projects

- [PS4RaspberryPi](https://github.com/PaulJenkin/PS4RaspberryPi): Similar project with more features, only Raspberry Pi boards are supported.
- [ESP32-Server-900u](https://github.com/stooged/ESP32-Server-900u): pOOBs4 on ESP32-S2 / ESP32-S3.

## License

This project is licensed under the **GPL-2.0 license**.

See [LICENSE](LICENSE) for more information.

[pOOBs4]: https://github.com/ChendoChap/pOOBs4
[payload-guest]: https://github.com/Al-Azif/ps4-payload-guest
[buildroot]: https://buildroot.org
[br-external]: https://buildroot.org/downloads/manual/manual.html#outside-br-custom
