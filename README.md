# Scanner in CPP

This is a simple C++ based code using OpenCV to upload an image, transform and display it on the screen. The final image looks like a scanne image which is the result of local thresholding. 
The code has two modes, first is an automatic mode where corner point detection is automatic. If for some reason the corner point detection is not accurate then the user has the option to choose them manually. Do as instructed to enter the manual mode and select the four corners with left-mouse button click starting from the top left corner in a clockwise manner. After selecting the four corner points press any key to continue.

#HOW TO USE THE CODE (LINUX)
1. Open the project folder in the terminal
2. $ mkdir build/
3. $ cd build
4. $ cmake ..
5. $ make 
6. $ ./scanner++

#Requirements
1. OpenCV 

