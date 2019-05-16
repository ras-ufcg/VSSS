# VSSS Computational Vision System using OpenCV+Qt
<b>Real-time multi robot tracking system</b>

In this program robots in a live camera feed are detected and tracked on a Very Small Size Soccer (VSSS) game. As frames are passed through the camera feed, their RGB attributes are converted into the HSV type. This provides better depth so the algorithm can understand all the different colors and their hues in the image. Through that, the min/max HSV attributes are altered until the resulting threshold image has the objects of interests isolateds, the ball and players colors, thereby providing a quick and responsive players tracking mechanism.

The system allows tracking multiple robots by storing their attributes in a container. Each object that is to be added is given a name of the user's choice and their corresponding HSV attributes are collected in a struct. The next time the object appears, it is tracked and the tag appears along with it.

Development was done in C++ on the Qt Framework.
