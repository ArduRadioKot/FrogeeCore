
# FrogeeCore
## What is it? 
FrogeeCore (hereinafter referred to as the Core) is a platform designed for rapid prototyping of embedded systems. The core can be used for various types of tasks, such as IoT device development, autonomous wearable devices, training, and more. The Core idea was based on Flipper Zero, M5Stick and M5Atom. 
## Why is this? 
The idea of the Core is to quickly prototype devices, due to a large number of built-in libraries, to work with external devices or to work with embedded devices and a module. With the help of the Kernel, you can create different devices and at the same time, everyone can create their own variations of the kernel for their own purposes. 
## How to install?
Development can take place both in the code editor and in any text editor, the resulting code must be written to a SD card and inserted into the device. You can install and run the script in python: Required libraries: customtkinter Or download the executable file from the release. 
## Element base 
- esp32 c3 supermini 
- bmp280 
- mpu6050 
- OLED SSD1306 DISPLAY 128 * 64 pixels 
- rechargeable battery(battery)  
- TP4056 battery charging module 
- 3 function buttons 
## What is available? 
- 8 pins withdrawn: 
- 2 power pins 
- 2 i2c interface pins 
- 4 GPIOs, of which 2 can be assigned to the UART interface 
- ## Where to write the program? 
- In the code editor: 

	![[photo_2025-03-28_12-29-02 1.jpg]]

- In any text editor. 

## A simple example 
```swift
display.begin() 
display.print(0,0, "Hello, World!") 
```
We save it as helloword.txt and run on the device.