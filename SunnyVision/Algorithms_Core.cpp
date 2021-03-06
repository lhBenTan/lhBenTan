#include <ctime>
#include <comutil.h>
#include <iostream>
#include <opencv.hpp>
#include "putText.h"
#include "Math.h"
#include "StringEx.h"

using namespace cv;

#define PIXEL_BGR(frame, W, x, y) (frame+(y)*3*(W)+(x)*3)
#define PIXEL_GRAY(frame, W, x, y) (frame+(y)*(W)+(x))

/// 图像交换数据结构
struct ImageInfo
{
	unsigned char* data;
	unsigned char* pMat;
	int size;
};

/// 交换数据
struct SwapData
{
	double outTotalArea;
	double outCircularity;
	double outCircularity2;
	int inRoiX;
	int inRoiY;
	int inRoiW;
	int inRoiH;
	int isEffectDisplay;
};

/// 标准阈值数据
struct StdData
{
	double Threshold;
	double Area;
	double Circularit;
	double HistEvenness;
	double SFR;
};

/// Opencv wrapper - gray

double _stdcall CvpGray(unsigned char* pBuf, const int width, const int height, ImageInfo& imgInfo)
{
	TickMeter tm;
	tm.start();

	Mat src_gray(height, width, IMREAD_GRAYSCALE);
	Mat src(height + height / 2, width, CV_8UC1, pBuf);
	cvtColor(src, src_gray, CV_YUV2GRAY_YV12);

	std::vector<unsigned char> bytes;
	imencode(".bmp", src_gray, bytes);
	imgInfo.size = static_cast<int>(bytes.size());
	imgInfo.data = static_cast<unsigned char*>(calloc(imgInfo.size, sizeof(unsigned char)));
	std::copy(bytes.begin(), bytes.end(), imgInfo.data);

	tm.stop();
	return tm.getTimeMilli();
}

/// 求矩阵中指定灰度范围内的像素的个数
int CalcMatPixelNum(Mat im_gray, int min, int max)
{
	int sum = 0;
	Mat im_temp;
	threshold(im_gray, im_temp, min, max, THRESH_BINARY);
	sum = cv::countNonZero(im_temp);

	return sum;
}

/// 截图
BSTR _stdcall CaptureBmp(unsigned char* pBuf, const int width, const int height)
{
	// 读取YV12
	//Mat src_color(height, width, IMREAD_ANYCOLOR);
	Mat src(height * 3 / 2, width, CV_8UC1, pBuf);	// 高度乘以3/2，因为存在4个Y样本，并且每2x2平方像素存储1 U和1 V样本。这导致字节样本与像素的比率为3/2
	//cvtColor(src, src_color, COLOR_YUV2BGR_YV12);
	Mat src_gray(height, width, CV_8UC1, pBuf);

	auto t = std::time(nullptr);
	std::tm tm = {};
	localtime_s(&tm, &t);
	// std::put_time(&tm, "%Y%m%d%H%M%S");

	char path[80];
	strftime(path, 80, ".//CapturePicArea//%Y%m%d%H%M%S.bmp", &tm);
	imwrite(path, src_gray);

	return SysAllocString(ANSItoBSTR(path));
}

/// ROI定位函数
Point ROIPositioning(Mat src, int x_origin, int y_origin)
{
	int thresh_value = 240;

	Mat im_roi = src(Rect(x_origin, y_origin, 400, 400));
	// cvtColor(im_roi, im_roi, COLOR_BGR2GRAY);
	Mat im_roi_thresh;
	threshold(im_roi, im_roi_thresh, thresh_value, 255, THRESH_BINARY);

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierachy;
	findContours(im_roi_thresh, contours, hierachy, /*RETR_TREE*/RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	int x_center_min = 0;
	int y_center_min = 0;
	double min_distance = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double areaForContour = contourArea(contours[i], false);
		if (areaForContour > 100)
		{
			Rect min_bounding_rect = boundingRect(contours[i]);
			//rectangle(im_output, min_bounding_rect, Scalar(255, 255, 0));
			int x_center = static_cast<int>(min_bounding_rect.x + min_bounding_rect.width / 2);
			int y_center = static_cast<int>(min_bounding_rect.y + min_bounding_rect.height / 2);
			double distance = pow(x_center, 2) + pow(y_center, 2);

			if (min_distance == 0)
			{
				min_distance = distance;
			}
			if (distance <= min_distance)
			{
				x_center_min = x_center;
				y_center_min = y_center;
				min_distance = distance;
			}
		}
	}

	Point point;
	point.x = x_origin + x_center_min - 184;
	point.y = y_origin + y_center_min - 246;

	return point;
}

