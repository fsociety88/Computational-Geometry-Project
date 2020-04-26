Intersection points between two simple polygons
=================================================
This is a code base to find intersection points between two simple polygons. For more details, refer the attached report. [*Report.pdf*]

>**Usage**

Compile and run *polygonIntersection.cpp*

```
n
x11 y11
x12 y12
...
x1n y1n
m
x21 y21
x22 y22
...
x2m y2m
```
Inputs are read through *stdin*. 

First line of the input is number of vertices in first polygon *n*. Next *n* lines contain coordinates of the vertices in anti-clockwise direction.

Next line of input contains the number of vertices in second polygon *m*. Next *m* lines contain coordinates of the vertices in anti-clockwise direction.

>**Random Polygon Generator**

Compile *random_polygon.cpp*.

```
random_polygon.exe [count] [mean] [stddev] [output_file]
```
- [count] means the number of the points
- [mean] the mean value for normal-distributed random points
- [stddev] the standard devirants for normal-distributed random points
- [output_file] the output file path for simple polygon points
