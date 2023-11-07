## Задание от Отдела разработки решений для мобильных устройств на базе ОС ЛК

Требуется написать программу, которая читает произвольную CSV-форму из файла (количество строк и столбцов может быть любым), вычисляет значения ячеек, если это необходимо, и выводит получившуюся табличку в виде CSV-представления в консоль и файл. Подразумевается, что на вход всегда подается корректный CSV-файл, его дополнительная валидация не требуется.

В заголовке CSV файла перечислены названия столбцов. В ячейках файла могут храниться или целые числа, или выражения, состоящие из цепочки операций (+, -, *, /, унарный -).

Формат для выражений:
= ARG1 OP ARG2 OP …

Количество аргументов в выражении может быть любым. Между аргументом и операцией может быть произвольное число пробелов.

В случае некорректного выражения и/или наличия циклических ссылок в ячейку записывается «Invalid Value».

Важные особенности реализации:
* Необходимо использовать систему сборки CMake.
* Разрешается использование только стандартной библиотеки C++, стандарт языка – любой
* Имя CSV-файла передается первым аргументом программы.
* Необходимо написать юнит тесты на основной функционал утилиты (в качестве тестового фреймворка можно использовать gtest)

### Запуск программы
В качестве фреймворка используется gtest.
Установка фреймворка.
```shell
git clone git@github.com:google/googletest.git
```

Сборка проекта
```shell
mkdir build && cd build
cmake ..
make
./csvreader <file.csv>
```

### Запуск тестов
```shell
cd tests
./csvreaderTests
```

Юнит тесты лежат в /tests/CsvFiles. 