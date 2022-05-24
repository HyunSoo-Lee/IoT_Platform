#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

// Solution #1 - by 20201514 LeeHyunSoo
// 1. Find proportion of GREEN(plant) in First IMG -> proportion 'default'
// 2. Set as DEFAULT
// 3. Find proportion of GREEN in Current IMG -> proportion 'current'
// 4. Compare 

Mat imgCVT(Mat img, Mat img_hsv, Mat mask);

int main() {

	//i) masking - show
	Mat health = imread("grape_health.png"); 
	Mat health_hsv, health_mask;
	health_mask = imgCVT(health, health_hsv, health_mask);

	Mat unhealth = imread("grape_unhealth.png");
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
		drawContours(unhealth, conPoly, i, Scalar(0, 0, 255), 3);
	}

	cout << "conPoly size : " << conPoly.size() << endl;

	//imshow("health", health);
	//imshow("mask", health_mask);
	imshow("unhealth", unhealth);
	imshow("un_mask", unhealth_mask);
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
	Scalar lower_green = Scalar(60, 153, 178);
	Scalar upper_green = Scalar(95, 255, 255);
	inRange(img_hsv, lower_green, upper_green, mask);
	GaussianBlur(mask, mask, Size(7, 7), 0);
	return mask;
}