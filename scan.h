
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


class scanner
{
	public:
		scanner();
		
		Mat four_point_transform(Mat &image, Point tl, Point tr, Point bl, Point br);
		void display(string s, Mat &m);
		void canny_edge(Mat &m, Mat &gray, Mat &edges);
		void draw_contour(Mat &img, vector<vector<Point>> &contours1, vector<vector<Point>> &screenCnt);
		void scannedImage(Mat &orig, vector<vector<Point>> &screenCnt, vector<int> px, vector<int> py, string input = "automatic");
		void Print();

};
