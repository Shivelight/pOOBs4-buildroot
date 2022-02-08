# hid-gadget-keyboard

hid-gadget-keyboard is a small program to send keystrokes to HID keyboard gadget driver.

Based on https://www.kernel.org/doc/html/latest/usb/gadget_hid.html

## Usage

```sh
hid-gadget-keyboard <device> [delay]
```

### Examples

Send the letter "a"

```sh
echo "a" | hid-gadget-keyboard /dev/hidg0
```

Send the letter "A"

```sh
echo "--left-shift a" | hid-gadget-keyboard /dev/hidg0
```

Hold the right arrow

```sh
echo "--hold --right" | hid-gadget-keyboard /dev/hidg0
```

Press the left arrow for 200ms before releasing

```sh
echo "--left" | hid-gadget-keyboard /dev/hidg0 200
```

Control-Alt-Delete combo

```sh
echo "--left-ctrl --left-alt --del" | hid-gadget-keyboard /dev/hidg0
```
Scripting example

```sh
#!/bin/sh

for key in a --left "--left-shift a" --right b --enter
do
    echo "$key" | hid-gadget-keyboard /dev/hidg0 200
done
```

eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

```sh
echo "--hold e" | hid-gadget-keyboard /dev/hidg0
```

### Issues

#### PS4 Dynamic Menu

Key press need to be delayed (around 100ms-200ms) for PS4 Dynamic Menu to respect the input.
