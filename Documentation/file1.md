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
## Управление пинами
```swift
pin.init(but, input)
pin.read(an, 2)
pin.write(dg, 3, HIGH)
```
## Переменнные 
```swift
var a = 0
```
## Константы
```swift
let b = 0
```
## Циклы
```swift
for(i = 0, i > 10, i++) {
    print.Serial(i)
    }
while(a>10){
    print.Serial(a)
    a++
}
```
## Функции
```swift
func add(a, b) {
    return a + b
    }
function add(a, b){
    return a + b
}
```
## Условия
```swift
if(a>10){
    print.Serial("a>10")
    }
```
## Списки
```swift
var list = [1, 2, 3]
```
## Словари
```swift
var dict = {"a":1, "b":2}
```
## Классы
```swift
class Person {
    var name = ""
    var age = 0
    }
```
## Наследование
```swift
class Person {
    var name = ""
    var age = 0
    }
    class Student: Person {
        var grade = 0
        }
```
## Интерфейсы
```swift
interface Person {
    var name = ""
    var age = 0
    }
```
## Абстрактные классы
```swift
abstract class Person {
    var name = ""
    var age = 0
    }
```
## Декораторы
```swift
@objc func myFunction() {
    print.Serial("Hello, world!")
    }
```
## Генераторы
```swift
func* myGenerator() {
    yield 1
    yield 2
    yield 3
    }
```
## Switch case
```swift
switch myVariable {
    case 1:
    print.Serial("One")
    break
    case 2:
    print.Serial("Two")
    break
    default:
    print.Serial("Unknown")
    break
    }
```
## Switch case с множеством значений
```swift
switch myVariable {
    case 1, 2, 3:
    print.Serial("One, Two or Three")
    break
    default:
    print.Serial("Unknown")
    break
```
## Switch case с диапазоном значений
```swift
switch myVariable {
case 1...3:
print.Serial("One, Two or Three")
break
default:
print.Serial("Unknown")
break
```
## Switch case с шаблоном
```python
switch myVariable {
    case let x where x > 10:
        print.Serial("Greater than 10")
        break
    default:
    print.Serial("Unknown")
    break
```
## Switch case с шаблоном и диапазоном
```swift
switch myVariable {
    case let x where x > 10 && x < 20:
    print.Serial("Between 10 and 20")
    break
```
## Switch case с шаблоном и множеством значений
```swift
switch myVariable {
    case let x where x == 1 || x == 2:
    print.Serial("One or Two")
    break
```
## Switch case с шаблоном, диапазоном и множеством значений
```swift
switch myVariable {
    case let x where x == 1 || x == 2 || x > 10 &&
    x < 20:
    print.Serial("One, Two or Between 10 and 20")
    break
```
## Switch case с шаблоном, диапазоном, множеством значений
```swift
switch myVariable {
    case let x where x == 1 || x == 2 || x > 10 &&
    x < 20 || x == 5:
    print.Serial("One, Two, Between 10 and 20 or Five")
    break
```
## Макросы
```c
#define MY_MACRO 5
```