# ����� �� 4 ��

### ����

������� � ����������� ��������� ������ ����������� �����������. ����������� ��������� �������� �������� GUI ��� ���������� ���������� �����������, � ����� ����������� ���������� ��� �������������� �������� ����������� � ������������ ������ �������� ��������������.

### ��� ������

� ���� ���������� ��������� ������������ ������ ���� �������� ���������, ������� ���������� �������� ����������� � ��������� ����������� � ��������� ��������� ��������� �����������. ����� ��� ������������ ����� ���������� ��������� ���������� ����������� ����� GUI, ���������� ���������� ��� �������������� �������� �� ����������� � ��� ������ �������� ������ ����������.

1. ����������� � ����������� ��������� countCircles=10, minRadius=5, maxRadius=15, minContrast=50, maxContrast=255, blur=4, noise=4:

![example](assets/example.png "example")

������ ������ GUI:

����������� ��������� ����������� ������� ��������:

![bernsen](assets/bernsen.png "bernsen")

�������������� ��� ���������� radius = 14, CMin = 40:

![bernsenDetection](assets/bernsenDetection.png "bernsenDetection")

![bernsenkQuality](assets/bernsenQuality.png "bernsenQuality")

������ ������ GUI:

����������� ��������� ����������� ������� �������:

![niblack](assets/niblack.png "niblack")

�������������� ��� ���������� Radius = 24, k Value = 9, d Value = 18:

![niblackDetection](assets/niblackDetection.png "niblackDetection")

![niblackQuality](assets/niblackQuality.png "niblackQuality")

2. ����������� � ����������� ��������� countCircles=5, minRadius=5, maxRadius=15, minContrast=25, maxContrast=200, blur=3, noise=5:

![example2](assets/example2.png "example2")

�������������� ��� ���������� radius = 14, CMin = 40:

![bernsenDetection2](assets/bernsenDetection2.png "bernsenDetection2")

![bernsenkQuality](assets/bernsenQuality.png "bernsenQuality")

�������������� ��� ���������� Radius = 8, k Value = 9, d Value = 7:

![niblackDetection2](assets/niblackDetection2.png "niblackDetection2")

![niblackQuality2](assets/niblackQuality2.png "niblackQuality2")

3. ����������� � ����������� ��������� countCircles=7, minRadius=5, maxRadius=20, minContrast=40, maxContrast=180, blur=3, noise=7:

![example3](assets/example3.png "example3")

�������������� ��� ���������� radius = 10, CMin = 30:

![bernsenDetection3](assets/bernsenDetection3.png "bernsenDetection3")

![bernsenkQuality3](assets/bernsenQuality3.png "bernsenQuality3")

�������������� ��� ���������� Radius = 20, k Value = 9, d Value = 23:

![niblackDetection3](assets/niblackDetection3.png "niblackDetection3")

![niblackQuality3](assets/niblackQuality3.png "niblackQuality3")

### ������ ����������� ����������

��� ������� ����������� ����������, ����� ����� ����������� ���� � ����� ���������� � ��������� � �������:

```.\lab04.exe <countCircles> <minRadius> <maxRadius> <minContrast> <maxContrast> <blur> <noise> <method("bernsen", "niblack", "detection")>```

����� ����� ������ ��������� ����������� ����, ��������������� ��������� � ����� ���������.

### �����

��� ����� ��������, ������� ������������� �����, ��� ����������� ������� �������.