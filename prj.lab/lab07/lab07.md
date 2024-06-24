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





# **Purposes**
- [x] implement one segmentation method on lab.4 data
- [x] implement a method for assessing the quality of segmentation using reference markings
- [x] compare the segmentation method and binarization from lab.4

# Steps:

## 1.Edge_Based Segmentation

```C++
    double otsu_threshold = cv::threshold(img.src_image, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double low_threshold = otsu_threshold * 0.5;
    double high_threshold = otsu_threshold * 1.5;

    cv::Canny(img.src_image, edges, low_threshold, high_threshold);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    ...

    for (size_t i = 0; i < contours.size(); ++i) {
        cv::drawContours(img.result, contours, static_cast<int>(i), color, cv::FILLED);
        cv::drawContours(img.result_for_estimation, contours, static_cast<int>(i), color, cv::FILLED);
    }
```

The example of programms work:
![Result ](Images/Edge_based segmentation.png)

## 2.Estimation of results
Use the same idea as in 4 lab. 
```C++
    cv::Mat intersection = (img.src_image & gray_result);
    cv::Mat unionArea = (img.src_image | gray_result);

    double intersectionArea = cv::countNonZero(intersection);
    double unionAreaValue = cv::countNonZero(unionArea);

    double IoU = intersectionArea / unionAreaValue;
    double precision = intersectionArea / cv::countNonZero(gray_result);
    double recall = intersectionArea / cv::countNonZero(img.src_image);
    double f1Score = 2 * (precision * recall) / (precision + recall);
```

## 3.Compare results of the binarization and of segmentation

### **Calculated results for binarization from 4 lab:**

| Source picture | ![1S](Images/1S.png) | ![2S](Images/2S.png) | ![3S](Images/3S.png) | ![4S](Images/4S.png) | ![5S](Images/5S.png) |
| ----------- | -----------| --------- | ----------- | --------- | --------|
| Result picture | ![1R](Images/1R.png) | ![2R](Images/2R.png) | ![3R](Images/3R.png) | ![4R](Images/4R.png) | ![5R](Images/5R.png) |
| Precision | 0.498108 | 0.49994 | 0.499467 | 0.50004 | 0.499839 |
| Recall | 0.0277897 | 0.105774 | 0.049067 | 0.106125 | 0.133304 |
| F1 Score | 0.0526424 | 0.174607 | 0.0893559 | 0.17509 | 0.210475 |


### **Calculated results for segmentation:**

| Source picture | ![11S](Images/11S.png) | ![12S](Images/12S.png) | ![13S](Images/13S.png) | ![14S](Images/14S.png) | ![15S](Images/15S.png) |
| ----------- | -----------| --------- | ----------- | --------- | --------|
| Result picture | ![11R](Images/11R.png) | ![12R](Images/12R.png) | ![13R](Images/13R.png) | ![14R](Images/14R.png) | ![15R](Images/15R.png) |
| Intersection over union | 0.998266 | 0.998863 | 0.980634 | 0.999687 | 0.998866 |
| Precision | 0.998266 | 0.998863 | 0.980634 | 0.999687  | 0.998866 |
| Recall | 0.998266 | 0.998863 | 0.980641 | 0.999687  | 0.998866 |
| F1 Score | 0.998266 | 0.998863 | 0.980638 | 0.999687  | 0.998866 |

### **Comparing table:**

| | Binariztion | Segmentation |
| --- | --------- | ------------ |
| Mean presicion | 0.499603 | 0.995263 |
| Mean recall | 0.084359 | 0.995257 |
| Mean F1 Score | 0.14043406 | 0.995264 |

# Conclusions:
- Precision is much more better because of smaller percentage of false recalls and segmaentation don't have so wide range of perception, recall and F1 Score.
