# Отчет по 7 ЛР

### Цель

Изучить и реализовать метод сегментации изображений. Рассмотреть возможные варианты оценки качества сегментации с использованием эталонной разметки и реализовать функционал для этого.

### Ход работы

В ходе выполнения седьмой лабораторной работы была написана программа, которая генерирует тестовое изображение из json с заданными параметрами и производит Edge-Based сегментацию объектов изображения с последующей оценкой метрик. 

Edge-Based сегментация - этот метод основывается на том, что границы объектов в изображениях обычно соответствуют резким изменениям интенсивности, цвета или текстуры.

Тестовые изображения с различными параметрами генерации:

1. size - 200x200, background color = 100, blur = 2.0, noise = 3.0:

![example1](assets/example1.png "example1")

2. size - 375x375, background color = 85, blur = 2.0, noise = 3.0:

![example2](assets/example2.png "example2")

3. size - 475x475, background color = 65, blur = 2.0, noise = 5.0:

![example3](assets/example3.png "example3")

Соответствующие результаты сегментации:

1.
|![example1](assets/example1.png "example1") | ![result1](assets/result1.png "result1") |

![quality1](assets/quality1.png "quality1")

2.
|![example2](assets/example2.png "example2") | ![result2](assets/result2.png "result2") |

![quality2](assets/quality2.png "quality2")

3.
|![example3](assets/example3.png "example3") | ![result3](assets/result3.png "result3") |

![quality3](assets/quality3.png "quality3")

