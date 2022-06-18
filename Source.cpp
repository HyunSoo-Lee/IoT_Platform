#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

// Solution #1 - by 20201514 LeeHyunSoo
// 1. Find proportion of GREEN(plant) in First IMG -> proportion 'default'
// 2. Set as DEFAULT
// 3. Find proportion of GREEN in Current IMG -> proportion 'current'
// 4. Compare 
// *Upload to git

Mat imgCVT(Mat img, Mat img_hsv, Mat mask);

int main() {
	//i) masking - show

	Mat unhealth = imread("unhealth.png");
	Mat unhealth_hsv, unhealth_mask;
	unhealth_mask = imgCVT(unhealth, unhealth_hsv, unhealth_mask);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(unhealth_mask, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
	cout << "contours size : " << contours.size() << endl;

	vector<vector<Point>> conPoly(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conPoly[i], 0.001 * peri, true);
		cout << " Area: " << contourArea(contours[i]) << endl;
		drawContours(unhealth, conPoly, i, Scalar(0, 0, 255), 3);
		imshow("result", unhealth);
	}	

	std::ofstream ofs("test.txt");
	if (ofs.fail())
	{
		std::cerr << "Error!" << std::endl;
		return -1;
	}
	ofs << contours.size() << endl;

	//imshow("health", health);
	imshow("mask", unhealth_mask);
	imshow("result", unhealth);
	waitKey(0);

	return 0;
}

Mat imgCVT(Mat img, Mat img_hsv, Mat mask) {
	cvtColor(img, img_hsv, COLOR_BGR2HSV);
	//Test at https://ko.rakko.tools/tools/30/
	//H : 85(60)~135(95)
	//S : 60   153~255
	//V : 70   178~255
	Scalar lower_green = Scalar(25, 0, 0);
	Scalar upper_green = Scalar(70, 255, 255);
	inRange(img_hsv, lower_green, upper_green, mask);
	GaussianBlur(mask, mask, Size(7, 7), 0);
	return mask;

}