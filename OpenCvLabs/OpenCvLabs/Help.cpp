#include <stdio.h>
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <ctime>

#include <iomanip>
#include <windows.h>

using namespace cv;
using namespace std;


int main() {

	Mat firstFrame, backgroundFrame, maskFrame, temp, threshFrame;
	VideoCapture cap;

	cap.open("C:\\Users\\aosowska\\source\\repos\\OpenCvLabs\\OpenCvLabs\\pedestriancounting_cutted.avi");
	char checkKey = 0;

	int width = cap.get(CAP_PROP_FRAME_WIDTH), height = cap.get(CAP_PROP_FRAME_HEIGHT), margin = 30;
	int InX1 = margin, InX2 = InX1, InY1 = height, InY2 = 0;

	int stepIn = 0, stepOut = 0 , wasPresent = 0;

	int OutX1 = width - margin, OutX2 = OutX1;
	int OutY1 = InY1, OutY2 = 0;
	bool started = false;

	Ptr<BackgroundSubtractor> pMOG2;
	vector<vector<Point>> contours;

	pMOG2 = createBackgroundSubtractorMOG2(10, 16, false);

	while (checkKey != 27 && cap.isOpened()) {

		cap.read(firstFrame);

		if (firstFrame.empty()) {

			Mat statistics(height, width, CV_8UC3, Scalar(0,0,0));

			putText(statistics, "Statistics", Point(50, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 192, 203), 2);

			stringstream inInfo;
			inInfo << "Crossed 'In' line : " << stepIn << " should be : " << 9;
			putText(statistics, inInfo.str(), Point(50, 150), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(123, 192, 0), 1);

			stringstream outInfo;
			outInfo << "Crossed 'Out' line : " << stepOut << " should be : " << 3;
			putText(statistics, outInfo.str(), Point(50, 200), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(123, 192, 0), 1);


			stringstream homeLanders;
			homeLanders << "Was present before: " << wasPresent << " should be : " << 3;
			putText(statistics, homeLanders.str(), Point(50, 250), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(123, 192, 0), 1);

			imshow("Statistics", statistics);
		}
		else 
		{
			GaussianBlur(firstFrame, firstFrame, Size(7, 7), 0);
			pMOG2->apply(firstFrame, backgroundFrame);
			threshold(backgroundFrame, threshFrame, 10, 255, CV_THRESH_BINARY);

			Mat morphFrame = getStructuringElement(MORPH_RECT, Size(10, 10));
			morphologyEx(threshFrame, temp, MORPH_OPEN, morphFrame);
			morphologyEx(temp, maskFrame, MORPH_CLOSE, morphFrame);

        	findContours(maskFrame.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			vector<vector<Point>> contours_poly(contours.size());
			vector<Rect> boundRect(contours.size());

			for (int i = 0; i < contours.size(); i++) {
				Moments moment = moments(contours[i], true);
				int x = moment.m10 / moment.m00;
				int y = moment.m01 / moment.m00;

				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));

				rectangle(firstFrame, boundRect[i].tl(), boundRect[i].br(), Scalar(250, 0, 0), 2, 8, 0);

				if (!started)
					wasPresent = contours.size() - 1; //because something triggered

				if (abs(x - OutX1) < 1)
					started = true; 
				if (abs(x - InX1) < 1)
					started = true;

				if (abs(x - OutX1) < 5) {
					putText(firstFrame, "IN", Point(x, y), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
					stepIn++;
				}

				if (abs(x - InX1) < 5) {
					putText(firstFrame, "OUT", Point(x, y), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
					stepOut++;
				}	
			}

			stringstream general;
			general <<"Now : " << contours.size();

			putText(firstFrame, general.str(), Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(120, 255, 0), 3);

			line(firstFrame, Point(InX1, InY1), Point(InX2, InY2), Scalar(0, 10, 255), 2, CV_AA);
			line(firstFrame, Point(OutX1, OutY1), Point(OutX2, OutY2), Scalar(0, 10, 255), 2, CV_AA);

			stringstream statistics;
			statistics << "Out : " << stepOut << " In : " << stepIn << " Was : "  << wasPresent;

			putText(firstFrame, statistics.str(), Point(50, 50), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 192, 0), 2);
			imshow("Street", firstFrame);
		}
		checkKey = waitKey(15);
	}
	cap.release();
	destroyAllWindows();

	return 0;
}
