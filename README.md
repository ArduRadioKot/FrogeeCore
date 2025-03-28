Что это?
Зачем это?
Как установить?
Элементная база
Что доступно?
Где писать программу?
Простой пример
Синтаксис(немного с сылкой на документцию)
Как загружать программу 

## Что это?
 FrogeeCore(далее Ядро) - это платфома, предназначенная для быстрого прототипирования встраиваемых систем. Ядро можно использовать для разного типа задачь, таких как, разработка IoT устройств, автономные носимые устройства, обучение и другое.
 Основой идеи Ядра послужили Flipper Zero, M5Stick и M5Atom. 
## Зачем это?
Идея Ядра заключается в быстром прототипровании устройств, за счёт большго количества встроенных библиотек, для работы с внешними устройствами или для работой со встроенными устройствами и модуль. С помощью Ядра можно создавать различные устройства и при этом каждый может создавать свои вариации ядра для своих целей

## Как установить?
Разработка может провоиться как в редакторе кода, так и в любом текстовом редакторе, получившийся код нужно записать на СД-карту и втавить её в устройство.
Можно установть и запустить скрипт на питоне:
	Необходимые библиотеки:
		customtkinter
Либо загрузить исполняемый файл из релиза.

## Элементная база
- esp32 c3 supermini
- bmp280
- mpu6050
- oled ssd1306 дисплей 128 * 64 пикселя
- аккумуляторная батарея(акб)
- модуль зарядки акб tp4056
- 3 функциональные кнопки
## Что доступно?
8 выведеных пинов:
- 2 пина питания
- 2 пина интерфейса i2c
- 4 GPIO, из которых 2 можно назначить на интерфейс UART

## Где писать программу?
- В редакторе кода:
    ![[photo_2025-03-28_12-29-02.jpg]]
- В любом текстовом редакторе.

## Простой пример
```swift
display.begin()
display.print(0,0, "Hello, World!")
```

Сохраняем как helloword.txt и запускаем на устройстве.





What's it? 
Why is that?
How do I install it?
Element base What is available? 
Where to write a program?
A simple example Syntax (a bit with a link to the documentation)
How to download the program

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
- ## What is available? 
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