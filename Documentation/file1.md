# Documentation
## Для вывода текста в терминал используется комманда ```printlnSerial()``` и ```printSerial```

```swift
print.Serial(123)
print.lnSerial("Hello, World!")
```
## Для вывода картинки на дисплей используются комманды ```displayText```, ```displayRect```, ```displayClear```, ```displayPixel```, ```dispaly```
```swift
print.displayText("text", x, y)
print.displayRect(x, y, width, height)
displayClear()
print.displayPixel(x, y)
print.display(varible, x, y) 
```
## управление пинами
```swift
pin.read(an, 2)
pin.write(dg, 3, HIGH)
```