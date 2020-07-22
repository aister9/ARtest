#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main() {
	Mat frame = imread("test2.jpg");
	Mat frame2;
	GaussianBlur(frame, frame, Size(3,3), 0);
	cvtColor(frame, frame2, COLOR_BGR2YCrCb);
	
	Mat frames[3];
	
	split(frame2, frames);


	hconcat(frames[0], frames[1], frames[0]);
	hconcat(frames[0], frames[2], frames[0]);

	//imshow("test", frame);
	for (int i = 0; i < frame2.cols; i++) {
		for (int j = 0; j < frame2.rows; j++) {
			uchar cb = frame2.at<Vec3b>(j, i)[1];
			uchar cr = frame2.at<Vec3b>(j, i)[2];

			if (cb >= 128 && cb <= 170 && cr >= 73 && cr <= 158) {
				frame2.at<Vec3b>(j, i) = Vec3b(255, 255, 255);
			}
			else {
				frame2.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
			}
		}
	}
	Mat dist;
	cvtColor(frame2, dist, COLOR_BGR2GRAY);
	


	distanceTransform(dist, dist, DIST_C, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);

	double min, max;
	Point minLoc, maxLoc;
	minMaxLoc(dist, &min, &max, &minLoc, &maxLoc);

	Mat cannyed;
	Canny(frame2, cannyed, 70, 125);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cannyed, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	circle(frame2, maxLoc, 7, (175, 175, 0));

	cout << minLoc << endl << maxLoc << endl;

	//for (int i = 0; i < frame2.cols; i++) {
	//	for (int j = 0; j < frame2.rows; j++) {
	//		//cout << dist.at<float>(j, i)<<" ";
	//		if (cannyed.at<uchar>(j, i) == 255) {
	//			circle(frame2, Point(i, j), 7, (0, 0, 255));
	//		}
	//	}
	//}

	vector<Point> set;
	vector<Point> set2;

	for (int i = 0; i < contours.size(); i++) {
		for (int j = 1; j < contours[i].size()-1; j++) {
			Point previous = contours[i][j-1];
			Point here = contours[i][j];
			Point next = contours[i][j+1];

			Point v1 = here - previous;
			Point v2 = here - next;

			double k = v1.dot(v2) / (sqrt(v1.x*v1.x+v1.y*v1.y)* sqrt(v2.x * v2.x + v2.y * v2.y));

			if (k > -0.01 && k < 0.01) set.push_back(here);
		}
	}


	//hconcat(frame, frame2, frame);
	cout << set.size();
	/*for (int j = 1; j < set.size() - 1; j++) {
		Point previous = set[j - 1];
		Point here = set[j];
		Point next = set[j + 1];

		Point v1 = here - previous;
		Point v2 = here - next;

		double k = v1.dot(v2) / (sqrt(v1.x * v1.x + v1.y * v1.y) * sqrt(v2.x * v2.x + v2.y * v2.y));

		if (k > -0.3 && k < 0.3) set2.push_back(here);
	}
	cout << " "<<set2.size();*/

	int is = 0;
	while (waitKey(27) != 27) {

		imshow("Y", frame2);
		if(is<set.size())circle(frame2, set[is++], 7, (0, 0, 255));
	}

	system("pause");

	//VideoCapture source("test.mp4");

	//while (true) {
	//	source >> frame;
	//	if (frame.empty()) break;
	//	cvtColor(frame, frame, COLOR_BGR2YCrCb);
	//	/*for (int i = 0; i < frame.cols; i++) {
	//		for (int j = 0; j < frame.rows; j++) {
	//			uchar cb = frame.at<Vec3b>(j, i)[1];
	//			uchar cr = frame.at<Vec3b>(j, i)[2];

	//			if (cb >= 128 && cb <= 170 && cr >= 73 && cr <= 158) {
	//					frame.at<Vec3b>(j, i) = Vec3b(255, 255, 255);
	//			}
	//			else {
	//				frame.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
	//			}
	//		}
	//	}*/
	//	imshow("test", frame);
	//	waitKey(27);
	//}


	//system("pause");
}