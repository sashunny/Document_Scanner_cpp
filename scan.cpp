
#include "scan.h"

scanner::scanner()
{

}


Mat scanner::four_point_transform(Mat &image, Point tl, Point tr, Point bl, Point br)
{
    Mat lambda(2, 4, CV_32FC1); 

    //compute the width of the new image
    double widthA = sqrt(pow((br.x - bl.x),2) + pow((br.y - bl.y),2));
    double widthB = sqrt(pow((tr.x - tl.x),2) + pow((tr.y - tl.y),2));
    int maxWidth = max(static_cast<int>(widthA), static_cast<int>(widthB));

    //compute the height of the new image
    double heightA = sqrt(pow((tr.x - br.x),2) + pow((tr.y - br.y),2));
    double heightB = sqrt(pow((tl.x - bl.x),2) + pow((tl.y - bl.y),2));
    int maxHeight = max(static_cast<int>(heightA), static_cast<int>(heightB));

    lambda = Mat::zeros(image.rows, image.cols, image.type());

    // construct the input image by cropping the ROI 
    Point2f inputQuad[4];

    inputQuad[0] = Point2f(tl.x, tl.y);
    inputQuad[1] = Point2f(tr.x, tr.y);
    inputQuad[2] = Point2f(br.x, br.y);
    inputQuad[3] = Point2f(bl.x, bl.y);

    //create the output matrix based on the max width and height 
    Point2f outputQuad[4];

    outputQuad[0] = Point2f(0,0);
    outputQuad[1] = Point2f(maxWidth-1,0);
    outputQuad[2] = Point2f(maxWidth-1, maxHeight-1);
    outputQuad[3] = Point2f(0,maxHeight-1);

    Mat output = Mat::zeros(maxHeight, maxWidth, image.type());

    //compute the perspective transform matrix and display a top down view 
    lambda = getPerspectiveTransform(inputQuad, outputQuad);
    warpPerspective(image,output,lambda,output.size());

    return output;
}


void scanner::display(string s, Mat &m)
{
    namedWindow(s, WINDOW_NORMAL);
    imshow(s, m);
    waitKey(0);
}


void scanner::canny_edge(Mat &m, Mat &gray, Mat &edges)
{
    //convert the image to grayscale, blur it, find its edges and display it.
    cvtColor(m, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 25);
    GaussianBlur(gray, gray, Size(5, 5), 0);
    Canny(gray, edges, 100, 200, 3, false);
}


void scanner::draw_contour(Mat &img, vector<vector<Point>> &contours1, vector<vector<Point>> &screenCnt)
{
    vector<Point> approx;

    for(size_t i = 0; i< contours1.size(); i++)
    {
        //approximate the polygon
        double peri = arcLength(contours1[i], true);
        approxPolyDP(contours1[i], approx, 0.02*peri, true);

        //approximated contour has to have four points
        if(approx.size() == 4)
        {
            screenCnt.push_back(approx);
            break;
        }   
    }

    //draw and display the contours
    drawContours(img, screenCnt, -1, Scalar(0, 255, 0), 3);
}


void scanner::scannedImage(Mat &orig, vector<vector<Point>> &screenCnt , vector<int> px, vector<int> py, string input)
{
    Mat warped;

    if(input == "automatic")
        warped = four_point_transform(orig, Point(screenCnt[0][1].x, screenCnt[0][1].y),Point(screenCnt[0][0].x, screenCnt[0][0].y),Point(screenCnt[0][2].x, screenCnt[0][2].y),Point(screenCnt[0][3].x, screenCnt[0][3].y));
    else
        warped = four_point_transform(orig, Point(px[0], py[0]),Point(px[1], py[1]),Point(px[3], py[3]),Point(px[2], py[2]));

    cvtColor(warped, warped, COLOR_BGR2GRAY);
    display("transformed image", warped);

    //to give the image a scanned paper like appearance
    threshold(warped, warped, 0, 255, THRESH_BINARY | THRESH_OTSU);
    display("scanned image", warped);
}


void scanner::Print()
{
    cout<<"Are you happy with the output? if not you can select the four corners yourself with mouse clicks."<<endl;
    cout<<"Click on the corners in a CLOCKWISE manner starting from the TOP-LEFT corner."<<endl;
    cout<<"Press 1 if you want to continue and press any key to stop registering mouse clicks after 4 clicks"<<endl;
}