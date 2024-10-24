# Основные элементы синтаксиса

## Объявление переменных:

Переменные объявляются с указанием типа, как в C/C++.
Пример:
```javascript
int x = 10;  // Объявление переменной
const int y = 20;  // Константа
```
## Функции:
Синтаксис объявления функций остается как в JavaScript, с указанием типов, как в Python.
```javascript
function add(int a, int b): int {
    return a + b;
}
```
## Условия и циклы:
Условия как в Python, но с использованием фигурных скобок и точек с запятой, как в JavaScript и C/C++.
```javascript
if (x > 0) {
    print("Положительное число");
} else {
    print("Неположительное число");
}
```
## Объекты и классы:
Синтаксис классов остается как в Python, но с использованием конструктора JavaScript.
```javascript
class Person {
    constructor(string name) {
        this.name = name;
    }

    greet() {
        print("Привет, " + this.name + "!");
    }
}
```
## Списки и массивы:
Списки объявляются как в Python, но с возможностью манипуляции через методы, как в JavaScript.
```javascript
int[] my_list = [1, 2, 3]; 
my_list.append(4);  // Добавление элемента
```
## Пример программы
```javascript

int[] numbers = [1, 2, 3, 4, 5];

function sum_array(int[] arr): int {
    int total = 0;
    for (int num : arr) {
        total += num;
    }
    return total;
}

print("Сумма: " + sum_array(numbers));
```