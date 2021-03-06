#pragma once
#ifndef MATH_H_
#define MATH_H_

#include <opencv.hpp>
#include <iostream>
using namespace cv;

// 计算轮廓的圆的特性
float calculateCircularity(std::vector<Point> contour, Point2f center, float radius)
{
	// 以最小外接圆半径作为数学期望，计算轮廓上各点到圆心距离的标准差
	float fsum = 0;
	float fcompare = 0;
	for (size_t i = 0; i < contour.size(); i++) {
		Point2f ptmp = contour[i];
		float fdistenct = sqrt((float)((ptmp.x - center.x)*(ptmp.x - center.x) + (ptmp.y - center.y)*(ptmp.y - center.y)));
		float fdiff = abs(fdistenct - radius);
		fsum = fsum + fdiff;
	}
	fcompare = fsum / (float)contour.size();
	return fcompare;
}

#endif