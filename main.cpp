#include "scan.h"

using namespace std;
using namespace cv;

vector<int> px, py;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if(event == EVENT_LBUTTONDOWN)
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
          px.push_back(x);
          py.push_back(y); 
     }
}


bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) 
{
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return (i > j);
}


int main()
{
	//declare variables
	double ratio;
	Mat orig, gray, edges;
	vector<vector<Point>> contours1, screenCnt;
	int response = 0;

	//declare the object
	scanner object;

	//read the image, display it and find the ratio
    string image_path = "/home/novice/Downloads/test_1.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty())
    {
        cout << "Could not read the image: " << image_path <<endl;
        return 1;
    }

    object.display("original", img);
    ratio = static_cast<double>(img.rows)/500.0;

    // clone the original image for future use
    orig = img.clone();

    object.canny_edge(img,gray,edges);
    object.display("Canny edge detection", edges);

    //find the contours and sort it increasing contour size 
    threshold(edges, edges, 125, 255, THRESH_BINARY);
	findContours(edges.clone(), contours1,RETR_TREE, CHAIN_APPROX_NONE);
	sort(contours1.begin(), contours1.end(), compareContourAreas);

	object.draw_contour(img, contours1, screenCnt);
	object.display("Contour detection", img);
	object.scannedImage(orig, screenCnt, px, py);
    object.Print();

    cin>>response;

    if(response)
    {

		namedWindow("manual",WINDOW_NORMAL);
    	setMouseCallback("manual", CallBackFunc, NULL);	
		imshow( "manual", orig);
    	waitKey(0);

    	object.scannedImage(orig, screenCnt, px, py, "manual");

    }

    return 0;
}