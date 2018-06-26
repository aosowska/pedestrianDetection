#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>
#include<ctime>

#include<iostream>

using namespace cv;
using namespace std;

class Help
{
public:
	Help();
	~Help();
	Scalar getHsvMin(char color);
	Scalar getHsvMax(char color);
	void setX(int parX);
	void setY(int parY);
	int getX();
	int getY();

private:
	int X;
	int Y;
};