/// 杂光验证算法
double _stdcall Algorithm_GetContourArea(unsigned char* pBuf, const int width, const int height, ImageInfo& imgInfo, SwapData& swapData, StdData& stdData)
{
	TickMeter tm;
	tm.start();

	// 读取YV12，转灰度矩阵
	//Mat src_gray(height, width, IMREAD_GRAYSCALE);
	//Mat src(height * 3 / 2, width, CV_8UC1, pBuf);	// 高度乘以3/2，因为存在4个Y样本，并且每2x2平方像素存储1 U和1 V样本。这导致字节样本与像素的比率为3/2
	//cvtColor(src, src_gray, COLOR_YUV2GRAY_YV12);

	Mat src_gray(height, width, CV_8UC1, pBuf);

	// 参数设置
	std::vector<std::vector<Point>> contours;	// 轮廓点集合
	std::vector<Vec4i> hierachies;				// 轮廓层级结构
	double TotalArea = 0;						// 外接圆总面积
	float Circularity = 0;						// 轮廓圆度
	float Circularity2 = 0;						// 光源内亮线情况轮廓圆度

	//// 建立ROI
	//Point roi_point = ROIPositioning(src_gray, swapData.inRoiX, swapData.inRoiY);
	//roi_point.x = roi_point.y = 0;
	//// 定位不了轮廓的情况，直接输出图像
	//if (roi_point.x < 0)
	//{
	//	swapData.outTotalArea = 7198;
	//	swapData.outCircularity = 0;
	//	swapData.outCircularity2 = 0;
	//	std::vector<unsigned char> bytes;
	//	std::vector<int>param = std::vector<int>(2);
	//	imencode(".bmp", src_gray, bytes);
	//	imgInfo.size = static_cast<int>(bytes.size() - 54);
	//	// imgInfo.data = static_cast<unsigned char*>(calloc(imgInfo.size, sizeof(unsigned char)));
	//	imgInfo.data = new unsigned char[imgInfo.size];
	//	std::copy(bytes.begin() + 54, bytes.end(), imgInfo.data);

	//	tm.stop();
	//	return tm.getTimeMilli();
	//}
	Mat im_gray_roi = src_gray(Rect(swapData.inRoiX, swapData.inRoiY, swapData.inRoiW, swapData.inRoiH));

	// 建立彩色矩阵
	Mat im_output;
	/*imshow("im_gray_roi", im_gray_roi);
	waitKey(2);*/
	cvtColor(im_gray_roi, im_output, CV_GRAY2BGR);

	// 排除超级发胖泛白的情况
	if (CalcMatPixelNum(im_gray_roi, static_cast<int>(stdData.Threshold), 255) > 700000)
	{
		swapData.outTotalArea = 7197;
		swapData.outCircularity = 0;
		std::vector<unsigned char> bytes;
		std::vector<int>param = std::vector<int>(2);
		imencode(".bmp", im_output, bytes);
		imgInfo.size = static_cast<int>(bytes.size() - 54);
		// imgInfo.data = static_cast<unsigned char*>(calloc(imgInfo.size, sizeof(unsigned char)));
		imgInfo.data = new unsigned char[imgInfo.size];
		std::copy(bytes.begin() + 54, bytes.end(), imgInfo.data);

		tm.stop();
		return tm.getTimeMilli();
	}

	// 求取二值化矩阵
	Mat im_thresh;
	threshold(im_gray_roi, im_thresh, stdData.Threshold, 255, THRESH_BINARY);

	// 寻找轮廓点
	findContours(im_thresh, contours, hierachies, RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	// 最小外接矩形，用于圆度灰度算法
	Mat roi_minRect;

	// 计算每一个轮廓最小外接圆的面积
	if (0 < contours.size())
	{
		int i = 0;
		for (; i >= 0; i = hierachies[i][0])
		{
			Point2f center;	// 圆心
			float radius;	// 半径

			// 求最小外接圆
			minEnclosingCircle(contours[i], center, radius);

			// 筛选
			double temp2 = radius * radius * 3.14;
			if (temp2 > 2000)
			{
				// 判断外接圆是否越界，获取最小矩阵ROI
				bool isCross = false;
				if ((center.x - radius) > 0 
					&& (center.y - radius) > 0 
					&& (center.x + radius) < im_gray_roi.cols 
					&& (center.y + radius) < im_gray_roi.rows)
				{
					im_gray_roi(Rect(center.x - radius, center.y - radius, 2 * radius, 2 * radius)).copyTo(roi_minRect);
				}
				else
				{
					// 越界了，不进行圆周灰度算法
					isCross = true;
				}

				TotalArea += temp2;
				// 绘图
				if (swapData.isEffectDisplay > 0)
				{
					drawContours(im_output, contours, i, Scalar(255, 0, 0), 2, LINE_8, hierachies, 0, Point(0, 0));
					circle(im_output, center, static_cast<int>(radius), Scalar(0, 255, 255), 2);
				}

				// 求轮廓圆度
				float temp1 = calculateCircularity(contours[i], center, radius);
				Circularity = temp1 > Circularity ? temp1 : Circularity;

				// 内部亮线使用圆度灰度算法处理
				if (Circularity < stdData.Circularit && !isCross)
				{
					int i = 0;
					bool a = false;
					bool b = false;
					std::vector<Point> v1;
					for (uchar v : (std::vector<uchar>)roi_minRect.reshape(1, 1))
					{
						// 以220为阈值去掉中心光源
						if (v > 220)
						{
							roi_minRect.at<uchar>(i) = 0;
							b = true;
						}
						else
						{
							b = false;
						}

						// a和b用来记录轮廓点坐标
						if (a != b)
						{
							// 记录点
							int x = i < roi_minRect.cols ? i : i % roi_minRect.cols;
							int y = i < roi_minRect.cols ? 0 : i / roi_minRect.cols;
							v1.push_back(Point(x, y));
							a = b;
						}

						i++;
					}

					// 拟合椭圆
					auto rRect = fitEllipse(v1);
					// 获取标准圆圆心与半径
					Point c = rRect.center;
					int r = static_cast<int>(rRect.size.height / 2) * 2;

					// 拟合的圆也不能超过边界
					if ((c.x - r) > 0
						&& (c.y - r) > 0
						&& (c.x + r) < roi_minRect.cols
						&& (c.y + r) < roi_minRect.rows)
					{
						// 获取圆周灰度值集合
						std::vector<int> v2;
						for (size_t angle = 0; angle < 180; angle += 2)
						{
							int x = static_cast<int>(c.x + r * sin(angle * CV_PI / 180));
							int y = static_cast<int>(c.y + r * cos(angle * CV_PI / 180));

							v2.push_back(PIXEL_GRAY(roi_minRect.data, roi_minRect.cols, x, y)[0]);
						}

						// 二值化处理
						double thresh = (double)(*std::max_element(v2.begin(), v2.end()));
						Mat roi_minRect_thresh;
						threshold(roi_minRect, roi_minRect_thresh, thresh, 255, THRESH_BINARY);

						// 找轮廓
						std::vector<std::vector<Point>> contours;	// 轮廓点集合
						findContours(roi_minRect_thresh, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

						// 筛选最大轮廓
						std::vector<Point> max_contour;
						if (contours.size() > 0)
						{
							max_contour = *std::max_element(
								contours.begin(),
								contours.end(),
								[](std::vector<cv::Point> const& lhs, std::vector<cv::Point> const& rhs) {
								return contourArea(lhs, false) < contourArea(rhs, false);
							});

							// 求最小外接圆
							Point2f center;
							float radius;
							minEnclosingCircle(max_contour, center, radius);

							// 求轮廓圆度
							float circularity = calculateCircularity(max_contour, center, radius);
							// 只记录最大轮廓
							Circularity2 = circularity > Circularity2 ? circularity : Circularity2;
						}
					}
				}
			}
		}

		/*putTextZH(im_output, format("阈值：%f", threshold_value).c_str(), Point(10, 5), Scalar(255, 128, 0), 56, "黑体");
		putTextZH(im_output, format("轮廓面积：%.2f", TotalArea).c_str(), Point(10, 70), Scalar(255, 128, 0), 56, "黑体");
		putTextZH(im_output, format("最大圆度：%.2f", Circularity).c_str(), Point(10, 135), Scalar(255, 128, 0), 56, "黑体");*/
		swapData.outTotalArea = TotalArea;
		swapData.outCircularity = Circularity;
		swapData.outCircularity2 = Circularity2;
	}

	std::vector<unsigned char> bytes;
	std::vector<int>param = std::vector<int>(2);
	imencode(".bmp", im_output, bytes);
	imgInfo.size = static_cast<int>(bytes.size() - 54);
	// imgInfo.data = static_cast<unsigned char*>(calloc(imgInfo.size, sizeof(unsigned char)));
	imgInfo.data = new unsigned char[imgInfo.size];
	std::copy(bytes.begin() + 54, bytes.end(), imgInfo.data);

	tm.stop();
	return tm.getTimeMilli();
}