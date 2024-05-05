# Raspberry Pi Pico 2040 vband compatible morse keyer interface

This implements a USB keyboard interface for a Raspberry Pi Pico 2040 to allow you to plug in your morse straight key or paddle into your computer. It should also work on the Pico-W but doesn't use any Wifi facilities so you don't need the more expensive unit.

Inputs

- Keyer on stereo 3.5mm audio jack 3 conductor TRS
    - GP16 should be T (tip)
    - GP17 should be connected to R (ring)
    - Ground should be connected S (shield)

Output

- A USB keyboard interface

When you plug it into your computer it will appear as a new keyboard

```
Bus 003 Device 051: ID cafe:4004 M5NCW Morse Key Keyboard
```

Note that you need a USB A to Micro USB B cable to plug in to the Pico. You've likely got lots of these lying around at home, however many of these cables are power only which **don't work**. I suggest you buy one from the same place you get the Pico if you aren't sure, or you can do what I did which was try about 5 cables before I found one which worked!

## Construction

This requires soldering a connector on to the Pico so a total of 6 solder joints. Or if you wish to do less soldering, buy a 3.5mm stereo extension lead and cut it in half. You'll only have 3 joints to solder then.

You could also build this on a breadboard but I'm imagining most radio amateurs will prefer to solder on a connector.

[Insert image here]

## Installing the firmware

Download the `vband.uf2` file from [FIXME] first.

1. Unplug the Pico from the USB for 5 seconds
2. Plug in while holding the button on the board
3. After 5 seconds release the button
4. Wait for the mass storage device to be mounted - this should pop up a window on your computer
5. Copy the `vband.uf2` firmware to this window - this will program the firware into the Pico
6. The Pico will unmount and restart running the firmware

Connect a paddle at this point.

- pressing the left paddle should cause the LED to come on
- pressing the right paddle should cause the LED to come on
- pressing both paddles should cause the LED to go off

If that looks like it is working, then connect to vband or one of the other programs in the compatible software section and give it a go!

## How it works

It essentially implements a two key USB keyboard

- `dit` Left CTRL
- `dah` Right CTRL

It uses the well developed USB libraries for the Raspberry Pi Pico to do this.

## Compatible software

- [vband](https://hamradio.solutions/vband/) for online practice QSOs.
- others coming soon!

## Building the firmware

Note that you **do not** need to build the firmware if you just want to use the interface - download the prebuilt `vband.uf2` from the downloads section.

First you will need to install the [pico 2040 C SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)

I installed this on my ubuntu machine by checking out the pico SDK and running this to install the compilers.

```
sudo apt install cmake gcc-arm-none-eabi
```

Point `PICO_SDK_PATH` at the path you installed the sdk and compile

```
export PICO_SDK_PATH=/opt/pico-sdk
mkdir build
cd build
cmake ..
make
```

This will produce a `vband.uf2` file in the `build` directory.

Send this to the board using the instructions above.

## Bugs

Please report bugs and request new features or send pull requests to GitHub.

## Licence

This is licenced under the permissive MIT licence. If someone wants to build a commercial version of this I'm totally fine with that as long as it is attributed to M5NCW.
