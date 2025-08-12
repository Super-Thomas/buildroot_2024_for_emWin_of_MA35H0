# buildroot_2024_for_emWin_of_MA35H0

## Intro
This Buildroot can be used to port Embedded Linux with emWin v6.40a.1 to the Nuvoton MA35H0 evaluation board.

## Development environment
### Hardware
|N|Name|Description|Note|
|---|---|---|---|
|1|NuMaker-HMI-MA35H0-A1|MA35H0 Evaluation board|Version 1.0|

### Software
|N|Name|Description|Note|
|---|---|---|---|
|1|emWin|emWin Graphics library|Version 6.40a.1|

## How to build
1) Run 'make numaker_hmi_ma35h04f70_for_emwin_defconfig'.
2) Run 'make'.
3) Do download output/images/pack-core-image-buildroot-ma35h0-hmi-128m-sdcard.bin file to MA35H0 EVB with NuWriter.

### Results
<img width="765" height="560" alt="image" src="https://github.com/user-attachments/assets/9a376574-cda4-4c50-9e86-cdae6338bf8c" /><br />

## Embedded Linux for LVGL of MA35H0
Please refer https://github.com/Super-Thomas/buildroot_2024_for_LVGL_with_LinuxFB_of_MA35H0

## Thanks to
https://github.com/OpenNuvoton/buildroot_2024<br />
https://www.nuvoton.com/products/microprocessors/arm-cortex-a35-mpus/ma35h0-industrial-hmi-series/?group=Software&tab=2
