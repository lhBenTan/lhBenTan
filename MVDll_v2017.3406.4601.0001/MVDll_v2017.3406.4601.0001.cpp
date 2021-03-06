#include <windows.h>
#include "opencv.hpp"
#include <math.h>
#include <vector>
#include <iostream>
#include <ctime>
#include "stdafx.h"
#include "putText.h"
#include "MV.20191210.V010.h"


//计时器
clock_t start, finish;

struct BmpBuf
{
	unsigned char* data_Output;
	int size;
	unsigned char* data_Input;
	int Height;
	int Width;

};

struct Pare_Output
{
	int output_Parameter_Int_0;
	int output_Parameter_Int_1;
	int output_Parameter_Int_2;
	int output_Parameter_Int_3;

	float output_Parameter_float_0;
	float output_Parameter_float_1;
	float output_Parameter_float_2;
	float output_Parameter_float_3;

	unsigned char* output_Parameter_Int;

};

struct Redate
{
	bool isGray = true;
	int threshold = 0;
	int maxRadiius = 0;
	int minRadiius = 0;
	int defaultAngle = 0;
	int radius = 0;
	int radiusThr = 0;

	float delta = 0;
	float angle = 0;
	float deltaX = 0;
	float deltaY = 0;

	int isMatched = 0;
};

struct PJ_Para
{
	float isGray = 0;
	float B1 = 0;
	float G1 = 0;
	float R1 = 0;
	float Gain1 = 0;
	float threshold0 = 0;
	float radius1 = 0;
	float radius2 = 0;
	float B2 = 0;
	float G2 = 0;
	float R2 = 0;
	float Gain2 = 0;
	float threshold1 = 0;
	float thLen0 = 0;
	float thLen1 = 0;
	float radius3 = 0;
	float radius4 = 0;
	float radius5 = 0;
	float radius6 = 0;
	float radius7 = 0;
	float radius8 = 0;
	float radius9 = 0;
	float num = 0;
	float len = 0;
	int JTNum = 0;
};

vector<string> Spilt(char* str)
{
	vector<string> v;
	istringstream iss(str);
	string token;
	while (getline(iss, token, '&')) { v.push_back(token); }
	return v;
}

vector<string> Spilt2(char* str)
{
	vector<string> v;
	istringstream iss(str);
	string token;
	while (getline(iss, token, '-')) { v.push_back(token); }
	return v;
}

void  MV_Release(BmpBuf &data)
{
	//Mat test = imread("d:\\zsliujunj\\桌面\\123.jpg");
	//imshow("123", test);
	//waitKey(2);
	delete data.data_Output;
	data.data_Output = NULL;

	data.size = 0;
	//free(data.data_Output);
	//data.data_Output = nullptr;

	//delete[] data.data_Output;
	//data.data_Output = nullptr;

}

bool img(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter)
{

	stringstream ss;
	int test = atoi(input_Parameter[1]);
	cout << test << endl;
	Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);
	Mat img = img_Temp.clone();
	Mat Color;
	cvtColor(img, Color, CV_GRAY2BGR);
	ss << "测试"  << endl;
	putTextZH(Color, ss.str().c_str(), Point(0, 0 + 20), Scalar(0, 255, 0), 50, "黑体", 0, 0);
	
	data.Height = Color.rows;
	data.Width = Color.cols;
	std::vector<uchar>buf;
	cv::imencode(".bmp", Color, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	return 0;

}

//bool MV_EntryPoint(int type, int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float);

//bool MV_GW01_JT(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//{
//	return MV_EntryPoint(0, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
//	 
//#pragma region 载入图像
//	//载入图像
//	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
//	//MV_Release(data);
//	//Mat img = imread("C:\\1.bmp", 0);
//#pragma endregion
//
//#pragma region 参数载入
//	Redate Para;
//
//	int MaxL = -1;
//	long area = -1;
//	TickMeter tm;
//	stringstream ss;
//	Mat gray, output;
//	CONTOURS contours;
//	vector<Vec4i> hierarchy;
//
//	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
//	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
//
//	//ss << "镜筒检测" << endl;
//
//	gray = img.clone();
//	Mat img_decode;
//
//	tm.start();
//
//
//	int result = 1;//1是ng 0是ok
//
//	float size = input_Parameter_Int[0];//扫描点数
//
//	bool isGray = input_Parameter_Int[1];//是否灰度显示
//
//	int threshold0 = input_Parameter_Int[2];//二值化阈值
//
//	int Radius1 = input_Parameter_Int[3];//轮廓外径
//	int Radius2 = input_Parameter_Int[4];//轮廓内径
//
//	int Radius3 = input_Parameter_Int[5];//角度-计算轮廓-外径
//	int Radius4 = input_Parameter_Int[6];//角度-计算轮廓-内径
//	int threshold1 = input_Parameter_Int[7];//角度-灰度阈值
//	float GPRate0 = input_Parameter_Int[8];//角度-有效点占比 0~1
//
//	float Radius5 = input_Parameter_Int[9];//缺陷深度下限
//	float Radius6 = input_Parameter_Int[10];//缺陷深度上限
//	float GPRate1 = input_Parameter_Int[11];//合格点比例
//
//	int Radius7 = input_Parameter_Int[12];//匹配轮廓-外径
//	int Radius8 = input_Parameter_Int[13];//匹配轮廓-内径
//	int threshold2 = input_Parameter_Int[14];//匹配-灰度阈值
//	float GPRate2 = input_Parameter_Int[15];//匹配-有效点占比
//	float GPRate3 = input_Parameter_Int[16];//匹配-有效角度占比
//	float BzCenterX = input_Parameter_Int[17];//画面补正中心X坐标
//	float BzCenterY = input_Parameter_Int[18];//画面补正中心Y坐标
//
//	int BzAngle = input_Parameter_Int[19];//补正角度
//	int isCap = input_Parameter_Int[20];//是否保存截图
//
//	float rule_X = input_Parameter_Int[21];//比例尺
//	float rule_Y = input_Parameter_Int[22];//比例尺
//#pragma endregion
//
//#pragma region 原图截图
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\JT\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), img);
//	}
//
//	tm.stop();
//	//ss << "1" <<tm.getTimeMilli()<< endl;
//	tm.start();
//#pragma endregion
//
//#pragma region 图像预处理
//
//	cvtColor(gray, img_decode, CV_GRAY2BGR);
//	Mat gray_Temp = gray.clone();
//	blur(gray_Temp, gray, Size(5, 5));
//	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
//	//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1));
//	//threshold(gray, gray, threshold0 + 70, 255, THRESH_TOZERO_INV);//二值化阈值
//	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
//	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//
//	tm.stop();
//	//ss << "1" << tm.getTimeMilli() << endl;
//	tm.start();
//#pragma endregion
//	/////////////////////////////////////////////
//#pragma region 筛选特定大小的轮廓
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		vector<Point2i> tmpCont;
//		convexHull(contours[i], tmpCont);
//		double tmpArea = arcLength(tmpCont, 0);
//		if (tmpArea > 2 * CV_PI*Radius1 || tmpArea < 2 * CV_PI*Radius2)continue;
//		if (tmpArea > area)
//		{
//			//double Area = contourArea(tmpCont);
//			//检测圆形是否规则
//			//if (Area < Radius2*Radius2*CV_PI || Area > Radius1*Radius1*CV_PI)continue;
//			area = (long)tmpArea;
//			MaxL = i;
//		}
//	}
//	//tm.stop();
//	//ss << "2" << tm.getTimeMilli() << endl;
//	//tm.start();
//#pragma endregion
//
//#pragma region 判定是否二值化显示
//	if (isGray)
//	{
//		cvtColor(gray, output, CV_GRAY2BGR);
//	}
//	else
//		output = img_decode.clone();
//	//tm.stop();
//	//ss << "3" << tm.getTimeMilli() << endl;
//	//tm.start();
//#pragma endregion
//
//	for (int i = 0; i < 1; i++)
//	{
//
//#pragma region 检测轮廓是否有效
//		if (MaxL<0 || 6>contours[MaxL].size())
//		{
//			ss << "无有效目标" << endl;
//			break;
//		}
//		//tm.stop();
//		//ss << "4" << tm.getTimeMilli() << endl;
//		//tm.start();
//#pragma endregion
//
//#pragma region 检测圆度
//		drawContours(output, contours, MaxL, Scalar(255, 0, 0));
//		//tm.stop();
//		//ss << "5" << tm.getTimeMilli() << endl;
//		//tm.start();
//#pragma endregion
//
//#pragma region 获取圆心及半径
//		Point2f center(0, 0);
//		float radius;
//		vector<Point2i> tmpCont;
//		convexHull(contours[MaxL], tmpCont);
//		RotatedRect ell = fitEllipse(tmpCont);
//		ellipse(output, ell, Scalar(255, 0, 0), 2);
//		center = ell.center;
//		Para.deltaX = center.x - BzCenterX;
//		Para.deltaY = center.y - BzCenterY;
//
//		radius = ell.size.width / 2;
//		ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
//		ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
//		ss << "半径：" << radius << endl;
//		//tm.stop();
//		//ss << "6" << tm.getTimeMilli() << endl;
//		//tm.start();
//
//		//circle(output, center, Radius1, Scalar(0, 0, 255), 2);
//		//circle(output, center, Radius2, Scalar(0, 0, 255), 2);
//		//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
//#pragma endregion
//
//#pragma region 利用圆周灰度获取部品角度
//		vector<bool> GoodPoint;
//		//这里的Para1跟Para1是设置值
//		cvtColor(img_decode, gray, CV_BGR2GRAY);
//		Ana_DefectDetection_2(gray,
//			GoodPoint,
//			center,
//			Radius3,
//			Radius4,
//			threshold1,
//			size,
//			(Radius3 - Radius4) *GPRate0);
//
//		if (0 >= GoodPoint.size())
//		{
//			ss << "轮廓过小" << endl;
//			break;
//		}
//
//		//circle(output, center, Radius3, Scalar(0, 255, 0), 2);
//		//circle(output, center, Radius4, Scalar(0, 255, 0), 2);
//
//		//for (int i = 0; i < size; i++)
//		//{
//		//	float s = sin(i * 360 / size * CV_PI / 180);
//		//	float c = cos(i * 360 / size * CV_PI / 180);
//		//	Point2f start = center + para2 * Point2f(s, -c);
//		//	Point2f end = center + para1 * Point2f(s, -c);
//		//	if (GoodPoint[i])
//		//	{
//		//		line(output, start, end, Scalar(255, 255, 0), 1);
//		//	}
//		//	else
//		//	{
//		//		line(output, start, end, Scalar(0, 0, 255), 1);
//		//	}
//		//}
//
//		Vec2i ang = Ana_FindMaxLen(GoodPoint);
//		//ang[0] = (ang[0] + (int)size) % (int)size;
//		Para.angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
//		Para.angle = ((int)Para.angle + 360) % 360;
//
//		ss << "角度：" << Para.angle << endl;
//
//		line(output,
//			center,
//			Point(center.x + ell.size.width / 2 * sin(Para.angle * CV_PI / 180.0),
//				center.y - ell.size.width / 2 * cos(Para.angle * CV_PI / 180.0)),
//			Scalar(255, 0, 0),
//			4);
//		//tm.stop();
//		//ss << "7" << tm.getTimeMilli() << endl;
//		//tm.start();
//#pragma endregion
//
//#pragma region 遍历轮廓，获取部品方向(异常 待修改)
//		//GoodPoint.clear();
//		//float para[2] = { Radius5,Radius6 };
//		//ss << "lunkuoyusndu:" << contours[MaxL].size() << endl;
//		//Ana_DefectDetection_1(contours[MaxL], para, GoodPoint, center);
//
//		//for (size_t i = 0; i < GoodPoint.size(); i++)
//		//{
//		//	if (GoodPoint[i])
//		//		circle(output, contours[MaxL][i], 1, Scalar(0, 255, 0), 2);
//		//	else
//		//		circle(output, contours[MaxL][i], 1, Scalar(0, 0, 255), 2);
//		//}
//
//		//Vec2i len = Ana_FindMaxLen(GoodPoint);
//		//Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
//		//double ang2 = Ana_ComputeAngle(center, end);
//		//line(output, center, end, Scalar(0, 255, 0), 1);
//
//		//float sum = 0;
//		//for (size_t i = 0; i < GoodPoint.size(); i++)
//		//{
//		//	sum += GoodPoint[i];
//		//}
//		//sum /= GoodPoint.size();
//		//ss << "轮廓类圆率：" << sum << endl;
//
//		//if (sum < GPRate1)
//		//{
//		//	ss << "部品外轮廓可能存在脏污" << endl;
//		//	break;
//		//}
//		//tm.stop();
//		//ss << "8" << tm.getTimeMilli() << endl;
//		//tm.start();
//		//ss << "轮廓距离角度:" << ang2 << endl;
//#pragma endregion
//
//#pragma region 利用圆周灰度进行面别匹配
//		GoodPoint.clear();
//
//		Ana_DefectDetection_2(gray,
//			GoodPoint,
//			center,
//			Radius7,
//			Radius8,
//			threshold2,
//			size,
//			(Radius7 - Radius8)*GPRate2);
//
//		//circle(output, center, Radius7, Scalar(255, 0, 0), 2);
//		//circle(output, center, Radius8, Scalar(255, 0, 0), 2);
//
//		for (int i = 0; i < size; i++)
//		{
//			float s = sin(i * 360 / size * CV_PI / 180);
//			float c = cos(i * 360 / size * CV_PI / 180);
//			//Point2f start = center + Radius8 * Point2f(s, -c);
//			Point2f end = center + (Radius7 + Radius8) * Point2f(s, -c) / 2;
//			if (GoodPoint[i])//BGR
//			{
//				circle(output, end, 1, Scalar(0, 255, 0), 2);
//				//line(output, start, end, Scalar(0, 255, 0), 1.5);
//			}
//			else
//			{
//				circle(output, end, 1, Scalar(0, 0, 255), 2);
//				//line(output, start, end, Scalar(0, 0, 255), 1.5);
//			}
//		}
//
//		////////////////////
//		Para.delta = 0;
//		for (size_t i = 0; i < GoodPoint.size(); i++)
//		{
//			Para.delta += GoodPoint[i];
//		}
//		Para.delta /= GoodPoint.size();
//		ss << "匹配度:" << Para.delta << endl;
//
//		if (Para.delta >= GPRate3)
//		{
//			Para.isMatched = 0;
//		}
//		else
//		{
//			Para.isMatched = 1;
//			break;
//		}
//
//		//tm.stop();
//		//ss << "9" << tm.getTimeMilli() << endl;
//		//tm.start();
//#pragma endregion
//		result = 0;
//		ss << "结果：" << result << endl;
//		ss << "镜筒判定通过" << endl;
//	}
//
//
//	//ss << "偏差X = " << Para.deltaX << endl;
//	//ss << "偏差Y = " << Para.deltaY << endl;
//	if (isGray)
//	{
//		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//	else
//	{
//		stringstream ss2;
//
//		ss2 << "结果:" << result << endl;
//		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//	/////////////////////////////////////////////
//#pragma region 结果图像输出
//	tm.stop();
//#pragma endregion
//
//	//字体大小
//	int text_Size;
//	text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
//	//位置
//	Point text_Localtion01;
//	text_Localtion01.x = text_Size / 3;
//	text_Localtion01.y = text_Size / 3;
//	Point text_Localtion02;
//	text_Localtion02.x = text_Size / 3;
//	text_Localtion02.y = nHeight - text_Size * 4;
//	Point text_Localtion03;
//	text_Localtion03.x = text_Size / 3;
//	text_Localtion03.y = nHeight - text_Size * 3;
//
//
//	//return tm.getTimeMilli();
//
//	//stringstream ss;
//	/*ss << "时间 = " << tm.getTimeMilli() << endl;
//	if (result == 0)
//	{
//		ss << "结果 = OK"  << endl;
//
//	}
//	else { ss << "结果 = NG" << endl; }
//	ss << "偏差X = " << Para.deltaX << endl;
//	ss << "偏差Y = " << Para.deltaY << endl;
//	ss << "角度 = " << Para.angle << endl;
//	ss << "二值化阈值："<< Para.threshold<< endl;
//
//	std::string test = ss.str();
//	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体");*/
//
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\JTOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), output);
//	}
//
//	data.Height = output.rows;
//	data.Width = output.cols;
//	//输出
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = static_cast<int>(buf.size() - 54);
//	data.data_Output = new unsigned char[data.size];
//	std::copy(buf.begin() + 54
//		, buf.end(), data.data_Output);
//
//	//Output.output_Parameter_Float[1] = distance;
//	*output_Parameter_Int = tm.getTimeMilli();
//	output_Parameter_Int++;
//
//	*output_Parameter_Float = Para.angle + BzAngle;
//	output_Parameter_Float++;
//
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	//中心差值
//	*output_Parameter_Float = Para.deltaX * rule_X;
//	output_Parameter_Float++;
//	*output_Parameter_Float = Para.deltaY * rule_Y;
//	output_Parameter_Float++;
//	//镜筒结果
//	*output_Parameter_Float = result;
//	output_Parameter_Float++;
//	return 0;
//
//}
//
//bool MV_GW01_Lens(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//{
//	return MV_EntryPoint(1, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
//#pragma region 载入图像
//
//	//载入图像
//	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
//	//MV_Release(data);
//	//Mat img = imread("C:\\Image_20200701214915979.bmp", 0);
//	Redate Para;
//
//	int MaxL = -1;
//	long area = -1;
//	TickMeter tm;
//	stringstream ss;
//	Mat gray, output;
//	CONTOURS contours;
//	vector<Vec4i> hierarchy;
//
//	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
//	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
//
//	gray = img.clone();
//	Mat img_decode;
//
//	tm.start();
//
//
//	int result = 1;//1是ng 0是ok
//
//	float size = input_Parameter_Int[0];//扫描点数
//
//	bool isGray = input_Parameter_Int[1];//是否灰度显示
//
//	int threshold0 = input_Parameter_Int[2];//二值化阈值
//
//	int Radius1 = input_Parameter_Int[3];//轮廓外径
//	int Radius2 = input_Parameter_Int[4];//轮廓内径
//
//	int Radius3 = input_Parameter_Int[5];//角度-计算轮廓-外径
//	int Radius4 = input_Parameter_Int[6];//角度-计算轮廓-内径
//	int threshold1 = input_Parameter_Int[7];//角度-灰度阈值
//	float GPRate0 = input_Parameter_Int[8];//角度-有效点占比 0~1
//
//	float Radius5 = input_Parameter_Int[9];//缺陷深度下限
//	float Radius6 = input_Parameter_Int[10];//缺陷深度上限
//	float GPRate1 = input_Parameter_Int[11];//合格点比例
//
//	int Radius7 = input_Parameter_Int[12];//匹配轮廓-外径
//	int Radius8 = input_Parameter_Int[13];//匹配轮廓-内径
//	int threshold2 = input_Parameter_Int[14];//匹配-灰度阈值
//	float GPRate2 = input_Parameter_Int[15];//匹配-有效点占比
//	float GPRate3 = input_Parameter_Int[16];//匹配-有效角度占比
//	float BzCenterX = input_Parameter_Int[17];//画面补正中心X坐标
//	float BzCenterY = input_Parameter_Int[18];//画面补正中心Y坐标
//
//	int BzAngle = input_Parameter_Int[19];//补正角度
//	int isCap = input_Parameter_Int[20];//是否保存截图
//
//	float rule_X = input_Parameter_Int[21];//比例尺
//	float rule_Y = input_Parameter_Int[22];//比例尺
//#pragma region 图像预处理
//
//	cvtColor(gray, img_decode, CV_GRAY2BGR);
//	Mat gray_Temp = gray.clone();
//	blur(gray_Temp, gray, Size(3, 3));
//	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
//	//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1));
//	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
//
//	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\JP\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), img);
//
//	}
//#pragma endregion
//	/////////////////////////////////////////////
//#pragma region 筛选特定大小的轮廓
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		vector<Point2i> tmpCont;
//		convexHull(contours[i], tmpCont);
//		double tmpArea = arcLength(tmpCont, 0);
//		if (tmpArea > 2 * CV_PI*Radius1 || tmpArea < 2 * CV_PI*Radius2)continue;
//		if (tmpArea > area)
//		{
//			double Area = contourArea(tmpCont);
//			//检测圆形是否规则
//			if (Area < Radius2*Radius2*CV_PI || Area > Radius1*Radius1*CV_PI)continue;
//			area = (long)tmpArea;
//			MaxL = i;
//		}
//	}
//#pragma endregion
//
//#pragma region 判定是否二值化显示
//	if (isGray)
//	{
//		cvtColor(gray, output, CV_GRAY2BGR);
//	}
//	else
//		output = img_decode.clone();
//#pragma endregion
//
//	for (int i = 0; i < 1; i++)
//	{
//#pragma region 检测轮廓是否有效
//		if (MaxL<0 || 6>contours[MaxL].size())
//		{
//			ss << "无有效目标" << endl;
//			break;
//		}
//#pragma endregion
//
//#pragma region 检测圆度
//		drawContours(output, contours, MaxL, Scalar(255, 0, 0));
//#pragma endregion
//
//#pragma region 获取圆心及半径
//		Point2f center(0, 0);
//		float radius;
//		vector<Point2i> tmpCont;
//		convexHull(contours[MaxL], tmpCont);
//		RotatedRect ell = fitEllipse(tmpCont);
//		ellipse(output, ell, Scalar(255, 0, 0), 2);
//		center = ell.center;
//		Para.deltaX = center.x - BzCenterX;
//		Para.deltaY = center.y - BzCenterY;
//
//		radius = ell.size.width / 2;
//		ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
//
//		ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
//
//		circle(output, center, Radius1, Scalar(0, 0, 255), 2);
//		circle(output, center, Radius2, Scalar(0, 255, 0), 2);
//		//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
//#pragma endregion
//
//#pragma region 利用圆周灰度获取部品角度
//		vector<bool> GoodPoint;
//		//这里的Para1跟Para1是设置值
//		cvtColor(img_decode, gray, CV_BGR2GRAY);
//		Ana_DefectDetection_2(gray,
//			GoodPoint,
//			center,
//			Radius3,
//			Radius4,
//			threshold1,
//			size,
//			(Radius3 - Radius4) *GPRate0);
//
//		if (0 >= GoodPoint.size())
//		{
//			ss << "轮廓过小" << endl;
//			break;
//		}
//
//		//for (int i = 0; i < size; i++)
//		//{
//		//	float s = sin(i * 360 / size * CV_PI / 180);
//		//	float c = cos(i * 360 / size * CV_PI / 180);
//		//	Point2f start = center + para2 * Point2f(s, -c);
//		//	Point2f end = center + para1 * Point2f(s, -c);
//		//	if (GoodPoint[i])
//		//	{
//		//		line(output, start, end, Scalar(255, 255, 0), 1);
//		//	}
//		//	else
//		//	{
//		//		line(output, start, end, Scalar(0, 0, 255), 1);
//		//	}
//		//}
//
//		Vec2i ang = Ana_FindMaxLen(GoodPoint);
//		//ang[0] = (ang[0] + (int)size) % (int)size;
//		Para.angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
//		Para.angle = ((int)Para.angle + 360) % 360;
//
//		ss << "角度：" << Para.angle << endl;
//
//		line(output,
//			center,
//			Point(center.x + ell.size.width / 2 * sin(Para.angle * CV_PI / 180.0),
//				center.y - ell.size.width / 2 * cos(Para.angle * CV_PI / 180.0)),
//			Scalar(255, 0, 0),
//			4);
//#pragma endregion
//
//#pragma region 遍历轮廓，获取部品方向
//		//GoodPoint.clear();
//		//float para[2] = { Radius5,Radius6 };
//		//Ana_DefectDetection_1(contours[MaxL], para, GoodPoint, center);
//
//		//for (size_t i = 0; i < GoodPoint.size(); i++)
//		//{
//		//	if (GoodPoint[i])
//		//		circle(output, contours[MaxL][i], 1, Scalar(0, 255, 0), 2);
//		//	else
//		//		circle(output, contours[MaxL][i], 1, Scalar(0, 0, 255), 2);
//		//}
//
//		//Vec2i len = Ana_FindMaxLen(GoodPoint);
//		//Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
//		//double ang2 = Ana_ComputeAngle(center, end);
//		//line(output, center, end, Scalar(0, 255, 0), 1);
//
//		//float sum = 0;
//		//for (size_t i = 0; i < GoodPoint.size(); i++)
//		//{
//		//	sum += GoodPoint[i];
//		//}
//		//sum /= GoodPoint.size();
//		//ss << "轮廓类圆率：" << sum << endl;
//
//		//if (sum < GPRate1)
//		//{
//		//	ss << "部品外轮廓可能存在脏污" << endl;
//		//	break;
//		//}
//
//		//ss << "轮廓距离角度:" << ang2 << endl;
//#pragma endregion
//
//#pragma region 利用圆周灰度进行面别匹配
//		GoodPoint.clear();
//
//		Ana_DefectDetection_2(gray,
//			GoodPoint,
//			center,
//			Radius7,
//			Radius8,
//			threshold2,
//			size,
//			(Radius7 - Radius8)*GPRate2);
//
//		for (int i = 0; i < size; i++)
//		{
//			float s = sin(i * 360 / size * CV_PI / 180);
//			float c = cos(i * 360 / size * CV_PI / 180);
//			Point2f end = center + Radius7 * Point2f(s, -c);
//			if (GoodPoint[i])//BGR
//			{
//				circle(output, end, 1, Scalar(0, 255, 0), 2);
//			}
//			else
//			{
//				circle(output, end, 1, Scalar(0, 0, 255), 2);
//			}
//		}
//
//		////////////////////
//		Para.delta = 0;
//		for (size_t i = 0; i < GoodPoint.size(); i++)
//		{
//			Para.delta += GoodPoint[i];
//		}
//		Para.delta /= GoodPoint.size();
//		ss << "匹配度:" << Para.delta << "\\" << GPRate3 << endl;
//#pragma endregion
//
//#pragma region 结果综合判定
//		if (Para.delta >= GPRate3)
//		{
//			Para.isMatched = 0;
//		}
//		else
//		{
//			Para.isMatched = 1;
//			break;
//		}
//
//		result = 0;
//		//break;
//#pragma endregion
//	}
//	//ss << "镜片测试：" << result << endl;
//	//ss << "偏差X = " << Para.deltaX << endl;
//	//ss << "偏差Y = " << Para.deltaY << endl;
//	if (isGray)
//	{
//		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//	else {
//		stringstream ss2;
//
//		ss2 << "结果:" << result << endl;
//		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//	/////////////////////////////////////////////
//#pragma region 结果图像输出
//
//
//	tm.stop();
//#pragma endregion
//
//	//字体大小
//	int text_Size;
//	text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
//	//位置
//	Point text_Localtion01;
//	text_Localtion01.x = text_Size / 3;
//	text_Localtion01.y = text_Size / 3;
//	Point text_Localtion02;
//	text_Localtion02.x = text_Size / 3;
//	text_Localtion02.y = nHeight - text_Size * 4;
//	Point text_Localtion03;
//	text_Localtion03.x = text_Size / 3;
//	text_Localtion03.y = nHeight - text_Size * 3;
//
//
//	//return tm.getTimeMilli();
//
//	//stringstream ss;
//	/*ss << "时间 = " << tm.getTimeMilli() << endl;
//	if (result == 0)
//	{
//		ss << "结果 = OK"  << endl;
//
//	}
//	else { ss << "结果 = NG" << endl; }
//	ss << "偏差X = " << Para.deltaX << endl;
//	ss << "偏差Y = " << Para.deltaY << endl;
//	ss << "角度 = " << Para.angle << endl;
//	ss << "二值化阈值："<< Para.threshold<< endl;
//
//	std::string test = ss.str();
//	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体");*/
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\JPOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), output);
//
//	}
//	data.Height = output.rows;
//	data.Width = output.cols;
//	//输出
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = static_cast<int>(buf.size() - 54);
//	data.data_Output = new unsigned char[data.size];
//	std::copy(buf.begin() + 54
//		, buf.end(), data.data_Output);
//
//	//Output.output_Parameter_Float[1] = distance;
//	*output_Parameter_Int = tm.getTimeMilli();
//	output_Parameter_Int++;
//
//	*output_Parameter_Float = Para.angle +(360-BzAngle);;
//	output_Parameter_Float++;
//
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	//中心差值
//	*output_Parameter_Float = Para.deltaX * rule_X;
//	output_Parameter_Float++;
//	*output_Parameter_Float = Para.deltaY * rule_Y;
//	output_Parameter_Float++;
//	//镜筒结果
//	*output_Parameter_Float = result;
//	output_Parameter_Float++;
//	return 0;
//
//#pragma endregion
//}
//
//bool MV_GW01_DGP(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//{
//	return MV_EntryPoint(2, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
//	//载入图像
//	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
//	//MV_Release(data);
//	//Mat img = imread("C:\\Users\\10056\\Desktop\\tlh\\Image_20200908232018546.bmp", 0);
//
//	string systemtime;
//	stringstream picfile;
//
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//	picfile << ".\\picture\\DGP\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//	imwrite(picfile.str(), img);
//
//
//	Redate Para;
//	int result = false;
//	//Mat img = imread("D:\\projrct\\机器视觉图像匹配\\20200103\\20200103\\9-903\\Image_20200105160407605.bmp", 0);
//
//	Para.threshold = input_Parameter_Int[0];
//	Para.minRadiius = input_Parameter_Int[1];
//	Para.maxRadiius = input_Parameter_Int[2];
//
//	int Ana_DefectDetection_2_01 = input_Parameter_Int[3];
//	int Ana_DefectDetection_2_02 = input_Parameter_Int[4];
//	Para.radiusThr = input_Parameter_Int[5];
//	Para.isGray = 1;
//
//	int resultNum = input_Parameter_Int[6];
//	int QKLength = input_Parameter_Int[7];
//	float OutputAngle = 0;
//	float OutputX = 0;
//	float OutputY = 0;
//
//
//	Mat Color, Gray;
//
//	TickMeter tm;
//	tm.start();
//
//	Mat gray, output;
//	CONTOURS contours;
//	vector<Vec4i> hierarchy;
//
//	gray = img.clone();
//	Mat img_decode;
//
//	cvtColor(gray, img_decode, CV_GRAY2BGR);
//	//gray = gray - 20;
//	//gray *= 15;
//	//imshow("1", gray);
//	//waitKey(2);
//
//	Mat gray2 = gray.clone();
//	blur(gray2, gray, Size(3, 3));
//	//gray = 255 - gray;
//	threshold(gray, gray, Para.threshold, 255, THRESH_BINARY);
//	//adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, Para.threshold);
//	//blur(gray, gray, Size(3, 3));
//	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//
//	int MaxL = -1;
//	long area = -1;
//
//	for (int i = 0; i < contours.size(); i++)
//	{
//#if 1
//		vector<Point2i> tmpCont;
//		convexHull(contours[i], tmpCont);
//		int tmpArea = arcLength(tmpCont, 0);
//		if (tmpArea > 2 * CV_PI*Para.maxRadiius || tmpArea < 2 * CV_PI*Para.minRadiius)continue;
//
//		//double area = contourArea(tmpCont,0);
//		//if (area > CV_PI*Para.maxRadiius*Para.maxRadiius || area < CV_PI*Para.minRadiius*Para.minRadiius)
//		//	continue;
//
//#else
//		int tmpArea = arcLength(contours[i], 0);
//#endif
//
//		if (tmpArea > area)
//		{
//			area = tmpArea;
//			MaxL = i;
//		}
//	}
//
//	if (Para.isGray)
//	{
//		cvtColor(gray, output, CV_GRAY2BGR);
//	}
//	else
//		output = img_decode.clone();
//
//	//result = true;
//	/////////////////////////////////////////////
//	stringstream ss;
//	for (int i = 0; i < 1; i++)
//	{
//		if (MaxL < 0 || 6>contours[MaxL].size())break;
//
//		Point2f center(0, 0);
//		float radius;
//
//		vector<Point2i> tmpCont;
//		convexHull(contours[MaxL], tmpCont);
//		RotatedRect ell = fitEllipse(tmpCont);
//		ellipse(output, ell, Scalar(0, 0, 255), 5);
//		//minEnclosingCircle(contours[MaxL], center, radius);
//		center = ell.center;
//
//		//这里是需要判断的数据 0.99
//		Para.delta = ell.size.width / ell.size.height;
//		//drawContours(output, contours, MaxL, Scalar(0, 255, 0), 2);
//
//		vector<bool> GoodPoint;
//		float size = 720;
//		//这里的315跟290是设置值
//		Ana_DefectDetection_2(img, GoodPoint, center, Ana_DefectDetection_2_01, Ana_DefectDetection_2_02, Para.radiusThr, size, (Ana_DefectDetection_2_01 - Ana_DefectDetection_2_02) / 1.05);
//		circle(output, center, Ana_DefectDetection_2_02, Scalar(0, 0, 255), 2);
//		circle(output, center, Ana_DefectDetection_2_01, Scalar(0, 255, 0), 2);
//		for (int i = 0; i < size; i++)
//		{
//			float s = sin(i * 360 / size * CV_PI / 180);
//			float c = cos(i * 360 / size * CV_PI / 180);
//			Point2f start = center + Ana_DefectDetection_2_02 * Point2f(s, -c);
//			Point2f end = center + Ana_DefectDetection_2_01 * Point2f(s, -c);
//			if (GoodPoint[i])
//			{
//				line(output, start, end, Scalar(255, 255, 0), 2);
//			}
//			else
//			{
//				line(output, start, end, Scalar(0, 0, 255), 2);
//			}
//		}
//		Vec2i len = Ana_FindMaxLen(GoodPoint);
//		if (len[0] < 0)len[0] += size;
//		Para.deltaX = len[0] - len[1];
//		//这里25是设置参数，返回的angle是缺口个数 ==1
//		Para.angle = Ana_LenFilter(GoodPoint, QKLength);
//
//		circle(output, center, Para.maxRadiius, Scalar(0, 0, 255), 2);
//		circle(output, center, Para.minRadiius, Scalar(0, 255, 0), 2);
//		circle(output, center, Para.radius, Scalar(0, 255, 255), 2);
//
//
//		//判断依据
//
//		if (Para.angle != resultNum)
//		{
//			ss << "缺口数目不匹配：" << Para.angle << endl;
//			break;
//		}
//
//		if (Para.delta < 0.9)
//		{
//			ss << "圆度不足" << endl;
//			break;
//		}
//
//		result = true;
//
//		ss << "圆度：" << Para.delta << endl;
//		ss << "缺口个数：" << Para.angle << endl;
//
//		Para.deltaX = center.x - output.cols / 2;
//		Para.deltaY = center.y - output.rows / 2;
//	}
//	ss << "挡光片";
//
//	float Lens_NG = 1;//不良
//	if (result == true)
//	{
//		Lens_NG = 0;
//		ss << ":OK" << endl;
//	}
//	else
//	{
//		ss << ":NG" << endl;
//	}
//	OutputX = 0;
//	OutputY = 0;
//
//	//字体大小
//	int text_Size;
//	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
//	//位置
//	Point text_Localtion01;
//	text_Localtion01.x = text_Size / 3;
//	text_Localtion01.y = text_Size / 3;
//	Point text_Localtion02;
//	text_Localtion02.x = text_Size / 3;
//	text_Localtion02.y = nHeight - text_Size * 4;
//	Point text_Localtion03;
//	text_Localtion03.x = text_Size / 3;
//	text_Localtion03.y = nHeight - text_Size * 3;
//
//	std::string test = ss.str();
//	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
//
//	data.Height = output.rows;
//	data.Width = output.cols;
//
//	GetLocalTime(&st);
//	stringstream picfile1;
//	picfile1 << ".\\picture\\DGPOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//	imwrite(picfile1.str(), output);
//
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = buf.size() - 54;
//	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
//	std::copy(buf.begin() + 54, buf.end(), data.data_Output);
//
//	*output_Parameter_Int = tm.getTimeMilli();
//	output_Parameter_Int++;
//	//OutputX = 5;
//	//OutputY = 5;
//	//角度
//	*output_Parameter_Float = OutputAngle;
//	output_Parameter_Float++;
//	//正反
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	//中心差值
//	*output_Parameter_Float = OutputX;
//	output_Parameter_Float++;
//	*output_Parameter_Float = OutputY;
//	output_Parameter_Float++;
//	//镜片结果
//	*output_Parameter_Float = Lens_NG;
//	output_Parameter_Float++;
//
//
//	return 0;
//}
//
//bool MV_DJ01(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//{
//	return MV_EntryPoint(0, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
//#pragma region 载入图像
//
//	//载入图像
//	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
//	MV_Release(data);
//	//Mat img_decode = imread("D:\\projrct\\机器视觉图像匹配\\20200103\\20200103\\新建文件夹\\1-空镜筒.bmp", 0);
//	Redate Para;
//
//	int MaxL = -1;
//	long area = -1;
//	TickMeter tm;
//	stringstream ss;
//	Mat gray, output;
//	CONTOURS contours;
//	vector<Vec4i> hierarchy;
//
//	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
//	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
//
//	gray = img.clone();
//	Mat img_decode;
//
//	tm.start();
//
//
//	int result = 1;//1是ng 0是ok
//
//	float size = input_Parameter_Int[0];//扫描点数
//
//	bool isGray = input_Parameter_Int[1];//是否灰度显示
//
//	int threshold0 = input_Parameter_Int[2];//二值化阈值
//
//	int Radius1 = input_Parameter_Int[3];//轮廓外径
//	int Radius2 = input_Parameter_Int[4];//轮廓内径
//
//	int Radius3 = input_Parameter_Int[5];//角度-计算轮廓-外径
//	int Radius4 = input_Parameter_Int[6];//角度-计算轮廓-内径
//	int threshold1 = input_Parameter_Int[7];//角度-灰度阈值
//	float GPRate0 = input_Parameter_Int[8];//角度-有效点占比 0~1
//
//	float Radius5 = input_Parameter_Int[9];//缺陷深度下限
//	float Radius6 = input_Parameter_Int[10];//缺陷深度上限
//	float GPRate1 = input_Parameter_Int[11];//合格点比例
//
//	int Radius7 = input_Parameter_Int[12];//匹配轮廓-外径
//	int Radius8 = input_Parameter_Int[13];//匹配轮廓-内径
//	int threshold2 = input_Parameter_Int[14];//匹配-灰度阈值
//	float GPRate2 = input_Parameter_Int[15];//匹配-有效点占比
//	float GPRate3 = input_Parameter_Int[16];//匹配-有效角度占比
//	float BzCenterX = input_Parameter_Int[17];//画面补正中心X坐标
//	float BzCenterY = input_Parameter_Int[18];//画面补正中心Y坐标
//
//	int BzAngle = input_Parameter_Int[19];//补正角度
//	int isCap = input_Parameter_Int[20];//是否保存截图
//
//	float rule_X = input_Parameter_Int[21];//比例尺
//	float rule_Y = input_Parameter_Int[22];//比例尺
//#pragma region 图像预处理
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\DJ\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), img);
//
//	}
//	cvtColor(gray, img_decode, CV_GRAY2BGR);
//	Mat gray_Temp = gray.clone();
//	blur(gray_Temp, gray, Size(3, 3));
//	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
//	//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1));
//	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
//	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//#pragma endregion
//	/////////////////////////////////////////////
//#pragma region 筛选特定大小的轮廓
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		vector<Point2i> tmpCont;
//		convexHull(contours[i], tmpCont);
//		double tmpArea = arcLength(tmpCont, 0);
//		if (tmpArea > 2 * CV_PI*Radius1 || tmpArea < 2 * CV_PI*Radius2)continue;
//		if (tmpArea > area)
//		{
//			//double Area = contourArea(tmpCont);
//			//检测圆形是否规则
//			//if (Area < Radius2*Radius2*CV_PI || Area > Radius1*Radius1*CV_PI)continue;
//			area = (long)tmpArea;
//			MaxL = i;
//		}
//	}
//#pragma endregion
//
//#pragma region 判定是否二值化显示
//	if (isGray)
//	{
//		cvtColor(gray, output, CV_GRAY2BGR);
//	}
//	else
//		output = img_decode.clone();
//#pragma endregion
//
//	for (int i = 0; i < 1; i++)
//	{
//#pragma region 检测轮廓是否有效
//		if (MaxL<0 || 6>contours[MaxL].size())
//		{
//			ss << "无有效目标" << endl;
//			break;
//		}
//#pragma endregion
//
//#pragma region 检测圆度
//		drawContours(output, contours, MaxL, Scalar(255, 0, 0));
//#pragma endregion
//
//#pragma region 获取圆心及半径
//		Point2f center(0, 0);
//		float radius;
//		vector<Point2i> tmpCont;
//		convexHull(contours[MaxL], tmpCont);
//		RotatedRect ell = fitEllipse(tmpCont);
//		ellipse(output, ell, Scalar(255, 0, 0), 2);
//		center = ell.center;
//		Para.deltaX = center.x - BzCenterX;
//		Para.deltaY = center.y - BzCenterY;
//
//		radius = ell.size.width / 2;
//		ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
//
//		ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
//
//		circle(output, center, Radius1, Scalar(0, 0, 255), 2);
//		circle(output, center, Radius2, Scalar(0, 255, 0), 2);
//		//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
//#pragma endregion
//
//#pragma region 利用圆周灰度获取部品角度
//		vector<bool> GoodPoint;
//		//这里的Para1跟Para1是设置值
//		cvtColor(img_decode, gray, CV_BGR2GRAY);
//		Ana_DefectDetection_2(gray,
//			GoodPoint,
//			center,
//			Radius3,
//			Radius4,
//			threshold1,
//			size,
//			(Radius3 - Radius4) *GPRate0);
//
//		if (0 >= GoodPoint.size())
//		{
//			ss << "轮廓过小" << endl;
//			break;
//		}
//
//		//for (int i = 0; i < size; i++)
//		//{
//		//	float s = sin(i * 360 / size * CV_PI / 180);
//		//	float c = cos(i * 360 / size * CV_PI / 180);
//		//	Point2f start = center + para2 * Point2f(s, -c);
//		//	Point2f end = center + para1 * Point2f(s, -c);
//		//	if (GoodPoint[i])
//		//	{
//		//		line(output, start, end, Scalar(255, 255, 0), 1);
//		//	}
//		//	else
//		//	{
//		//		line(output, start, end, Scalar(0, 0, 255), 1);
//		//	}
//		//}
//
//		Vec2i ang = Ana_FindMaxLen(GoodPoint);
//		//ang[0] = (ang[0] + (int)size) % (int)size;
//		Para.angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
//		Para.angle = ((int)Para.angle + 360) % 360;
//
//		ss << "角度：" << Para.angle << endl;
//
//		line(output,
//			center,
//			Point(center.x + ell.size.width / 2 * sin(Para.angle * CV_PI / 180.0),
//				center.y - ell.size.width / 2 * cos(Para.angle * CV_PI / 180.0)),
//			Scalar(255, 0, 0),
//			4);
//#pragma endregion
//
//#pragma region 遍历轮廓，获取部品方向
//		//GoodPoint.clear();
//		//float para[2] = { Radius5,Radius6 };
//		//Ana_DefectDetection_1(contours[MaxL], para, GoodPoint, center);
//
//		//for (size_t i = 0; i < GoodPoint.size(); i++)
//		//{
//		//	if (GoodPoint[i])
//		//		circle(output, contours[MaxL][i], 1, Scalar(0, 255, 0), 2);
//		//	else
//		//		circle(output, contours[MaxL][i], 1, Scalar(0, 0, 255), 2);
//		//}
//
//		//Vec2i len = Ana_FindMaxLen(GoodPoint);
//		//Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
//		//double ang2 = Ana_ComputeAngle(center, end);
//		//line(output, center, end, Scalar(0, 255, 0), 1);
//
//		//float sum = 0;
//		//for (size_t i = 0; i < GoodPoint.size(); i++)
//		//{
//		//	sum += GoodPoint[i];
//		//}
//		//sum /= GoodPoint.size();
//		//ss << "轮廓类圆率：" << sum << endl;
//
//		//if (sum < GPRate1)
//		//{
//		//	ss << "部品外轮廓可能存在脏污" << endl;
//		//	break;
//		//}
//
//		//ss << "轮廓距离角度:" << ang2 << endl;
//#pragma endregion
//
//#pragma region 利用圆周灰度进行面别匹配
//		GoodPoint.clear();
//
//		Ana_DefectDetection_2(gray,
//			GoodPoint,
//			center,
//			Radius7,
//			Radius8,
//			threshold2,
//			size,
//			(Radius7 - Radius8)*GPRate2);
//
//		for (int i = 0; i < size; i++)
//		{
//			float s = sin(i * 360 / size * CV_PI / 180);
//			float c = cos(i * 360 / size * CV_PI / 180);
//			Point2f end = center + Radius7 * Point2f(s, -c);
//			if (GoodPoint[i])//BGR
//			{
//				circle(output, end, 1, Scalar(0, 255, 0), 2);
//			}
//			else
//			{
//				circle(output, end, 1, Scalar(0, 0, 255), 2);
//			}
//		}
//
//		////////////////////
//		Para.delta = 0;
//		for (size_t i = 0; i < GoodPoint.size(); i++)
//		{
//			Para.delta += GoodPoint[i];
//		}
//		Para.delta /= GoodPoint.size();
//		ss << "匹配度:" << Para.delta << endl;
//
//#pragma endregion
//
//#pragma region 结果综合判定
//		if (Para.delta >= GPRate3)
//		{
//			Para.isMatched = 0;
//
//		}
//		else
//		{
//			Para.isMatched = 1;
//			break;
//		}
//
//		result = 0;
//		//break;
//#pragma endregion
//	}
//	//ss << "偏差X = " << Para.deltaX << endl;
//	//ss << "偏差Y = " << Para.deltaY << endl;
//	//ss << "结果:" << result << endl;
//	if (isGray)
//	{
//		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//	else {
//		stringstream ss2;
//
//		ss2 << "结果:" << result << endl;
//		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//	/////////////////////////////////////////////
//#pragma region 结果图像输出
//
//
//	tm.stop();
//#pragma endregion
//
//	//字体大小
//	int text_Size;
//	text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
//	//位置
//	Point text_Localtion01;
//	text_Localtion01.x = text_Size / 3;
//	text_Localtion01.y = text_Size / 3;
//	Point text_Localtion02;
//	text_Localtion02.x = text_Size / 3;
//	text_Localtion02.y = nHeight - text_Size * 4;
//	Point text_Localtion03;
//	text_Localtion03.x = text_Size / 3;
//	text_Localtion03.y = nHeight - text_Size * 3;
//
//
//	//return tm.getTimeMilli();
//
//	//stringstream ss;
//	/*ss << "时间 = " << tm.getTimeMilli() << endl;
//	if (result == 0)
//	{
//		ss << "结果 = OK"  << endl;
//
//	}
//	else { ss << "结果 = NG" << endl; }
//	ss << "偏差X = " << Para.deltaX << endl;
//	ss << "偏差Y = " << Para.deltaY << endl;
//	ss << "角度 = " << Para.angle << endl;
//	ss << "二值化阈值："<< Para.threshold<< endl;
//
//	std::string test = ss.str();
//	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体");*/
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\DJOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), output);
//
//	}
//	data.Height = output.rows;
//	data.Width = output.cols;
//	//输出
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = static_cast<int>(buf.size() - 54);
//	data.data_Output = new unsigned char[data.size];
//	std::copy(buf.begin() + 54
//		, buf.end(), data.data_Output);
//
//	//Output.output_Parameter_Float[1] = distance;
//	*output_Parameter_Int = tm.getTimeMilli();
//	output_Parameter_Int++;
//
//	*output_Parameter_Float = Para.angle + BzAngle;
//	output_Parameter_Float++;
//
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	//中心差值
//	*output_Parameter_Float = Para.deltaX*rule_X;
//	output_Parameter_Float++;
//	*output_Parameter_Float = Para.deltaY*rule_Y;
//	output_Parameter_Float++;
//	//镜筒结果
//	*output_Parameter_Float = result;
//	output_Parameter_Float++;
//	return 0;
//
//#pragma endregion
//}
//
////bool MV_PJ01(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
////
////{
////
////	stringstream ss;
////	Mat img(nHeight, nWidth, CV_8UC3, data.data_Input);
////	Mat gray, output;
////	cvtColor(img, gray, CV_BGR2GRAY);
////	output = img.clone();
//////	Mat Color = imread("d:\\zsliujunj\\桌面\\1.bmp", 1);
////	//Mat Gray, img;
////
//////
//////	Color -= Scalar(0, 20, 0);
//////	Color *= 8;
//////cvtColor(img, img, CV_GRAY2BGR);
//////	//提取绿色通道
//////	{
//////		Mat mv[3];
//////		split(Color, mv);
//////
//////		Gray = mv[1].clone();
//////	}
//////
//////	//imshow("1", Gray);
//////	//waitKey(2);
//////
//////	TickMeter tm;
//////	tm.start();
//////
//////	Point2f center(0, 0);
//////
//////	float radius;
//////
//////
//////	vector<vector<Point>> contours;
//////	vector<Vec4i> hierarchy;
//////
//////	img = Gray.clone();
//////
//////
//////	//计算画面
//////	Mat Gray_Blur = Gray.clone();
//////	blur(Gray_Blur, Gray, Size(5, 5));
//////	threshold(Gray, Gray, 150, 255, CV_THRESH_BINARY);
//////	cv::findContours(Gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//////
//////	int MaxL;
//////	LONG area = -1;
//////
//////#pragma region 整段点胶
//////#if 0
//////	for (int i = 0; i < contours.size(); i++)
//////	{
//////		if (hierarchy[i][3] < 0)continue;
//////		int tmpArea = contourArea(contours[i]);
//////		if (tmpArea < 20000)continue;
//////
//////		if (tmpArea > area)
//////		{
//////			area = tmpArea;
//////			MaxL = i;
//////		}
//////	}
//////	float num = 0;
//////	if (area > 0)
//////	{
//////		minEnclosingCircle(contours[MaxL], center, radius);
//////		circle(Color, center, radius, Scalar(0, 255, 0), 2);
//////		circle(Color, center, radius + 30, Scalar(0, 255, 0), 2);
//////
//////		//判断胶圈完整
//////		float size = 360;
//////		vector<int> OutContour;
//////		Ana_DefectDetection_2(img, Color, OutContour, center, radius + 30, radius, 20, size, 1);
//////		//判断外圈是否有溢胶
//////		Ana_DefectDetection_2(img, Color, OutContour, center, radius + 40, radius + 35, 20, size, 0);
//////		//判断内圈是否有溢胶
//////		Ana_DefectDetection_2(img, Color, OutContour, center, radius - 5, radius - 10, 20, size, 0);
//////	}
//////	else
//////	{
//////
//////		ss << "找不到胶圈" << endl;//rows
//////
//////	}
//////#endif
//////#pragma endregion
//////
//////#pragma region 三段点胶
//////	MaxL = Ana_FindBigestContours(contours);
//////
//////	minEnclosingCircle(contours[MaxL], center, radius);
//////
//////	vector<bool> OutContour[4];
//////	float size = 360;
//////	//外环溢胶检测
//////	Ana_DefectDetection_2(img, OutContour[0], center, 250, 245, 10, size, 3);
//////
//////	//欠胶检测
//////	Ana_DefectDetection_2(img, OutContour[1], center, 210, 200, 10, size, 3);
//////
//////	//胶环检测
//////	Ana_DefectDetection_2(img, OutContour[2], center, 240, 210, 10, size, 19);
//////
//////	//内环溢胶检测
//////	Ana_DefectDetection_2(img, OutContour[3], center, 195, 190, 10, size, 3);
//////
//////	vector<bool> test;
//////
//////	//欠胶检测
//////	for (int i = 0; i < size; i++)
//////	{
//////		if (OutContour[2][i] && OutContour[1][i])
//////		{
//////			test.push_back(0);
//////
//////			float s = sin(i*CV_PI / 180);
//////			float c = cos(i*CV_PI / 180);
//////
//////			Point2f start = center + 210 * Point2f(s, -c);
//////			Point2f end = center + 240 * Point2f(s, -c);
//////
//////
//////			line(Color, start, end, Scalar(255, 255, 0), 1);
//////		}
//////		else
//////			test.push_back(1);
//////	}
//////	ss << "有效胶水数:" << Ana_LenFilter(test, 50) << endl;
//////
//////	test.clear();
//////	for (int i = 0; i < size; i++)
//////	{
//////		if (OutContour[1][i] && OutContour[0][i])
//////		{
//////			test.push_back(0);
//////
//////			float s = sin(i*CV_PI / 180);
//////			float c = cos(i*CV_PI / 180);
//////
//////			Point2f start = center + 250 * Point2f(s, -c);
//////			Point2f end = center + 245 * Point2f(s, -c);
//////
//////
//////			line(Color, start, end, Scalar(255, 255, 0), 1);
//////		}
//////		else
//////			test.push_back(1);
//////	}
//////	if (Ana_LenFilter(test, 1))
//////		ss << "外环有溢胶" << endl;
//////	else
//////		ss << "外环点胶正常" << endl;
//////
//////	test.clear();
//////	for (int i = 0; i < size; i++)
//////	{
//////		if (OutContour[1][i] && OutContour[3][i])
//////		{
//////			test.push_back(0);
//////
//////			float s = sin(i*CV_PI / 180);
//////			float c = cos(i*CV_PI / 180);
//////
//////			Point2f start = center + 190 * Point2f(s, -c);
//////			Point2f end = center + 195 * Point2f(s, -c);
//////
//////
//////			line(Color, start, end, Scalar(255, 255, 0), 1);
//////		}
//////		else
//////			test.push_back(1);
//////	}
//////	if (Ana_LenFilter(test, 1))
//////		ss << "内环有溢胶" << endl;
//////	else
//////		ss << "内环点胶正常" << endl;
//////
//////	radius = 250;
//////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
//////
//////	radius = 245;
//////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
//////
//////	radius = 240;
//////	circle(Color, center, radius, Scalar(0, 255, 0), 1);
//////
//////	radius = 210;
//////	circle(Color, center, radius, Scalar(255, 255, 0), 1);
//////
//////	radius = 200;
//////	circle(Color, center, radius, Scalar(0, 255, 0), 1);
//////
//////	radius = 195;
//////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
//////
//////	radius = 190;
//////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
//////#pragma endregion
//////
//////	//imshow("1", Color);
//////	//waitKey(2);
//////	//data.h = Color.rows;
//////	//data.w = Color.cols;
//////
//////	tm.stop();
//////	ss << "时间 = " << tm.getTimeMilli() << endl;//rows
//////	putTextZH(Color, ss.str().c_str(), Point(0, 0 + 20), Scalar(0, 255, 0), 50, "黑体", 0, 0);
////	//
////	//flip(Color, Color, 1);  //rotate 180
////	//flip(Color, Color, -1);  //rotate 180
////	////Mat img_decode(nHeight, nWidth, CV_8UC1, data.data_Input);
////	//resize(Color, Color, Size(nWidth, nHeight), 0, 0, INTER_LINEAR);
////
//////stringstream ss;
////ss << "判胶" << endl;
////
//////字体大小
////int text_Size;
////text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
//////位置
////Point text_Localtion01;
////text_Localtion01.x = text_Size / 3;
////text_Localtion01.y = text_Size / 3;
////Point text_Localtion02;
////text_Localtion02.x = text_Size / 3;
////text_Localtion02.y = nHeight - text_Size * 4;
////Point text_Localtion03;
////text_Localtion03.x = text_Size / 3;
////text_Localtion03.y = nHeight - text_Size * 3;
////
////
////	std::string test = ss.str();
////	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体",0);
////
////
////	//输出
////	std::vector<uchar>buf;
////	cv::imencode(".bmp", output, buf);
////	data.size = buf.size() - 54;
////	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
////	std::copy(buf.begin() + 54, buf.end(), data.data_Output);
////
////	return 0;
////
////}
//
//bool img_JS(Mat &img_decode, PJ_Para Para, int n)//这里还要加上参数列表
//{
//	int result = 1;//1是ng 0是
//	CONTOURS contours;
//	vector<Vec4i> hierarchy;
//	int MaxL = -1;
//	long area = -1;
//
//	Mat gray, output, green;
//	Mat mv[3];
//
//	stringstream ss;
//
//	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
//	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
//
//#pragma region 输入参数
//	bool isGray = Para.isGray;//是否灰度显示
//
//	//圆心定位
//	float B1 = Para.B1, G1 = Para.G1, R1 = Para.R1, Gain1 = Para.Gain1;//颜色通道配比，支持负数 默认值均为1
//	int threshold0 = Para.threshold0;
//	float radius1 = Para.radius1, radius2 = Para.radius2;
//
//	//胶水判别
//	float B2 = Para.B2, G2 = Para.G2, R2 = Para.R2, Gain2 = Para.Gain2;//颜色通道配比，支持负数 默认值均为1
//
//	int threshold1 = Para.threshold1;//默认值-1
//	if (n % 5 == 0) { threshold1 -= GetPrivateProfileInt("Main", "R0", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R11", 0, "./1.ini"); }
//	if (n % 5 == 1) { threshold1 -= GetPrivateProfileInt("Main", "R1", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R12", 0, "./1.ini"); }
//	if (n % 5 == 2) { threshold1 -= GetPrivateProfileInt("Main", "R2", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R13", 0, "./1.ini"); }
//	if (n % 5 == 3) { threshold1 -= GetPrivateProfileInt("Main", "R3", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R14", 0, "./1.ini"); }
//	if (n % 5 == 4) { threshold1 -= GetPrivateProfileInt("Main", "R4", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R15", 0, "./1.ini"); }
//
//	//欠胶上限 胶水厚度下限
//	int thLen0 = Para.thLen0, thLen1 = Para.thLen1;
//	//6 7 胶水检测 5 6欠胶检测 9 8内环溢胶检测 3 4 外环溢胶检测 奇数为外径 偶数为内径
//	float radius3 = Para.radius3, radius4 = Para.radius4;
//	float radius5 = Para.radius5, radius6 = Para.radius6;
//	float radius7 = Para.radius7, radius8 = Para.radius8;
//	float radius9 = Para.radius9;
//
//	int num = Para.num;//胶水段数
//	int len = Para.len;//胶水长度 默认值0
//
//#pragma endregion
//	ss << "镜筒编号：" << Para.JTNum << endl;
//#pragma region 图像第一次预处理
//	//B1 /= 30;
//	//G1 /= 30;
//	//R1 /= 30;
//	//Gain1 /= 10;
//	//上面是为了进行归一
//	cv::split(img_decode, mv);
//	gray = B1 * mv[0] + G1 * mv[1] + R1 * mv[2];
//	gray *= Gain1;
//
//
//
//	//cvtColor(img_decode, gray, CV_BGR2GRAY);
//	Mat grayTemp = gray.clone();
//	blur(grayTemp, gray, Size(3, 3));
//	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
//#pragma endregion
//
//#pragma region 判定是否二值化显示
//	if (isGray)
//	{
//		cvtColor(gray, output, CV_GRAY2BGR);
//	}
//	else
//		output = img_decode.clone();
//#pragma endregion
//
//#pragma region 筛选特定大小的轮廓
//	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		vector<Point2i> tmpCont;
//		convexHull(contours[i], tmpCont);
//		double tmpArea = contourArea(tmpCont, 0);
//		if (tmpArea > CV_PI*radius1*radius1 || tmpArea < CV_PI*radius2*radius2)continue;
//		if (tmpArea > area)
//		{
//			area = (long)tmpArea;
//			MaxL = i;
//		}
//	}
//#pragma endregion
//
//	for (int i = 0; i < 1; i++)
//	{
//#pragma region 检测轮廓是否有效
//		if (MaxL < 0)
//		{
//			//ss << "无有效目标" << endl;
//			break;
//		}
//
//		if (30 > contours[MaxL].size())
//		{
//			ss << "轮廓无效" << endl;
//			break;
//		}
//#pragma endregion
//		if (MaxL >= 0)
//		{
//#pragma region 检测圆度
//			drawContours(output, contours, MaxL, Scalar(255, 0, 0), 2);
//			double Area = contourArea(contours[MaxL]);
//			//检测圆形是否规则
//			if (Area < radius2*radius2*CV_PI || Area > radius1*radius1*CV_PI)
//			{
//				ss << "轮廓圆度不足" << endl;
//				break;
//			}
//			ss << "轮廓面积:" << Area << endl;
//#pragma endregion
//
//#pragma region 获取圆心及半径
//			Point2f center(0, 0);
//			float radius;
//			vector<Point2i> tmpCont;
//			convexHull(contours[MaxL], tmpCont);
//			RotatedRect ell = fitEllipse(tmpCont);
//			ellipse(output, ell, Scalar(255, 0, 0), 2);
//			center = ell.center;
//			//Para.deltaX = (int)center.x;
//			//Para.deltaY = (int)center.y;
//
//			radius = ell.size.width / 2;
//			ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
//
//			ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
//
//			circle(output, center, radius1, Scalar(0, 0, 255), 1);
//			circle(output, center, radius2, Scalar(0, 255, 0), 1);
//			//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
//#pragma endregion
//
//#pragma region 图像第二次预处理
//			if (0 > threshold1)break;
//
//			//B2 /= 30;
//			//G2 /= 30;
//			//R2 /= 30;
//			//Gain2 /= 10;
//			//上面是为了进行归一
//			cv::split(img_decode, mv);
//
//			green = G2 * mv[1].clone();
//			grayTemp = green.clone();
//			blur(grayTemp, green, Size(3, 3));
//			threshold(green, green, threshold1, 255, THRESH_BINARY);//二值化阈值
//
//
//			gray = B2 * mv[0] + G2 * mv[1] + R2 * mv[2];
//			gray *= Gain2;
//			//cvtColor(img_decode, gray, CV_BGR2GRAY);
//			grayTemp = gray.clone();
//			blur(grayTemp, gray, Size(3, 3));
//			threshold(gray, gray, threshold1, 255, THRESH_BINARY);//二值化阈值
//
//
//#pragma endregion
//
//#pragma region 判定是否二值化显示
//			if (isGray)
//			{
//				cvtColor(gray, output, CV_GRAY2BGR);
//			}
//			else
//				output = img_decode.clone();
//#pragma endregion
//
//#pragma region 点胶判断
//			radius = radius3;
//			circle(output, center, radius, Scalar(0, 0, 255), 1);
//
//			radius = radius4;
//			circle(output, center, radius, Scalar(0, 0, 255), 1);
//
//			radius = radius5;
//			circle(output, center, radius, Scalar(0, 255, 0), 1);
//
//			radius = radius6;
//			circle(output, center, radius, Scalar(255, 255, 0), 1);
//
//			radius = radius7;
//			circle(output, center, radius, Scalar(0, 255, 0), 1);
//
//			radius = radius8;
//			circle(output, center, radius, Scalar(0, 0, 255), 1);
//
//			radius = radius9;
//			circle(output, center, radius, Scalar(0, 0, 255), 1);
//
//			vector<bool> OutContour[4];
//			float size = 360;
//			//外环溢胶检测
//			Ana_DefectDetection_2(gray, OutContour[0], center, radius3, radius4, 0, size, 1);
//
//			//欠胶检测
//			Ana_DefectDetection_2(green, OutContour[1], center, radius5, radius6, 0, size, thLen0);
//
//			//胶环检测
//			Ana_DefectDetection_2(green, OutContour[2], center, radius7, radius6, 0, size, thLen1);
//
//			//内环溢胶检测
//			Ana_DefectDetection_2(gray, OutContour[3], center, radius9, radius8, 0, size, 1);
//
//			vector<bool> test;
//
//			//欠胶检测
//			for (int i = 0; i < size; i++)
//			{
//				if (OutContour[2][i] && OutContour[1][i])
//				{
//					test.push_back(0);
//
//					float s = sin(i*CV_PI / 180);
//					float c = cos(i*CV_PI / 180);
//
//					Point2f start = center + radius6 * Point2f(s, -c);
//					Point2f end = center + radius7 * Point2f(s, -c);
//
//					line(output, start, end, Scalar(255, 255, 0), 1);
//				}
//				else
//					test.push_back(1);
//			}
//
//			float iii = 0;
//			for (size_t i = 0; i < test.size(); i++)
//			{
//				if (OutContour[2][i])iii++;
//			}
//			iii /= test.size();
//			//ss << iii << endl;
//			iii *= size / len;
//			//ss << iii << endl;
//			if (iii < 1)
//			{
//				ss << "胶水量异常" << endl;
//				ss << len / size << endl;
//				break;
//			}
//
//			int flag = GetPrivateProfileInt("Main", "JSM", 0, "./1.ini");
//			if (!flag)
//			{
//				if (num != Ana_LenFilter(test, len))
//				{
//					ss << "有效胶水段数：" << Ana_LenFilter(test, len) << endl;
//					break;
//				}
//				ss << "有效胶水数:" << num << endl;
//			}
//			else
//			{
//				iii = 0;
//				for (size_t i = 0; i < test.size(); i++)
//				{
//					if (OutContour[2][i])iii++;
//				}
//				iii /= test.size();
//
//				if (iii >= (num - 1)*len / size)
//				{
//					ss << "胶水量足够" << endl;
//				}
//				else
//				{
//					ss << "胶水量欠缺" << endl;
//					ss << iii << "//" << (num - 1)*len / size << endl;
//					break;
//				}
//			}
//
//			test.clear();
//			for (int i = 0; i < size; i++)
//			{
//				if (OutContour[1][i] && OutContour[0][i])
//				{
//					test.push_back(0);
//
//					float s = sin(i*CV_PI / 180);
//					float c = cos(i*CV_PI / 180);
//
//					Point2f start = center + radius4 * Point2f(s, -c);
//					Point2f end = center + radius3 * Point2f(s, -c);
//
//					line(output, start, end, Scalar(255, 255, 0), 1);
//				}
//				else
//					test.push_back(1);
//			}
//			if (Ana_LenFilter(test, 1))
//			{
//				ss << "外环有溢胶" << endl;
//				break;
//			}
//			else
//				ss << "外环点胶正常" << endl;
//
//			test.clear();
//			for (int i = 0; i < size; i++)
//			{
//				if (OutContour[1][i] && OutContour[3][i])
//				{
//					test.push_back(0);
//
//					float s = sin(i*CV_PI / 180);
//					float c = cos(i*CV_PI / 180);
//
//					Point2f start = center + radius9 * Point2f(s, -c);
//					Point2f end = center + radius8 * Point2f(s, -c);
//
//					line(output, start, end, Scalar(255, 255, 0), 1);
//				}
//				else
//					test.push_back(1);
//			}
//			if (Ana_LenFilter(test, 1))
//			{
//				ss << "内环有溢胶" << endl;
//				break;
//			}
//			else
//				ss << "内环点胶正常" << endl;
//#pragma endregion
//
//			result = 0;//仅输出OK/NG
//		}
//
//	}
//	//Scalar color = Scalar(0, 255, 0);
//	//if (result == 1)color = Scalar(0, 0, 255);
//
//	rectangle(output, Rect(0, 0, output.cols, output.rows), Scalar(0, 0, 255), 8);
//	//putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 0, 255), 40, "黑体", 0, 0);
//	if (isGray == 0)
//	{
//		stringstream ss2;
//		ss2 << "镜筒编号：" << Para.JTNum << endl;
//		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 80, "黑体", 0, 0);
//		if (result == 0)
//		{
//			stringstream ss3;
//			ss3 << "OK" << endl;
//			putTextZH(output, ss3.str().c_str(), Point(20, 100), Scalar(0, 255, 0), 200, "黑体", 0, 0);
//		}
//		else
//		{
//			stringstream ss3;
//			ss3 << "NG" << endl;
//			putTextZH(output, ss3.str().c_str(), Point(20, 100), Scalar(0, 0, 255), 200, "黑体", 0, 0);
//
//		}
//	}
//	else
//	{
//		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
//	}
//
//	img_decode = output.clone();
//	output.release();
//
//
//	return result;
//}
//
//bool MV_PJ01(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//{
//	return MV_EntryPoint(3, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
//	//Mat img_decode = imread("E:\\MM\\1.bmp");
//	Mat img_decode(nHeight, nWidth, CV_8UC3, data.data_Input);
//	//MV_Release(data);
//
//	TickMeter tm;
//	tm.start();
//
//	/////////////////////////////////////////////
//#pragma region 输入参数
//	PJ_Para Para;
//	Para.isGray = input_Parameter_Int[0];
//	Para.B1 = input_Parameter_Int[1];
//	Para.G1 = input_Parameter_Int[2];
//	Para.R1 = input_Parameter_Int[3];
//	Para.Gain1 = input_Parameter_Int[4];
//	Para.threshold0 = input_Parameter_Int[5];
//	Para.radius1 = input_Parameter_Int[6];
//	Para.radius2 = input_Parameter_Int[7];
//	Para.B2 = input_Parameter_Int[8];
//	Para.G2 = input_Parameter_Int[9];
//	Para.R2 = input_Parameter_Int[10];
//	Para.Gain2 = input_Parameter_Int[11];
//	Para.threshold1 = input_Parameter_Int[12];
//	Para.thLen0 = input_Parameter_Int[13];
//	Para.thLen1 = input_Parameter_Int[14];
//	Para.radius3 = input_Parameter_Int[15];
//	Para.radius4 = input_Parameter_Int[16];
//	Para.radius5 = input_Parameter_Int[17];
//	Para.radius6 = input_Parameter_Int[18];
//	Para.radius7 = input_Parameter_Int[19];
//	Para.radius8 = input_Parameter_Int[20];
//	Para.radius9 = input_Parameter_Int[21];
//	Para.num = input_Parameter_Int[22];
//	Para.len = input_Parameter_Int[23];
//	float JT_Num = input_Parameter_Int[24];
//	int LeftUp_x = img_decode.cols *input_Parameter_Int[25];//四点坐标
//	int	LeftUp_y = img_decode.rows *input_Parameter_Int[26];
//	int RightDown_x = img_decode.cols *input_Parameter_Int[27];
//	int RightDown_y = img_decode.rows*input_Parameter_Int[28];
//	float isCap = input_Parameter_Int[29];
//#pragma endregion
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\PJ\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), img_decode);
//
//	}
//#pragma region 切割并识别图片
//	int flag = 0;
//	Mat output = img_decode;
//	int Roi_w = (RightDown_x - LeftUp_x) / 5;
//	int Roi_h = (RightDown_y - LeftUp_y) / 5;
//	bool Result[25] = { false };
//	for (int i = 0; i < 5; i++)
//	{
//		for (int j = 0; j < 5; j++)
//		{
//			flag++;
//			int Roi_x = LeftUp_x + j * Roi_w;
//			int Roi_y = LeftUp_y + i * Roi_h;
//			Mat Roi_output = img_decode(Rect(Roi_x, Roi_y, Roi_w, Roi_h));
//			Para.JTNum = i * 5 + j + 1;
//			Result[i * 5 + j] = img_JS(Roi_output, Para, j);
//
//			Mat imgRoi = img_decode(Rect(Roi_x, Roi_y, Roi_w, Roi_h));
//			Roi_output.copyTo(imgRoi, Roi_output);
//			if (flag == JT_Num)
//			{
//				output.release();
//				output = Roi_output;
//			}
//		}
//	}
//#pragma endregion
//
//	if (JT_Num == 0)
//	{
//		resize(img_decode, output, Size(0, 0), 0.5, 0.5);
//	}
//	/////////////////////////////////////////////
//	tm.stop();
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\PJOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), output);
//
//	}
//
//
//
//	data.Height = output.rows;
//	data.Width = output.cols;
//	//data.size = output.total()*output.elemSize();
//	//data.data_Output = (uchar*)calloc(data.size, sizeof(uchar));
//	//std::memcpy(data.data_Output, output.data, data.size*sizeof(uchar));
//
//	//输出
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = static_cast<int>(buf.size() - 54);
//	data.data_Output = new unsigned char[data.size];
//	std::copy(buf.begin() + 54
//		, buf.end(), data.data_Output);
//
//	for (int i = 0; i < 25; i++)
//	{
//		output_Parameter_Int[i] = Result[i];
//	}
//
//
//
//	return 0;
//}

//查找所有满足长度要求的缺口中心 2020-11-10
float aaa(vector<bool> inContour, int max, int min)
{
	int start = 0;
	int size = inContour.size();
	std::vector<int> Down2Up, Up2Down;

	while (!inContour[start])//查找一个高电位，没有的话直接退出算法
	{
		start++;
		if (start >= inContour.size())
			return 0;
	}

	for (int i = start; i < start + size; i++)
	{
		if (inContour[i%size] && !inContour[(i + 1) % size] /*&& !inContour[(i + 2) % size]*/)
			Down2Up.push_back(i%size);

		if (!inContour[i%size] && inContour[(i + 1) % size] /*&& inContour[(i + 2) % size]*/)
			Up2Down.push_back(i%size);
	}

	start = 0;//用作累加
	size = 0;//用过计数
	for (int i = 0; i < Up2Down.size(); i++)
	{
		if (Down2Up[i] > Up2Down[i])Down2Up[i] -= size;
		int len = Up2Down[i] - Down2Up[i];
		if (len > min && len < max)
		{
			int newAng = (Up2Down[i] + Down2Up[i]) / 2;//作为起点
			if (size == 0)
			{
				start = newAng;
			}
			else
			{
				if (180 < abs(newAng - start))
				{
					start += newAng;
					start /= 2;
					start += 180;
				}
			}
			//start += (Up2Down[i] + Down2Up[i]) / 2;
			size++;
		}
	}
	size = 1;
	start = start * 360 / inContour.size();
	start %= 360;
	return ((float)start) / size;
}

//bool MV_TP(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//{
//	Mat gray(nHeight, nWidth, CV_8UC1, data.data_Input);
//
//	//Mat gray = imread("C:\\Image_20200907112002782.bmp", 0);
//
//	Mat img_decode, output;
//	cvtColor(gray, img_decode, CV_GRAY2BGR);
//	//Redate Para;
//	TickMeter tm;
//	tm.start();
//
//
//	//cvtColor(img_decode,output, CV_GRAY2BGR);
//
//	CONTOURS contours;
//	vector<Vec4i> hierarchy;
//	stringstream out;
//	int num = 0;
//
//	out << "输出";
//#pragma region 参数载入
//	int sx = input_Parameter_Int[0];
//	int sy = input_Parameter_Int[1];
//	int ex = input_Parameter_Int[2];
//	int ey = input_Parameter_Int[3];
//	int angle = input_Parameter_Int[4];;//画面旋转角度
//	int threshold = input_Parameter_Int[5];;//灰度阈值
//	int MaxRadius = input_Parameter_Int[6];;//半径上限
//	int MinRadius = input_Parameter_Int[7];;//半径下限
//	int isGray = input_Parameter_Int[8];;//灰度显示
//	int isCap = input_Parameter_Int[9];;//是否截图
//#pragma endregion
//
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\TP\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), img_decode);
//
//	}
//
//#pragma region 轮廓获取
//	//cvtColor(img_decode, gray, CV_BGR2GRAY);
//	//gray = img_decode.clone();
//	Mat gray_tmp;
//	blur(gray, gray_tmp, Size(3, 3));
//	cv::threshold(gray_tmp, gray, threshold, 255, THRESH_BINARY);//二值化阈值
//
//	Mat ROI_g = gray(Rect(sx, sy, ex - sx, ey - sy));
//	Mat ROI_c = img_decode(Rect(sx, sy, ex - sx, ey - sy));
//
//	findContours(ROI_g, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
//#pragma endregion
//
//#pragma region 获取轮廓
//	int MaxL = -1;
//	long area = -1;
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//#if 1
//		vector<Point2i> tmpCont;
//		convexHull(contours[i], tmpCont);
//
//		double tmpArea = contourArea(tmpCont, 0);
//		if (tmpArea > CV_PI * MaxRadius*MaxRadius || tmpArea < CV_PI * MinRadius*MinRadius)continue;
//#else
//		int tmpArea = arcLength(contours[i], 0);
//#endif
//
//		if (tmpArea > area)
//		{
//			area = (long)tmpArea;
//			MaxL = i;
//		}
//	}
//#pragma endregion
//
//#pragma region 九宫格识别
//#if 1
//	if (MaxL >= 0)
//	{
//		drawContours(ROI_c, contours, MaxL, Scalar(255, 0, 0), 2);
//		RotatedRect rect = minAreaRect(contours[MaxL]);
//
//		Point2f rangle[4];
//		rect.points(rangle);
//
//		int lu = -1;
//		int tmp = 9999999;
//		for (int i = 0; i < 4; i++)
//		{
//			if (rangle[i].x + rangle[i].y < tmp)
//			{
//				tmp = rangle[i].x + rangle[i].y;
//				lu = i;
//			}
//		}
//
//		int x_delta = rangle[(lu + 1) % 4].x - rangle[lu].x;
//		int y_delta = rangle[(lu + 3) % 4].y - rangle[lu].y;
//
//		int x_delta_y = rangle[(lu + 3) % 4].x - rangle[lu].x;
//		int y_delta_x = rangle[(lu + 1) % 4].y - rangle[lu].y;
//
//
//		for (size_t i = 0; i < 3; i++)
//		{
//			for (size_t j = 0; j < 3; j++)
//			{
//
//				int x = rangle[lu].x + i * (x_delta + x_delta_y) / 3 + 0.1*x_delta / 3;
//				int y = rangle[lu].y + j * (y_delta + y_delta_x) / 3 + 0.1*y_delta / 3;
//
//				stringstream ss;
//				ss << j + 3 * i << endl;
//
//				//putTextZH(ROI_c, ss, Point(x, y), Scalar(255, 0, 0), 15);
//				putTextZH(ROI_c, ss.str().c_str(), Point(x, y), Scalar(0, 255, 0), 80, "黑体", 0, 0);
//				rectangle(ROI_c, Rect(x, y, x_delta / 3 * 0.8, y_delta / 3 * 0.8), Scalar(0, 255, 0), 4);
//
//				Mat tmp_m, tmp_sd;
//				Mat mask = Mat(ROI_g.rows, ROI_g.cols, CV_8UC1, Scalar(0));
//				rectangle(mask, Rect(x, y, x_delta / 3 * 0.8, y_delta / 3 * 0.8), Scalar(255), -1);
//
//				meanStdDev(ROI_g, tmp_m, tmp_sd, mask);
//
//				if (tmp_m.at<double>(0, 0) < 128)
//				{
//					out << 1;
//					num += 1;
//				}
//				else
//				{
//					out << 0;
//					num += 0;
//				}
//				num *= 2;
//			}
//		}
//		num /= 2;
//
//		//circle(ROI_c, rangle[lu], 15, Scalar(255, 0, 0), -1);//B
//		//lu = (lu + 1) % 4;
//		//circle(ROI_c, rangle[lu], 15, Scalar(0, 255, 0), -1);//G
//		//lu = (lu + 1) % 4;
//		//circle(ROI_c, rangle[lu], 15, Scalar(0, 0, 255), -1);//R
//		//lu = (lu + 1) % 4;
//		//circle(ROI_c, rangle[lu], 15, Scalar(25, 25, 0), -1);
//	}
//#endif // 1
//#pragma endregion
//
//#pragma region 输出图像选择
//	if (isGray)
//	{
//		cvtColor(gray, output, CV_GRAY2BGR);
//	}
//	else
//		output = img_decode.clone();
//#pragma endregion
//
//	out << endl << "托盘号：" << num;
//	putTextZH(output, out.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 80, "黑体", 0, 0);
//	rectangle(output, Rect(sx, sy, ex - sx, ey - sy), Scalar(0, 255, 0), 4);
//
//	if (isCap == 1)
//	{
//		stringstream picfile;
//		SYSTEMTIME st;
//		GetLocalTime(&st);
//		picfile << ".\\picture\\TPOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
//		imwrite(picfile.str(), output);
//
//	}
//
//
//
//	tm.stop();
//
//#pragma region 图像输出
//	data.Height = output.rows;
//	data.Width = output.cols;
//	//输出
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = static_cast<int>(buf.size() - 54);
//	data.data_Output = new unsigned char[data.size];
//	std::copy(buf.begin() + 54
//		, buf.end(), data.data_Output);
//
//	//Output.output_Parameter_Float[1] = distance;
//	//*output_Parameter_Int = tm.getTimeMilli();
//	//output_Parameter_Int++;
//	*output_Parameter_Int = 0;
//	output_Parameter_Int++;
//
//	*output_Parameter_Float = num;
//	output_Parameter_Float++;
//
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	//中心差值
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//	//镜筒结果
//	*output_Parameter_Float = 0;
//	output_Parameter_Float++;
//#pragma endregion
//}
/*******************************************************************************************************************************************************

以下为2020-11重新整理的算法

*******************************************************************************************************************************************************/


bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

bool MV_JT(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	//Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img_Temp = imread("X:\\中山工程技术部临时交换区\\tlh\\BL_CF\\C\\C1.bmp",0);
	//Mat img = img_Temp.clone();
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//Mat img = imread("E:\\MM\\一体机\\1111\\test.bmp", 0);
	//LPSTR path = (LPSTR)malloc(sizeof(char)*1024);
	//GetPrivateProfileString("Main", "Path","",path,1024,"./2.ini");
	//Mat img = imread(path, 0);

	/*  2021-05-08 TLH增加  */
	SYSTEMTIME st;
	GetLocalTime(&st);
	//////////////////////////////////////

	bool isGray = (bool)atoi(input_Parameter[0]);	//灰度显示       0表示显示原图 1表示显示灰度图

	int r0		= atoi(input_Parameter[1]);	//定位-内环半径
	int r1		= atoi(input_Parameter[2]);	//定位-外环半径
	int th0		= atoi(input_Parameter[3]);	//定位-灰度阈值

	int isCanny	= atoi(input_Parameter[4]);//角度-边缘显示 //0表示不启用 3表示启用
	//int para0	= atoi(input_Parameter[5]);//角度-边缘参数0
	//int para1	= atoi(input_Parameter[6]);	//角度-边缘参数1
	vector<string> _para0 = Spilt(input_Parameter[5]);
	int para1 = atoi(input_Parameter[6]);	//角度-边缘参数1



	int r3		= atoi(input_Parameter[7]);		//角度-外环半径
	int r2		= atoi(input_Parameter[8]);	//角度-内环半径
	float GPR0	= atof(input_Parameter[9]);	//径向有效点比例
	//GPR0 /= 100;
	int th1		= atoi(input_Parameter[10]);	//角度-灰度阈值
	int angL	= atoi(input_Parameter[11]);	//角度-缺口长度

	//vector<string> angL_num;
	//angL_num = Spilt(input_Parameter[11]);
	//int angL = stoi(angL_num[0]);
	//int angN = 1;
	//if (angL_num.size() > 1)
	//{
	//	angN = stoi(angL_num[1]);
	//}
	
	int CN		= atoi(input_Parameter[12]);	//匹配-圆环数量
	 
	int *r4		= (int*)malloc(sizeof(int)*CN);		//匹配-半径下限
	int *r5		= (int*)malloc(sizeof(int)*CN);		//匹配-半径上限
	float *GPR1	= (float*)malloc(sizeof(float)*CN);	//匹配-径向有效点比例
	float *GPR2	= (float*)malloc(sizeof(float)*CN);	//匹配-切向有效点比例
	int *th2	= (int*)malloc(sizeof(int)*CN);		//匹配-灰度阈值

	int *errcode = (int*)malloc(sizeof(int)*CN);		//匹配-错误代码
	
	
	vector<string> R4;
	vector<string> R5;
	vector<string> gpr1;
	vector<string> gpr2;
	vector<string> TH2;
	vector<string> ErrCode;

	R4 = Spilt(input_Parameter[13]);
	R5 = Spilt(input_Parameter[14]);
	gpr1 = Spilt(input_Parameter[15]);
	gpr2 = Spilt(input_Parameter[16]);
	TH2 = Spilt(input_Parameter[17]);

	ErrCode = Spilt(input_Parameter[24]);
	//ErrCode.push_back("0");

	if (CN > 1 && R4.size() == CN && R5.size() == CN && gpr1.size()== CN && gpr2.size() == CN && TH2.size() == CN && ErrCode.size() == CN)
	{
		for (size_t i = 0; i < CN; i++)
		{
			r4[i] = stoi(R4[i]);
			r5[i]	= stoi(R5[i]);
			GPR1[i] = stof(gpr1[i]);
			GPR2[i] = stof(gpr2[i]);
			th2[i]	= stoi(TH2[i]);
			errcode[i] = stoi(ErrCode[i]);
		}
	}
	else
	{
		CN = 1;
		r4[0] = stoi(R4[0]);
		r5[0] = stoi(R5[0]);
		GPR1[0] = stof(gpr1[0]);
		GPR2[0] = stof(gpr2[0]);
		th2[0] = stoi(TH2[0]);
		errcode[0] = stoi(ErrCode[0]);
	}
	
	float BzCenterX = atof(input_Parameter[18]);	//补偿基准点X
	float BzCenterY = atof(input_Parameter[19]);	//补偿基准点Y

	float rule_X = atof(input_Parameter[20]);//比例尺
	float rule_Y = atof(input_Parameter[21]);//比例尺

	float HzAngle = atoi(input_Parameter[22]);//回正角度


	vector<string> SaveImg;
	int SaveImgNum = 0;
	int CameraNum = 0;
	int XTNum = 0;
	SaveImg = Spilt2(input_Parameter[23]);
	float radius;
	float Offset;
	float MaxOffset = 0.02;

	LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	if (SaveImg.size() == 3)
	{
		SaveImgNum = stoi(SaveImg[2]);
		CameraNum = stoi(SaveImg[0]);
		XTNum = stoi(SaveImg[1]);

		string str = "MaxOffset";
		str += SaveImg[0] + SaveImg[1];
		GetPrivateProfileString("Main", str.c_str(), "0.1", path, 1024, "./Offset.ini");
		OutputDebugString(str.c_str());
		MaxOffset = stof(path);

	}
	free(path);

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for(int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		//SYSTEMTIME st;
		//GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum <<"\\" <<"0"<< XTNum <<"\\Input\\"<< (filenames.size()+1) << ".bmp";
		imwrite(picfile.str(), img);
	}
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = 0;
	int size = 360;	//圆周切割段数 这里用360做定指扫描
	Vec2i ang;
	TickMeter tm;
	RotatedRect ell;
	CONTOURS contours;
	stringstream OK_ss;
	stringstream ERR_ss;
	Point2f center(0, 0);
	vector<bool> GoodPoint;
	vector<Vec4i> hierarchy;
	vector<Point2i> tmpCont;
	Mat gray, output, img_decode,temp;
	
	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "镜筒算法" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "镜筒算法" << endl;

	gray = img.clone();

	int angex[3] = { 60,180,300 };

	vector<float> _avg;
	vector<float> _res;
	int a = 0;

	tm.start();
#pragma endregion

#pragma region 输出参数
	float deltaX = 0;
	float deltaY = 0;
	float angle = 0;
	bool result = 1; //1是ng 0是ok
#pragma endregion

#pragma region 图像预处理
	cvtColor(gray, img_decode, CV_GRAY2BGR);
	//这里需要增加一个待测图片ROI选取
	//如果增加了ROI区域 是否会由概率出现识别异常
	//先留计划

	Mat gray_Temp = gray.clone();
	blur(gray_Temp, gray, Size(3, 3));	//高斯滤波

	threshold(gray, gray, th0, 255, THRESH_BINARY);//二值化阈值
	//threshold(gray, gray, th0, 255, THRESH_TRIANGLE);//二值化阈值
	//adaptiveThreshold(gray, gray, th0, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 0);

	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
#pragma endregion
	
#pragma region 轮廓筛选
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea	= contourArea(tmp);
		double tmpLen	= arcLength(tmp, 0);
		if (tmpLen > 2 * CV_PI*r1 || tmpLen < 2 * CV_PI*r0) continue;
		if (tmpArea > CV_PI*r1*r1 || tmpArea < CV_PI*r0*r0) continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}
#pragma endregion

#pragma region 灰度显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();

	//这里需要增加一个以图像中心为远点的ROI边界绘制
	circle(output, Point2f(nWidth / 2, nHeight / 2), r0, Scalar(0, 255, 0), 2);
	circle(output, Point2f(nWidth / 2, nHeight / 2), r1, Scalar(0, 255, 0), 2);
#pragma endregion

#pragma region 轮廓判定
	if (MaxL < 0)
	{
		ERR_ss << "未定位到有效边缘" << endl;
		goto OutPoint;
	}
#pragma endregion
	
#pragma region 圆心确定
	//float radius;

	convexHull(contours[MaxL], tmpCont);
	ell = fitEllipse(tmpCont);
	ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
	center = ell.center;
	deltaX = (center.x - BzCenterX)*rule_X;
	deltaY = (center.y - BzCenterY)*rule_Y;
	radius = ell.size.width / 2;

	Offset = sqrt(deltaX*deltaX + deltaY * deltaY);

	if (Offset > MaxOffset)
	{
		//ERR_ss << " " << endl;
		//ERR_ss << " " << endl;
		//ERR_ss << " " << endl;

		ERR_ss << "dx = " << deltaX << endl;
		ERR_ss << "dy = " << deltaY << endl;
		ERR_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
		ERR_ss << "偏移量过大！请确认矫正夹！" << endl;
		goto OutPoint;
	}
	else
	{
		//OK_ss << "dx = " << deltaX << endl;
		//OK_ss << "dy = " << deltaY << endl;
		//OK_ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;

		OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
	}
#pragma endregion

#pragma region 利用圆周灰度获取部品角度
	cvtColor(img_decode, gray, CV_BGR2GRAY);

	if (isCanny != 0)
	{
		//Canny(gray, temp, para0, para1);
		//Mat tmp;
		//blur(temp, tmp, Size(isCanny, isCanny));
		//threshold(tmp, temp, 0, 255, THRESH_BINARY);
		//if (isGray)
		//{
		//	cvtColor(temp, output, CV_GRAY2BGR);
		//}
	}
	else
	{
		temp = gray.clone();
	}

	Ana_DefectDetection_2(temp,
		GoodPoint,
		center,
		r3,
		r2,
		th1,
		size,
		(r3 - r2) *GPR0);

	if (0 >= GoodPoint.size())
	{
		ERR_ss << "轮廓无效" << endl;
		goto OutPoint;
	}

	for (size_t j = 0; j < size; j++)
	{
		float s = sin(j * 360 / size * CV_PI / 180);
		float c = cos(j * 360 / size * CV_PI / 180);
		Point2f end = center + r3 * Point2f(s, -c);
		Point2f sta = center + r2 * Point2f(s, -c);
		if (GoodPoint[j])//BGR
		{
			//circle(output, end, 1, Scalar(255, 0, 0), 2);
			//circle(output, sta, 1, Scalar(255, 0, 0), 2);

			line(output, end, end, Scalar(0, 255, 0), 6);
			line(output, sta, sta, Scalar(0, 255, 0), 6);
		}
		else
		{
			//circle(output, end, 1, Scalar(0, 0, 255), 2);
			//circle(output, sta, 1, Scalar(0, 0, 255), 2);

			line(output, end, end, Scalar(0, 0, 255), 6);
			line(output, sta, sta, Scalar(0, 0, 255), 6);
		}
	}
	
	if (angL != 0)
	{
		angle = Ana_FindLen(GoodPoint, size, angL);

		//if (Ana_LenFilter(GoodPoint, angL) != angN)
		//{
		//	ERR_ss << "缺口量异常，请确认实物是否有异常" << endl;
		//	goto OutPoint;
		//}
	}
	else
	{
		ang = Ana_FindMaxLen(GoodPoint);
		angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
	}
	

#pragma region 第二次获取角度
#if 1
	if (3 == _para0.size())
	{
		vector<int> angex;
		angex.push_back(stoi(_para0[0]));
		angex.push_back(stoi(_para0[1]));
		angex.push_back(stoi(_para0[2]));

		line(output,
			center,
			Point(center.x + ell.size.width / 2 * sin(angle * CV_PI / 180.0),
				center.y - ell.size.width / 2 * cos(angle * CV_PI / 180.0)),
			Scalar(0, 0, 255),
			4);

		for (int i = 0; i < 3; i++)
		{
			int aaa = angle;
			Mat mask = Mat::zeros(nHeight, nWidth, CV_8UC1);

			float s = sin((aaa + angex[i]) * 360 / size * CV_PI / 180);
			float c = cos((aaa + angex[i]) * 360 / size * CV_PI / 180);

			//float s1 = sin((aaa + angex[i] - 20) * 360 / size * CV_PI / 180);
			//float c1 = cos((aaa + angex[i] - 20) * 360 / size * CV_PI / 180);

			//float s2 = sin((aaa + angex[i] + 20) * 360 / size * CV_PI / 180);
			//float c2 = cos((aaa + angex[i] + 20) * 360 / size * CV_PI / 180);

			//Point center = center;
			ellipse(mask, center, Size(r3, r3), -90, aaa + angex[i] - para1 / 2, aaa + angex[i] + para1 / 2, Scalar(255), -1);
			ellipse(mask, center, Size(r2, r2), -90, aaa + angex[i] - para1 / 2, aaa + angex[i] + para1 / 2, Scalar(0), -1);

			Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
			Mat maskEx;
			morphologyEx(mask, maskEx, CV_MOP_OPEN, element);

			CONTOURS _contours;
			vector<Vec4i> _hierarchy;

			findContours(maskEx, _contours, _hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

			//int size[2] = {0,0};
			//for (int j = 0; j < _contours.size(); j++)
			//{
			//	if (size[0] < _contours[j].size())
			//	{
			//		size[0] = _contours[j].size();
			//		size[1] = j;
			//	}
			//}

			drawContours(output, _contours, 0, Scalar(255, 0, 255), 3);

			_avg.push_back(mean(temp, mask)[0]);
		}

		for (int i = 0; i < 3; i++)
		{
			float res = (_avg[i] - _avg[(i + 1) % 3]) / (_avg[i] + _avg[(i + 1) % 3]);
			res = abs(res);

			_res.push_back(res);
		}


		if (_res[0] <= _res[1] && _res[0] <= _res[2])
		{
			a = 2;
		}
		else if (_res[1] <= _res[0] && _res[1] <= _res[2])
		{
			a = 0;
		}
		else
		{
			a = 1;
		}
		angle += angex[a];

		//for (int i = 0; i < 3; i++)
		//{
		//	OK_ss << "区域" << i << "平均灰度：" << _avg[i] << endl;
		//}

		//for (int i = 0; i < 3; i++)
		//{
		//	OK_ss << "区域" << i << "&区域" << (i + 1) % 3 << " 对比度" << _res[i] << endl;
		//}
	}
	
#endif // 1
#pragma endregion

	angle += 360;
	angle = (float)(((int)angle) % 360);

	OK_ss << "角度：" << angle << endl;
	ERR_ss << "角度：" << angle << endl;

	line(output,
		center,
		Point(center.x + ell.size.width / 2 * sin(angle * CV_PI / 180.0),
			center.y - ell.size.width / 2 * cos(angle * CV_PI / 180.0)),
		Scalar(255, 0, 0),
		4);
#pragma endregion	

#pragma region 利用圆周灰度进行面别匹配
	for (size_t i = 0; i < CN; i++)
	{
		GoodPoint.clear(); //清空结果列表

		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			r5[i],
			r4[i],
			th2[i],
			size,
			(r5[i] - r4[i])*GPR1[i]);

		if (th2[i] < 0)
		{
			gray = 255 - gray;
		}

		for (size_t j = 0; j < size; j++)
		{
			float s = sin(j * 360 / size * CV_PI / 180);
			float c = cos(j * 360 / size * CV_PI / 180);
			Point2f end = center + r5[i] * Point2f(s, -c);
			Point2f sta = center + r4[i] * Point2f(s, -c);
			if (GoodPoint[j])//BGR
			{
				//circle(output, end, 1, Scalar(0, 255, 0), 2);
				//circle(output, sta, 1, Scalar(0, 255, 0), 2);
				line(output, end, end, Scalar(0, 255, 0), 6);
				line(output, sta, sta, Scalar(0, 255, 0), 6);
			}
			else
			{
				//circle(output, end, 1, Scalar(0, 0, 255), 2);
				//circle(output, sta, 1, Scalar(0, 0, 255), 2);
				line(output, end, end, Scalar(0, 0, 255), 6);
				line(output, sta, sta, Scalar(0, 0, 255), 6);
			}
		}
		//////////////////////
		float sum = 0;
		for (size_t j = 0; j < GoodPoint.size(); j++)
		{
			sum += GoodPoint[j];
		}
		sum /= GoodPoint.size();

		if (sum >= GPR2[i])
		{
			OK_ss << "圆环" << i << "匹配度:" << sum << endl;
			ERR_ss << "圆环" << i << "匹配度:" << sum << endl;
		}
		else
		{
			ERR_ss << "圆环" << i << "匹配度:" << sum << endl;

			LPSTR ErrOutput = (LPSTR)malloc(sizeof(char) * 1024);
			stringstream _ErrCode;
			_ErrCode << "Err" << errcode[i];
			GetPrivateProfileString("Main", _ErrCode.str().c_str(), "未知错误", ErrOutput, 1024, "./2.ini");


			ERR_ss << "异常可能为：" << ErrOutput << endl;

			free(ErrOutput);

			OK_ss << " " << endl;
			goto OutPoint;
		}
	}
#pragma endregion
	
#pragma region 函数出口
	result = 0;
	OutPoint:	
#pragma endregion
	
#pragma region 文字输出
			//字体大小
			int text_Size;
			text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
			//位置
			Point text_Localtion01;
			text_Localtion01.x = text_Size / 3;
			text_Localtion01.y = text_Size / 3;
			Point text_Localtion02;
			text_Localtion02.x = text_Size / 3;
			text_Localtion02.y = nHeight - text_Size * 4;
			Point text_Localtion03;
			text_Localtion03.x = text_Size / 3;
			text_Localtion03.y = nHeight - text_Size * 3;

			ERR_ss <<"\r"<< endl;
			OK_ss <<"\r"<< endl;

			std::string test;
			if (result != 0)
			{
				test = ERR_ss.str();
				putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
			}
			else
			{
				test = OK_ss.str();
				putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
			}
#pragma endregion
			
#pragma region 图像转换
			data.Height = output.rows;
			data.Width = output.cols;

			std::vector<uchar>buf;
			cv::imencode(".bmp", output, buf);
			data.size = buf.size() - 54;
			data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
			std::copy(buf.begin() + 54, buf.end(), data.data_Output);

			if (SaveImgNum > 0 & SaveImgNum < 100)
			{
				stringstream src_path;
				std::vector<String> filenames;
				src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
				cv::glob(src_path.str(), filenames);//获取路径下所有文件名
				if (filenames.size() >= SaveImgNum)
				{
					for (int i = 0; i < filenames.size(); i++)
					{
						remove(filenames[i].c_str());
					}
				}
				cv::glob(src_path.str(), filenames);//获取路径下所有文件名
				stringstream picfile;
				//SYSTEMTIME st;
				//GetLocalTime(&st);
				picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
				imwrite(picfile.str(), output);
			}
#pragma endregion	
			
#pragma region 不良图片保存
			if (result)
			{
				stringstream path;
#pragma region NG-Input图片输出
				//判断是否存在最新输出文件夹
				path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG";
				if (!dirExists(path.str()))
				{
					CreateDirectory(path.str().c_str(), NULL);
				}
				path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
				if (!dirExists(path.str()))
				{
					CreateDirectory(path.str().c_str(), NULL);
				}
				//生成NG的输出图片
				path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
				imwrite(path.str(), img);
#pragma endregion

#pragma region NG-Output图片输出
				path.str("");
				//判断是否存在最新输出文件夹
				path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG";
				if (!dirExists(path.str()))
				{
					CreateDirectory(path.str().c_str(), NULL);
				}
				path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
				if (!dirExists(path.str()))
				{
					CreateDirectory(path.str().c_str(), NULL);
				}
				//生成NG的输出图片
				path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
				imwrite(path.str(), output);
#pragma endregion

#pragma region 删除N天之前的输出 N默认为7
				time_t NowTime = time(NULL);
				int cycle = GetPrivateProfileInt("Main", "cycle", 7, "./1.ini");
				NowTime -= 60 * 60 * 24 * cycle;
				struct tm p;
				localtime_s(&p, &NowTime);

				path.str("");
				path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
				if (dirExists(path.str()))
				{
					std::vector<String> filenames;
					cv::glob(path.str(), filenames);//获取路径下所有文件名
					for (int i = 0; i < filenames.size(); i++)
					{
						remove(filenames[i].c_str());
					}
					RemoveDirectory(path.str().c_str());
				}

				path.str("");
				path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
				if (dirExists(path.str()))
				{
					std::vector<String> filenames;
					cv::glob(path.str(), filenames);//获取路径下所有文件名
					for (int i = 0; i < filenames.size(); i++)
					{
						remove(filenames[i].c_str());
					}
					RemoveDirectory(path.str().c_str());
				}
#pragma endregion
			}
#pragma endregion

#pragma region 偏移量保存
	stringstream str;
	str << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\";
	if (dirExists(str.str()))
	{
		str << "Offset.csv";
		//CreateDirectory(path.str().c_str(), NULL);
		FILE *fp = fopen(str.str().c_str(), "a+");
		stringstream str_r;
		//str.clear();
		str_r << deltaX << "," << deltaY << endl;
		fputs(str_r.str().c_str(), fp);
		fclose(fp);
	}
#pragma endregion

#pragma region 参数输出
			tm.stop();
			//OutputX = 5;
			//OutputY = 5;
			//耗时
			*output_Parameter_Float = tm.getTimeMilli();
			output_Parameter_Float++;
			//角度
			*output_Parameter_Float = angle + HzAngle;
			output_Parameter_Float++;
			//正反
			*output_Parameter_Float = 0;
			output_Parameter_Float++;
			//中心差值
			*output_Parameter_Float = deltaX;
			output_Parameter_Float++;
			*output_Parameter_Float = deltaY;
			output_Parameter_Float++;
			//镜片结果
			*output_Parameter_Float = result;
			output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
			free(r4);
			free(r5);
			free(GPR1);
			free(GPR2);
			free(th2);
			free(errcode);
#pragma endregion

return 0;
}
//暂时不需重写
bool MV_JP(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	//Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img = img_Temp.clone();
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//Mat img = imread("E:\\MM\\一体机\\1111\\test.bmp", 0);
	//LPSTR path = (LPSTR)malloc(sizeof(char)*1024);
	//GetPrivateProfileString("Main", "Path","",path,1024,"./2.ini");
	//Mat img = imread(path, 0);

	SYSTEMTIME st;
	GetLocalTime(&st);

	bool isGray = (bool)atoi(input_Parameter[0]);	//灰度显示       0表示显示原图 1表示显示灰度图

	int r0 = atoi(input_Parameter[1]);	//定位-内环半径
	int r1 = atoi(input_Parameter[2]);	//定位-外环半径
	int th0 = atoi(input_Parameter[3]);	//定位-灰度阈值

	int isCanny = atoi(input_Parameter[4]);//角度-边缘显示 //0表示不启用 3表示启用
	int para0 = atoi(input_Parameter[5]);//角度-边缘参数0
	int para1 = atoi(input_Parameter[6]);	//角度-边缘参数1

	int r3 = atoi(input_Parameter[7]);		//角度-外环半径
	int r2 = atoi(input_Parameter[8]);	//角度-内环半径
	float GPR0 = atof(input_Parameter[9]);	//径向有效点比例
	//GPR0 /= 100;
	int th1 = atoi(input_Parameter[10]);	//角度-灰度阈值
	int angL = atoi(input_Parameter[11]);	//角度-缺口长度

	int CN = atoi(input_Parameter[12]);	//匹配-圆环数量

	int *r4 = (int*)malloc(sizeof(int)*CN);		//匹配-半径下限
	int *r5 = (int*)malloc(sizeof(int)*CN);		//匹配-半径上限
	float *GPR1 = (float*)malloc(sizeof(float)*CN);	//匹配-径向有效点比例
	float *GPR2 = (float*)malloc(sizeof(float)*CN);	//匹配-切向有效点比例
	int *th2 = (int*)malloc(sizeof(int)*CN);		//匹配-灰度阈值

	int *errcode = (int*)malloc(sizeof(int)*CN);		//匹配-错误代码

	vector<string> R4;
	vector<string> R5;
	vector<string> gpr1;
	vector<string> gpr2;
	vector<string> TH2;

	vector<string> ErrCode;

	R4 = Spilt(input_Parameter[13]);
	R5 = Spilt(input_Parameter[14]);
	gpr1 = Spilt(input_Parameter[15]);
	gpr2 = Spilt(input_Parameter[16]);
	TH2 = Spilt(input_Parameter[17]);
	ErrCode = Spilt(input_Parameter[24]);

	if (CN > 1 && R4.size() == CN && R5.size() == CN && gpr1.size() == CN && gpr2.size() == CN && TH2.size() == CN && ErrCode.size() == CN)
	{
		for (size_t i = 0; i < CN; i++)
		{
			r4[i] = stoi(R4[i]);
			r5[i] = stoi(R5[i]);
			GPR1[i] = stof(gpr1[i]);
			GPR2[i] = stof(gpr2[i]);
			th2[i] = stoi(TH2[i]);
			errcode[i] = stoi(ErrCode[i]);
		}
	}
	else
	{
		CN = 1;
		r4[0] = stoi(R4[0]);
		r5[0] = stoi(R5[0]);
		GPR1[0] = stof(gpr1[0]);
		GPR2[0] = stof(gpr2[0]);
		th2[0] = stoi(TH2[0]);
		errcode[0] = stoi(ErrCode[0]);
	}


	float BzCenterX = atof(input_Parameter[18]);	//补偿基准点X
	float BzCenterY = atof(input_Parameter[19]);	//补偿基准点Y

	float rule_X = atof(input_Parameter[20]);//比例尺
	float rule_Y = atof(input_Parameter[21]);//比例尺

	float HzAngle = atoi(input_Parameter[22]);//回正角度

	vector<string> SaveImg;
	int SaveImgNum = 0;
	int CameraNum = 0;
	int XTNum = 0;
	SaveImg = Spilt2(input_Parameter[23]);

	float radius;
	float Offset;
	float MaxOffset = 0.02;

	LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	if (SaveImg.size() == 3)
	{
		SaveImgNum = stoi(SaveImg[2]);
		CameraNum = stoi(SaveImg[0]);
		XTNum = stoi(SaveImg[1]);

		string str = "MaxOffset";
		str += SaveImg[0] + SaveImg[1];
		GetPrivateProfileString("Main", str.c_str() , "0.1", path, 1024, "./Offset.ini");
		OutputDebugString(str.c_str());
		MaxOffset = stof(path);
		
	}
	free(path);

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		//SYSTEMTIME st;
		//GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), img);
	}
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = 0;
	int size = 360;	//圆周切割段数 这里用360做定指扫描
	Vec2i ang;
	TickMeter tm;
	RotatedRect ell;
	CONTOURS contours;
	stringstream OK_ss;
	stringstream ERR_ss;
	Point2f center(0, 0);
	vector<bool> GoodPoint;
	vector<Vec4i> hierarchy;
	vector<Point2i> tmpCont;
	Mat gray, output, img_decode, temp;

	

	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "镜片算法" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "镜片算法" << endl;

	gray = img.clone();

	tm.start();
#pragma endregion

#pragma region 输出参数
	float deltaX = 0;
	float deltaY = 0;
	float angle = 0;
	bool result = 1; //1是ng 0是ok
#pragma endregion

#pragma region 图像预处理
	cvtColor(gray, img_decode, CV_GRAY2BGR);
	//这里需要增加一个待测图片ROI选取
	//如果增加了ROI区域 是否会由概率出现识别异常
	//先留计划

	Mat gray_Temp = gray.clone();
	blur(gray_Temp, gray, Size(3, 3));	//高斯滤波

	threshold(gray, gray, th0, 255, THRESH_BINARY);//二值化阈值
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
#pragma endregion

#pragma region 轮廓筛选
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea = contourArea(tmp);
		double tmpLen = arcLength(tmp, 0);
		if (tmpLen > 2 * CV_PI*r1 || tmpLen < 2 * CV_PI*r0) continue;
		if (tmpArea > CV_PI*r1*r1 || tmpArea < CV_PI*r0*r0) continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}
#pragma endregion

#pragma region 灰度显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();

	//这里需要增加一个以图像中心为远点的ROI边界绘制
	circle(output, Point2f(nWidth / 2, nHeight / 2), r0, Scalar(0, 255, 0), 2);
	circle(output, Point2f(nWidth / 2, nHeight / 2), r1, Scalar(0, 255, 0), 2);
#pragma endregion

#pragma region 轮廓判定
	if (MaxL < 0)
	{
		ERR_ss << "未定位到有效边缘" << endl;
		goto OutPoint;
	}
#pragma endregion

#pragma region 圆心确定
	convexHull(contours[MaxL], tmpCont);
	ell = fitEllipse(tmpCont);
	ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
	center = ell.center;
	deltaX = (center.x - BzCenterX)*rule_X;
	deltaY = (center.y - BzCenterY)*rule_Y;
	radius = ell.size.width / 2;

	Offset = sqrt(deltaX*deltaX + deltaY * deltaY);

	if (Offset > MaxOffset)
	{
		//ERR_ss << " " << endl;
		//ERR_ss << " " << endl;
		//ERR_ss << " " << endl;

		ERR_ss << "dx = " << deltaX << endl;
		ERR_ss << "dy = " << deltaY << endl;
		ERR_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
		ERR_ss << "偏移量过大！请确认矫正夹！" << endl;
		goto OutPoint;
	}
	else
	{
		//OK_ss << "dx = " << deltaX << endl;
		//OK_ss << "dy = " << deltaY << endl;
		//OK_ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;

		OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
	}
#pragma endregion

#pragma region 利用圆周灰度获取部品角度
	cvtColor(img_decode, gray, CV_BGR2GRAY);

	if (isCanny != 0)
	{
		Canny(gray, temp, para0, para1);
		Mat tmp;
		blur(temp, tmp, Size(isCanny, isCanny));
		threshold(tmp, temp, 0, 255, THRESH_BINARY);
		if (isGray)
		{
			cvtColor(temp, output, CV_GRAY2BGR);
		}
	}
	else
	{
		temp = gray.clone();
	}

	Ana_DefectDetection_2(temp,
		GoodPoint,
		center,
		r3,
		r2,
		th1,
		size,
		(r3 - r2) *GPR0);

	if (0 >= GoodPoint.size())
	{
		ERR_ss << "轮廓无效" << endl;
		goto OutPoint;
	}

	for (size_t j = 0; j < size; j++)
	{
		float s = sin(j * 360 / size * CV_PI / 180);
		float c = cos(j * 360 / size * CV_PI / 180);
		Point2f end = center + r3 * Point2f(s, -c);
		Point2f sta = center + r2 * Point2f(s, -c);
		if (GoodPoint[j])//BGR
		{
			//circle(output, end, 1, Scalar(0, 255, 0), 2);
			//circle(output, sta, 1, Scalar(0, 255, 0), 2);

			line(output, end, end, Scalar(0, 255, 0), 6);
			line(output, sta, sta, Scalar(0, 255, 0), 6);
		}
		else
		{
			//circle(output, end, 1, Scalar(0, 0, 255), 2);
			//circle(output, sta, 1, Scalar(0, 0, 255), 2);

			line(output, end, end, Scalar(0, 0, 255), 6);
			line(output, sta, sta, Scalar(0, 0, 255), 6);
		}
	}

	if (angL != 0)
	{
		angle = Ana_FindLen(GoodPoint, size, angL);
	}
	else
	{
		ang = Ana_FindMaxLen(GoodPoint);
		angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
	}

	angle += 360;
	angle = (float)(((int)angle) % 360);

	OK_ss << "角度：" << angle << endl;
	ERR_ss << "角度：" << angle << endl;

	line(output,
		center,
		Point(center.x + ell.size.width / 2 * sin(angle * CV_PI / 180.0),
			center.y - ell.size.width / 2 * cos(angle * CV_PI / 180.0)),
		Scalar(255, 0, 0),
		4);
#pragma endregion	

#pragma region 利用圆周灰度进行面别匹配
	for (size_t i = 0; i < CN; i++)
	{
		GoodPoint.clear(); //清空结果列表

		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			r5[i],
			r4[i],
			th2[i],
			size,
			(r5[i] - r4[i])*GPR1[i]);

		if (th2[i] < 0)
		{
			gray = 255 - gray;
		}

		for (size_t j = 0; j < size; j++)
		{
			float s = sin(j * 360 / size * CV_PI / 180);
			float c = cos(j * 360 / size * CV_PI / 180);
			Point2f end = center + r5[i] * Point2f(s, -c);
			Point2f sta = center + r4[i] * Point2f(s, -c);
			if (GoodPoint[j])//BGR
			{
				//circle(output, end, 1, Scalar(0, 255, 0), 2);
				//circle(output, sta, 1, Scalar(0, 255, 0), 2);

				line(output, end, end, Scalar(0, 255, 0), 6);
				line(output, sta, sta, Scalar(0, 255, 0), 6);
			}
			else
			{
				//circle(output, end, 1, Scalar(0, 0, 255), 2);
				//circle(output, sta, 1, Scalar(0, 0, 255), 2);

				line(output, end, end, Scalar(0, 0, 255), 6);
				line(output, sta, sta, Scalar(0, 0, 255), 6);
			}
		}
		//////////////////////
		float sum = 0;
		for (size_t j = 0; j < GoodPoint.size(); j++)
		{
			sum += GoodPoint[j];
		}
		sum /= GoodPoint.size();

		if (sum >= GPR2[i])
		{
			OK_ss << "圆环" << i << "匹配度:" << sum << endl;
			ERR_ss << "圆环" << i << "匹配度:" << sum << endl;
		}
		else
		{
			ERR_ss << "圆环" << i << "匹配度:" << sum << endl;

			LPSTR ErrOutput = (LPSTR)malloc(sizeof(char) * 1024);

			stringstream _ErrCode;
			_ErrCode << "Err" << errcode[i];
			GetPrivateProfileString("Main", _ErrCode.str().c_str(), "未知错误", ErrOutput, 1024, "./2.ini");
			
			ERR_ss <<"异常可能为："<< ErrOutput << endl;

			free(ErrOutput);


			//FILE *pFile(NULL);
			//_wfopen_s(&pFile, (wchar_t*)"./2.ini", L"wt, ccs=UTF-16LE");

			//fclose(pFile);

			OK_ss << " " << endl;
			OK_ss << " " << endl;
			goto OutPoint;
		}
	}
#pragma endregion

#pragma region 函数出口
	result = 0;
OutPoint:
#pragma endregion

#pragma region 文字输出
	//字体大小
	int text_Size;
	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
	//位置
	Point text_Localtion01;
	text_Localtion01.x = text_Size / 3;
	text_Localtion01.y = text_Size / 3;
	Point text_Localtion02;
	text_Localtion02.x = text_Size / 3;
	text_Localtion02.y = nHeight - text_Size * 4;
	Point text_Localtion03;
	text_Localtion03.x = text_Size / 3;
	text_Localtion03.y = nHeight - text_Size * 3;

	ERR_ss << "\r" << endl;
	OK_ss << "\r" << endl;

	std::string test;
	if (result != 0)
	{
		test = ERR_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	}
	else
	{
		test = OK_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
	}
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), output);
	}
#pragma endregion		

#pragma region 不良镜片保存
	if (result)
	{
		stringstream path;
#pragma region NG-Input图片输出
		//判断是否存在最新输出文件夹
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), img);
#pragma endregion

#pragma region NG-Output图片输出
		path.str("");
		//判断是否存在最新输出文件夹
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), output);
#pragma endregion

#pragma region 删除N天之前的输出 N默认为7
		time_t NowTime = time(NULL);
		int cycle = GetPrivateProfileInt("Main", "cycle", 7, "./1.ini");
		NowTime -= 60 * 60 * 24 * cycle;
		struct tm p;
		localtime_s(&p, &NowTime);

		path.str("");
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
		if (dirExists(path.str()))
		{
			std::vector<String> filenames;
			cv::glob(path.str(), filenames);//获取路径下所有文件名
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
			RemoveDirectory(path.str().c_str());
		}

		path.str("");
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
		if (dirExists(path.str()))
		{
			std::vector<String> filenames;
			cv::glob(path.str(), filenames);//获取路径下所有文件名
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
			RemoveDirectory(path.str().c_str());
		}
#pragma endregion
	}
#pragma endregion

#pragma region 偏移量保存
	stringstream str;
	str << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\";
	if (dirExists(str.str()))
	{
		str << "Offset.csv";
		//CreateDirectory(path.str().c_str(), NULL);
		FILE *fp = fopen(str.str().c_str(), "a+");
		stringstream str_r;
		//str.clear();
		str_r << deltaX << "," << deltaY << endl;
		fputs(str_r.str().c_str(), fp);
		fclose(fp);
	}
#pragma endregion

#pragma region 参数输出

	//OutputX = 5;
	//OutputY = 5;
	tm.stop();
	//耗时
	*output_Parameter_Float = tm.getTimeMilli();
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = angle + HzAngle;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = deltaX;
	output_Parameter_Float++;
	*output_Parameter_Float = deltaY;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = result;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	free(r4);
	free(r5);
	free(GPR1);
	free(GPR2);
	free(th2);
	free(errcode);
#pragma endregion
	return 0;
}

bool MV_DGP(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	//Mat img_Temp = imread("C:\\项目\\702.bmp", 0);
	//Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img = img_Temp.clone();
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	//GetPrivateProfileString("Main", "Path", "", path, 1024, "./2.ini");
	//Mat img = imread(path, 0);

	SYSTEMTIME st;
	GetLocalTime(&st);

	bool isGray = (bool)atoi(input_Parameter[0]);	//灰度显示 0表示显示原图 1表示显示灰度图

	int r0 = atoi(input_Parameter[1]);		//定位-内环半径
	int r1 = atoi(input_Parameter[2]);		//定位-外环半径
	int th0 = atoi(input_Parameter[3]);	//定位-灰度阈值

	int NoUse0		= 0;//占位但是不用，界面上也不需要显示
	int QKLength	= atoi(input_Parameter[4]);	//缺口-长度下限
	int resultNum	= atoi(input_Parameter[5]);	//缺口-缺口数量
	int NoUse1		= 0;//占位但是不用，界面上也不需要显示

	int r2 = atoi(input_Parameter[6]);		//缺口-外环半径
	int r3 = atoi(input_Parameter[7]);			//缺口-内环半径
	float GPR0 = atof(input_Parameter[8]);	//缺口-径向有效点比例
	//GPR0 /= 100;
	int th1 = atoi(input_Parameter[9]);		//缺口-灰度阈值

	vector<string> SaveImg;
	int SaveImgNum = 0;
	int CameraNum = 0;
	int XTNum = 0;
	SaveImg = Spilt2(input_Parameter[10]);
	if (SaveImg.size() == 3)
	{
		SaveImgNum = stoi(SaveImg[2]);
		CameraNum = stoi(SaveImg[0]);
		XTNum = stoi(SaveImg[1]);
	}

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		//SYSTEMTIME st;
		//GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), img);
	}
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = 0;
	//Vec2i ang;
	float size = 360;		//圆周切割段数 这里用360做定指扫描
	TickMeter tm;
	RotatedRect ell;
	CONTOURS contours;
	stringstream OK_ss;
	stringstream ERR_ss;
	Point2f center(0, 0);
	vector<bool> GoodPoint;
	vector<Vec4i> hierarchy;
	vector<Point2i> tmpCont;
	Mat gray, output, img_decode, temp;

	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "挡光片算法" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "挡光片算法" << endl;

	gray = img.clone();
	
	tm.start();
#pragma endregion

#pragma region 输出参数
	//int deltaX = 0;
	//int deltaY = 0;
	//float angle = 0;
	int num = 0;
	bool result = 1; //1是ng 0是ok
#pragma endregion

#pragma region 图像预处理
	cvtColor(gray, img_decode, CV_GRAY2BGR);

	Mat gray_Temp = gray.clone();
	blur(gray_Temp, gray, Size(3, 3));	//高斯滤波

	threshold(gray, gray, th0, 255, THRESH_BINARY);//二值化阈值
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
#pragma endregion

#pragma region 轮廓筛选
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea = contourArea(tmp);
		double tmpLen = arcLength(tmp, 0);

		if (tmpLen > 2 * CV_PI*r1 || tmpLen < 2 * CV_PI*r0) continue;
		if (tmpArea > CV_PI*r1*r1 || tmpArea < CV_PI*r0*r0) continue;

		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}

	
#pragma endregion

#pragma region 灰度显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();

	if (MaxL < 0)
		goto OutPoint;
#pragma endregion

#pragma region 圆心确定
	float radius;

	convexHull(contours[MaxL], tmpCont);
	ell = fitEllipse(tmpCont);
	ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
	center = ell.center;
	//deltaX = center.x - BzCenterX;
	//deltaY = center.y - BzCenterY;
	radius = ell.size.width / 2;

	OK_ss << "半径：" << radius << endl;
	//OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

	ERR_ss << "半径：" << radius << endl;
	//ERR_ss << " " << endl;

	//这里需要增加一个以图像中心为远点的ROI边界绘制
	circle(output, center, r0, Scalar(0, 255, 0), 2);
	circle(output, center, r1, Scalar(0, 255, 0), 2);
#pragma endregion

#pragma region 轮廓判定
	if (MaxL < 0)
	{
		ERR_ss << "部品定位异常" << endl;
		goto OutPoint;
	}
#pragma endregion

#pragma region 缺口查找
	Ana_DefectDetection_2(img, GoodPoint, center, r2, r3, th1, size, (r2 - r3) * GPR0);

	for (size_t i = 0; i < size; i++)
	{
		float s = sin(i * 360 / size * CV_PI / 180);
		float c = cos(i * 360 / size * CV_PI / 180);
		Point2f end = center + r3 * Point2f(s, -c);
		Point2f sta = center + r2 * Point2f(s, -c);
		if (GoodPoint[i])//BG
		{
			circle(output, end, 1, Scalar(255, 0, 0), 2);
			circle(output, sta, 1, Scalar(255, 0, 0), 2);
		}
		else
		{
			line(output, end, sta, Scalar(0, 0, 255), 2);
			circle(output, end, 1, Scalar(0, 0, 255), 2);
			circle(output, sta, 1, Scalar(0, 0, 255), 2);
		}
	}

	//Vec2i len = Ana_FindMaxLen(GoodPoint);
	//if (len[0] < 0)len[0] += size;
	//int angle = len[0] - len[1];

	num = Ana_LenFilter(GoodPoint, QKLength);
	if (num != resultNum)
	{
		ERR_ss << "缺口数目不匹配：" << num << endl;
		goto OutPoint;
	}

	result = true;
	OK_ss << "缺口个数：" << num << endl;

	//Para.deltaX = center.x - output.cols / 2;
	//Para.deltaY = center.y - output.rows / 2;
#pragma endregion

#pragma region 函数出口
	result = 0;
OutPoint:
#pragma endregion

#pragma region 文字输出
	//字体大小
	int text_Size;
	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
	//位置
	Point text_Localtion01;
	text_Localtion01.x = text_Size / 3;
	text_Localtion01.y = text_Size / 3;
	Point text_Localtion02;
	text_Localtion02.x = text_Size / 3;
	text_Localtion02.y = nHeight - text_Size * 4;
	Point text_Localtion03;
	text_Localtion03.x = text_Size / 3;
	text_Localtion03.y = nHeight - text_Size * 3;

	ERR_ss << "\r" << endl;
	OK_ss << "\r" << endl;

	std::string test;
	if (result != 0)
	{
		test = ERR_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	}
	else
	{
		test = OK_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
	}
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), output);
	}
#pragma endregion		

#pragma region 不良镜片保存
	if (result)
	{
		stringstream path;
#pragma region NG-Input图片输出
		//判断是否存在最新输出文件夹
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), img);
#pragma endregion

#pragma region NG-Output图片输出
		path.str("");
		//判断是否存在最新输出文件夹
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), output);
#pragma endregion

#pragma region 删除N天之前的输出 N默认为7
		time_t NowTime = time(NULL);
		int cycle = GetPrivateProfileInt("Main", "cycle", 7, "./1.ini");
		NowTime -= 60 * 60 * 24 * cycle;
		struct tm p;
		localtime_s(&p, &NowTime);

		path.str("");
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
		if (dirExists(path.str()))
		{
			std::vector<String> filenames;
			cv::glob(path.str(), filenames);//获取路径下所有文件名
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
			RemoveDirectory(path.str().c_str());
		}

		path.str("");
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
		if (dirExists(path.str()))
		{
			std::vector<String> filenames;
			cv::glob(path.str(), filenames);//获取路径下所有文件名
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
			RemoveDirectory(path.str().c_str());
		}
#pragma endregion
	}
#pragma endregion

#pragma region 参数输出
	
	//OutputX = 5;
	//OutputY = 5;
	//
	tm.stop();
	//耗时
	*output_Parameter_Float = tm.getTimeMilli();
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = result;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	//free(r4);
	//free(r5);
	//free(GPR1);
	//free(GPR2);
	//free(th2);
#pragma endregion

	return 0;
}

bool MV_JS(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	//Mat img_decode_Temp = imread("C:\\项目\\JS01.BMP", 3);
	//Mat img_decode_Temp(nHeight, nWidth, CV_8UC3, data.data_Input);//默认判胶相机使用的是彩色相机
	//Mat img_decode = img_decode_Temp.clone();//默认判胶相机使用的是彩色相机
	Mat img_decode(nHeight, nWidth, CV_8UC3, data.data_Input);//默认判胶相机使用的是彩色相机

	//LPSTR path = (LPSTR)malloc(sizeof(char)*1024);
	//GetPrivateProfileString("Main", "Path","",path,1024,"./3.ini");
	//Mat img_decode = imread("E:\\MM\\一体机\\1111\\JS01.bmp", 1);


	SYSTEMTIME st;
	GetLocalTime(&st);

	TickMeter tm;
	tm.start();
	bool isGray = atoi(input_Parameter[0]);//灰度显示

	int r1 = atoi(input_Parameter[1]);//定位-轮廓下限
	int r0 = atoi(input_Parameter[2]);//定位-轮廓上限
	int th0 = atoi(input_Parameter[3]);//定位-灰度阈值

	int th1 = atoi(input_Parameter[4]);//判胶-灰度阈值
	float R11 = atof(input_Parameter[5]);//判胶-R通道增益
	float B11 = atof(input_Parameter[6]);//判胶-B通道增益
	float G11 = atof(input_Parameter[7]);//判胶-G通道增益
	float Gain = atof(input_Parameter[8]);//判胶-总增益

	int r2 = atoi(input_Parameter[9]);//判胶-胶水检测内环
	int r3 = atoi(input_Parameter[10]);//判胶-胶水检测外环

	int r4 = atoi(input_Parameter[11]);//判胶-欠胶检测外径

	int r5 = atoi(input_Parameter[12]);//判胶-向外溢胶内环
	int r6 = atoi(input_Parameter[13]);//判胶-向外溢胶外环

	int r7 = atoi(input_Parameter[14]);//判胶-向内溢胶内环
	int r8 = atoi(input_Parameter[15]);//判胶-向内溢胶外环

	int th2 = atoi(input_Parameter[16]);//判胶-欠胶环最小胶量
	int th3 = atoi(input_Parameter[17]);//判胶-胶水环最小胶水量

	int th4 = atoi(input_Parameter[18]);//判胶-溢胶限度

	int num = atoi(input_Parameter[19]);//判胶-胶水段数
	int len = atoi(input_Parameter[20]);//判胶-胶水长度 默认值0

	vector<string> SaveImg;
	int SaveImgNum = 0;
	int CameraNum = 0;
	int XTNum = 0;
	SaveImg = Spilt2(input_Parameter[21]);
	if (SaveImg.size() == 3)
	{
		SaveImgNum = stoi(SaveImg[2]);
		CameraNum = stoi(SaveImg[0]);
		XTNum = stoi(SaveImg[1]);
	}
	
	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), img_decode);
	}

#pragma endregion

#pragma region 本地参数
	float iii = 0;
	int flag = 0;
	int MaxL = -1;
	Mat gray, output, green;
	Mat RGB[3];
	long area = 0;
	CONTOURS contours;
	vector<Vec4i> hierarchy;
	Point2f center(0, 0);
	vector<Point2i> tmpCont;
	RotatedRect ell;

	vector<bool> OutContour[4];

	vector<bool> test;
	float size = 360;

	stringstream OK_ss, ERR_ss;

	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "胶水检测" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "胶水检测" << endl;
#pragma endregion

#pragma region 输出参数
	//int deltaX = 0;
	//int deltaY = 0;
	//float angle = 0;
	//int num = 0;
	bool result = 1; //1是ng 0是ok
#pragma endregion

#pragma region 第一次合成
	//cv::split(img_decode, RGB);

	//gray = R11 * RGB[0] + G11 * RGB[1] + B11 * RGB[2];
	//直接利用灰度图进行定位，没必要合成
	cvtColor(img_decode, gray, CV_BGR2GRAY);

	Mat grayTemp = gray.clone();
	blur(grayTemp, gray, Size(3, 3));
	threshold(gray, gray, th0, 255, THRESH_BINARY);//二值化阈值
#pragma endregion

#pragma region 灰度显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
#pragma endregion

#pragma region 轮廓筛选
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea = contourArea(tmp);
		double tmpLen = arcLength(tmp, 0);
		if (tmpLen > 2 * CV_PI*r0 || tmpLen < 2 * CV_PI*r1) continue;
		if (tmpArea > CV_PI*r0*r0 || tmpArea < CV_PI*r1*r1) continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}

	if (MaxL < 0)
	{
		ERR_ss << "无有效目标" << endl;
		goto OutPoint;
	}

	if (30 > contours[MaxL].size())
	{
		ERR_ss << "轮廓无效" << endl;
		goto OutPoint;
	}
#pragma endregion

#pragma region 获取圆心及半径
	float radius;
	
	convexHull(contours[MaxL], tmpCont);
	ell = fitEllipse(tmpCont);
	ellipse(output, ell, Scalar(255, 0, 0), 2);
	center = ell.center;
	//Para.deltaX = (int)center.x;
	//Para.deltaY = (int)center.y;

	radius = ell.size.width / 2;
	OK_ss << "半径："<< radius << endl;
	//OK_ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
	//OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

	//ERR_ss << " " << endl;
	ERR_ss << "半径：" << radius << endl;

	circle(output, center, r0, Scalar(0, 255, 0), 1);
	circle(output, center, r1, Scalar(0, 255, 0), 1);
	//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 第二次合成
	//调试使用需求，th1<0表示不做胶水量判别
	if (0 > th1)goto OutPoint;

	cv::split(img_decode, RGB);

	green = RGB[1].clone();
	//green = G11 * RGB[1].clone();
	////green = G22 * RGB[1].clone();
	//grayTemp = green.clone();
	//blur(grayTemp, green, Size(3, 3));
	//threshold(green, green, th1, 255, THRESH_BINARY);//二值化阈值


	//gray = R22 * RGB[0] + G22 * RGB[1] + B22 * RGB[2];
	gray = R11 * RGB[0] + G11 * RGB[1] + B11 * RGB[2];
	gray *= Gain;
	grayTemp = gray.clone();
	blur(grayTemp, gray, Size(3, 3));
	threshold(gray, gray, th1, 255, THRESH_BINARY);//二值化阈值
#pragma endregion
	
#pragma region 灰度显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
#pragma endregion

#pragma region 胶水识别
	radius = r2;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	radius = r3;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	radius = r4;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	radius = r5;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	radius = r6;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	radius = r7;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	radius = r8;
	circle(output, center, radius, Scalar(0, 255, 255), 1);

	////胶环检测
	//Ana_DefectDetection_2(green, OutContour[0], center, r3, r2, 0, size, th3);

	////欠胶检测
	//Ana_DefectDetection_2(green, OutContour[1], center, r4, r2, 0, size, th2);

	//////////////////////////-2021/5/7 TLH-///////////////////////////////////
	//胶环检测
	Ana_DefectDetection_2(green, OutContour[0], center, r3, r2, th1*G11, size, th3);

	//欠胶检测
	Ana_DefectDetection_2(green, OutContour[1], center, r4, r2, th1, size, th2);
	///////////////////////////////////////////////////////////////////////////

	//外环溢胶检测
	Ana_DefectDetection_2(gray, OutContour[2], center, r6, r5, 0, size, th4);

	//内环溢胶检测
	Ana_DefectDetection_2(gray, OutContour[3], center, r8, r7, 0, size, th4);

	//欠胶检测
	for (int i = 0; i < size; i++)
	{
		if (OutContour[0][i] && OutContour[1][i])
		{
			test.push_back(0);

			float s = sin(i*CV_PI / 180);
			float c = cos(i*CV_PI / 180);

			Point2f sta = center + r2 * Point2f(s, -c);
			Point2f end = center + r3 * Point2f(s, -c);

			line(output, sta, end, Scalar(255, 255, 0), 1);
		}
		else
			test.push_back(1);
	}

	iii = 0;
	for (size_t i = 0; i < test.size(); i++)
	{
		if (!test[i])iii++;
	}
	//iii /= test.size();
	//iii *= size / len;
	//if (iii < 1)
	if (iii / test.size() < len/size)
	{
		ERR_ss << "胶水量异常" << endl;
		ERR_ss << "目标胶水量：" << len / size << endl;
		ERR_ss << "实际胶水量：" << iii / test.size() << endl;
		ERR_ss << "请检查胶水量是否足够" << endl;
		goto OutPoint;
	}
	OK_ss << "胶水比例:" << iii / test.size() << "/" << len / size << num << endl;
	ERR_ss << "胶水比例:" << iii / test.size() << "/" << len / size << num << endl;

	flag = GetPrivateProfileInt("Main", "JSM", 0, "./1.ini");
	if (!flag)
	{
		if (num != Ana_LenFilter(test, len))
		{
			ERR_ss << "有效胶水段数：" << Ana_LenFilter(test, len) << endl;
			ERR_ss << "请检查胶水量是否足够" << endl;
			goto OutPoint;
		}
		OK_ss << "有效胶水数:" << num << endl;
		ERR_ss << "有效胶水数:" << num << endl;
	}
	else
	{
		iii = 0;
		for (size_t i = 0; i < test.size(); i++)
		{
			if (OutContour[2][i])iii++;
		}
		iii /= test.size();

		if (iii >= (num - 1)*len / size)
		{
			OK_ss << "胶水量足够" << endl;
			ERR_ss << "胶水量足够" << endl;
		}
		else
		{
			ERR_ss << "胶水量欠缺" << endl;
			ERR_ss << iii << "//" << (num - 1)*len / size << endl;
			goto OutPoint;
		}
	}

	test.clear();
	for (int i = 0; i < size; i++)
	{
		if (OutContour[2][i] && OutContour[0][i])
		{
			test.push_back(0);

			float s = sin(i*CV_PI / 180);
			float c = cos(i*CV_PI / 180);

			Point2f sta = center + r6 * Point2f(s, -c);
			Point2f end = center + r5 * Point2f(s, -c);

			line(output, sta, end, Scalar(255, 255, 0), 1);
		}
		else
			test.push_back(1);
	}

	if (Ana_LenFilter(test, 1))
	{
		ERR_ss << "外环有溢胶" << endl;
		goto OutPoint;
	}
	else
	{
		OK_ss << "外环点胶正常" << endl;
		ERR_ss << "外环点胶正常" << endl;
	}
		
	test.clear();
	for (int i = 0; i < size; i++)
	{
		if (OutContour[3][i] && OutContour[0][i])
		{
			test.push_back(0);

			float s = sin(i*CV_PI / 180);
			float c = cos(i*CV_PI / 180);

			Point2f sta = center + r8 * Point2f(s, -c);
			Point2f end = center + r7 * Point2f(s, -c);

			line(output, sta, end, Scalar(255, 255, 0), 1);
		}
		else
			test.push_back(1);
	}
	if (Ana_LenFilter(test, 1))
	{
		ERR_ss << "内环有溢胶" << endl;
		goto OutPoint;
	}
	else
	{
		OK_ss << "内环点胶正常" << endl;
		ERR_ss << "内环点胶正常" << endl;
	}
	result = 0;
#pragma endregion

#pragma region 函数出口
	result = 0;
OutPoint:
#pragma endregion

#pragma region 文字输出
	//字体大小
	int text_Size;
	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
	//位置
	Point text_Localtion01;
	text_Localtion01.x = text_Size / 3;
	text_Localtion01.y = text_Size / 3;
	Point text_Localtion02;
	text_Localtion02.x = text_Size / 3;
	text_Localtion02.y = nHeight - text_Size * 4;
	Point text_Localtion03;
	text_Localtion03.x = text_Size / 3;
	text_Localtion03.y = nHeight - text_Size * 3;

	//std::string text = ERR_ss.str();
	//putTextZH(output, text.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	//text = OK_ss.str();
	//putTextZH(output, text.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);

	std::string text;
	if (result != 0)
	{
		text = ERR_ss.str();
		putTextZH(output, text.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	}
	else
	{
		text = OK_ss.str();
		putTextZH(output, text.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
	}
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= SaveImgNum)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), output);
	}
#pragma endregion		

#pragma region 不良图片保存
	if (result)
	{
		stringstream path;
#pragma region NG-Input图片输出
		//判断是否存在最新输出文件夹
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), img_decode);
#pragma endregion

#pragma region NG-Output图片输出
		path.str("");
		//判断是否存在最新输出文件夹
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), output);
#pragma endregion

#pragma region 删除N天之前的输出 N默认为7
		time_t NowTime = time(NULL);
		int cycle = GetPrivateProfileInt("Main", "cycle", 7, "./1.ini");
		NowTime -= 60 * 60 * 24 * cycle;
		struct tm p;
		localtime_s(&p, &NowTime);

		path.str("");
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
		if (dirExists(path.str()))
		{
			std::vector<String> filenames;
			cv::glob(path.str(), filenames);//获取路径下所有文件名
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
			RemoveDirectory(path.str().c_str());
		}

		path.str("");
		path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input" << "\\NG" << "\\" << p.tm_year + 1900 << "-" << setfill('0') << setw(2) << p.tm_mon + 1 << "-" << setfill('0') << setw(2) << p.tm_mday;
		if (dirExists(path.str()))
		{
			std::vector<String> filenames;
			cv::glob(path.str(), filenames);//获取路径下所有文件名
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
			RemoveDirectory(path.str().c_str());
		}
#pragma endregion
	}
#pragma endregion

#pragma region 参数输出
	tm.stop();
	//OutputX = 5;
	//OutputY = 5;
	//耗时
	*output_Parameter_Float = tm.getTimeMilli();
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = result;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	//free(r4);
	//free(r5);
	//free(GPR1);
	//free(GPR2);
	//free(th2);
#pragma endregion
	return 0;
}

bool MV_MTJS(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{

#pragma region 本地参数定义
	Mat img_decode(nHeight, nWidth, CV_8UC3, data.data_Input);//默认判胶相机使用的是彩色相机
#pragma endregion

#pragma region 输入参数设置
	//增加四位参数用于图像切割
	float x1 = atof(input_Parameter[22]);//左上角X
	float y1 = atof(input_Parameter[23]);//左上角Y

	float x2 = atof(input_Parameter[24]);//右下角X
	float y2 = atof(input_Parameter[25]);//右下角Y
#pragma endregion

#pragma region 整张图像保存
	vector<string> SaveImg;
	int SaveImgNum = 0;
	int CameraNum = 0;
	int XTNum = 0;
	SaveImg = Spilt2(input_Parameter[21]);
	if (SaveImg.size() == 3)
	{
		SaveImgNum = stoi(SaveImg[2]);
		CameraNum = stoi(SaveImg[0]);
		XTNum = stoi(SaveImg[1]);
	}

	if (SaveImgNum > 0 & SaveImgNum < 100)
	{
		stringstream src_path;
		std::vector<String> filenames;
		src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		if (filenames.size() >= (SaveImgNum % 25) + SaveImgNum + 1)
		{
			for (int i = 0; i < filenames.size(); i++)
			{
				remove(filenames[i].c_str());
			}
		}
		cv::glob(src_path.str(), filenames);//获取路径下所有文件名
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\原始" << (filenames.size() + 1) << ".bmp";
		imwrite(picfile.str(), img_decode);
	}
#pragma endregion

#pragma region 图像切割并计算
	for (int i = 0; i < 25; i++)
	{
		int x = i % 25;
	}
#pragma endregion

#pragma region 输出本地图像

#pragma endregion

#pragma region 输出计算结果

#pragma endregion

	return 0;
}

bool MV_TP(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
	//#pragma region 参数载入
//	int sx = input_Parameter_Int[0];
//	int sy = input_Parameter_Int[1];
//	int ex = input_Parameter_Int[2];
//	int ey = input_Parameter_Int[3];
//	int angle = input_Parameter_Int[4];;//画面旋转角度
//	int threshold = input_Parameter_Int[5];;//灰度阈值
//	int MaxRadius = input_Parameter_Int[6];;//半径上限
//	int MinRadius = input_Parameter_Int[7];;//半径下限
//	int isGray = input_Parameter_Int[8];;//灰度显示
//	int isCap = input_Parameter_Int[9];;//是否截图
//#pragma endregion


	//*output_Parameter_Float = num;
	//	output_Parameter_Float++;
	return 0;
}

void MyGamma(Mat& src,Mat& dst,float fGamma)
{
	CV_Assert(src.data);

	CV_Assert(src.depth() != sizeof(uchar));

	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<char>(pow((float)(i / 255.0), fGamma)*255.0f);
	}

	dst = src.clone();
	const int channels = dst.channels();

	switch (channels)
	{
	case 1:
	{
		MatIterator_<uchar> it, end;
		for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
		{
			*it = lut[*it];
		}
	}
	case 3:
	{
		MatIterator_<Vec3b> it, end;
		for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
		{
			(*it)[0] = lut[(*it)[0]];
			(*it)[1] = lut[(*it)[1]];
			(*it)[2] = lut[(*it)[2]];
		}
	}
	}
}

bool tmp(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);

	//GetPrivateProfileString("Main", "path", "", path, 1024, "./MD.ini");
	//Mat img = imread(path, 0);
	//CV_Assert(img.data);

	Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	Mat img = img_Temp.clone();

	//Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//Mat img = img2.clone();
	//LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	//GetPrivateProfileString("Main", "Path", "", path, 1024, "./2.ini");
	//Mat img = imread(path, 0);

	SYSTEMTIME st;
	GetLocalTime(&st);

	//bool isGray = (bool)atoi(input_Parameter[0]);	//灰度显示 0表示显示原图 1表示显示灰度图

	//int r0 = atoi(input_Parameter[1]);		//定位-内环半径
	//int r1 = atoi(input_Parameter[2]);		//定位-外环半径
	//int th0 = atoi(input_Parameter[3]);	//定位-灰度阈值

	//int NoUse0 = 0;//占位但是不用，界面上也不需要显示
	//int QKLength = atoi(input_Parameter[4]);	//缺口-长度下限
	//int resultNum = atoi(input_Parameter[5]);	//缺口-缺口数量
	//int NoUse1 = 0;//占位但是不用，界面上也不需要显示

	//int r2 = atoi(input_Parameter[6]);		//缺口-外环半径
	//int r3 = atoi(input_Parameter[7]);			//缺口-内环半径
	//float GPR0 = atof(input_Parameter[8]);	//缺口-径向有效点比例
	////GPR0 /= 100;
	//int th1 = atoi(input_Parameter[9]);		//缺口-灰度阈值

	//vector<string> SaveImg;
	//int SaveImgNum = 0;
	//int CameraNum = 0;
	//int XTNum = 0;
	//SaveImg = Spilt2(input_Parameter[10]);
	//if (SaveImg.size() == 3)
	//{
	//	SaveImgNum = stoi(SaveImg[2]);
	//	CameraNum = stoi(SaveImg[0]);
	//	XTNum = stoi(SaveImg[1]);
	//}

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	//SYSTEMTIME st;
	//	//GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), img);
	//}
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = 0;
	//Vec2i ang;
	float size = 360;		//圆周切割段数 这里用360做定指扫描
	//TickMeter tm;
	RotatedRect ell;
	CONTOURS contours;
	stringstream OK_ss;
	//stringstream ERR_ss;
	Point2f center(0, 0);
	vector<bool> GoodPoint;
	vector<Vec4i> hierarchy;
	vector<Point2i> tmpCont;
	Mat gray, output, img_decode, temp;

	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "麻点检测" << endl;

	//ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	//ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	//ERR_ss << "麻点检测" << endl;

	gray = img.clone();

	int flag = 2;

	//tm.start();
#pragma endregion

#pragma region 输出参数
	//int deltaX = 0;
	//int deltaY = 0;
	//float angle = 0;
	int num = 0;
	bool result = 0; //1是ng 0是ok
#pragma endregion

#pragma region 麻点检测
	float sum0 = 0, sum1 = 0;
	int a = 0;
	vector<Point2f> Lpoint, Dpoint;
	float summ1 = 0, summ2 = 0;
	GetPrivateProfileString("Main", "isgray", "1", path, 1024, "./MD.ini");
	int isGray = stoi(path);//原图显示与否

	GetPrivateProfileString("Main", "isCap", "0", path, 1024, "./MD.ini");
	int isCap = stoi(path);//是否保存图片


#pragma region 参数导入
	GetPrivateProfileString("Main", "blockSize", "3", path, 1024, "./MD.ini");
	int blockSize = stoi(path);//自适应块大小
	blockSize = blockSize / 2;

	GetPrivateProfileString("Main", "delta", "1", path, 1024, "./MD.ini");
	int Delta = stoi(path);//自适应常数

	GetPrivateProfileString("Main", "r0", "1", path, 1024, "./MD.ini");
	int R0 = stoi(path);//定位下限

	GetPrivateProfileString("Main", "r1", "1", path, 1024, "./MD.ini");
	int R1 = stoi(path);//定位上限

	GetPrivateProfileString("Main", "xos", "1", path, 1024, "./MD.ini");
	int Xos = stoi(path);//x坐标补正

	GetPrivateProfileString("Main", "yos", "1", path, 1024, "./MD.ini");
	int Yos = stoi(path);//y坐标补正

	GetPrivateProfileString("Main", "rw", "1", path, 1024, "./MD.ini");
	int Rw = stoi(path);//外环半径

	GetPrivateProfileString("Main", "rn", "1", path, 1024, "./MD.ini");
	int Rn = stoi(path);//内环半径

	GetPrivateProfileString("Main", "seg", "360", path, 1024, "./MD.ini");
	float Seg = stof(path);//细分

	GetPrivateProfileString("Main", "tho1", "0", path, 1024, "./MD.ini");
	float Tho1 = stof(path);//阶梯差-上坡

	GetPrivateProfileString("Main", "tho2", "0", path, 1024, "./MD.ini");
	float Tho2 = stof(path);//阶梯差-下坡

	GetPrivateProfileString("Main", "tho3", "0", path, 1024, "./MD.ini");
	float Tho3 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho4", "0", path, 1024, "./MD.ini");
	float Tho4 = stof(path);//峰值对比度

	////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("Main", "tho5", "0", path, 1024, "./MD.ini");
	float Tho5 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho6", "0", path, 1024, "./MD.ini");
	float Tho6 = stof(path);//峰值对比度
	////////////////////////////////////////////////////////////////////////////////



	GetPrivateProfileString("Main", "gg0", "0", path, 1024, "./MD.ini");
	float Gg0 = stof(path);//A品规格

	GetPrivateProfileString("Main", "gg1", "0", path, 1024, "./MD.ini");
	float Gg1 = stof(path);//B品规格
#pragma endregion

#pragma region 区域绘制
	cvtColor(img, output, CV_GRAY2BGR);
	
#pragma endregion

#pragma region 圆心定位

#pragma region 轮廓定位
#if 1
	Mat xy;
	Mat yz;
	blur(img, yz, Size(7, 7));
	const int maxVal = 255;


#pragma region 轮廓查找
#if 0

	//adaptiveThreshold(yz, xy, maxVal, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, (2 * blockSize) + 1, Delta);

	adaptiveThreshold(img, xy, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, (2 * blockSize) + 1, Delta);
	//threshold(img, xy, Delta, 255, THRESH_BINARY);
	findContours(xy, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//cvtColor(xy, output, CV_GRAY2BGR);
	//goto OutPoint;

	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea = contourArea(tmp);
		double tmpLen = arcLength(tmp, 0);
		if (tmpLen > 2 * CV_PI*R1 || tmpLen < 2 * CV_PI*R0) continue;
		if (tmpArea > CV_PI*R1*R1 || tmpArea < CV_PI*R0*R0) continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}

	if (MaxL >= 0)
	{
		float radius;

		convexHull(contours[MaxL], tmpCont);
		ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
		center = ell.center;
		//deltaX = center.x - BzCenterX;
		//deltaY = center.y - BzCenterY;
		radius = ell.size.width / 2;

		Point2f _tmp = Point2f(Xos, Yos);
		_tmp -= center;

		if (_tmp.x*_tmp.x + _tmp.y*_tmp.y < 50000)
		{
			Xos = center.x;
			Yos = center.y;
		}
		else
		{
			flag = 1;
			goto OutPoint;
		}

	}
	else
	{
		flag = 1;
		goto OutPoint;
	}
#endif // 0
#pragma endregion

#pragma region 霍夫圆定位
#if 1
	vector<Vec3f> circles;
	GetPrivateProfileString("Main", "dp", "1", path, 1024, "./MD.ini");
	double dp = stof(path);
	GetPrivateProfileString("Main", "minDist", "25", path, 1024, "./MD.ini");
	double minDist = stof(path);
	GetPrivateProfileString("Main", "para1", "70", path, 1024, "./MD.ini");
	double para1 = stof(path);
	GetPrivateProfileString("Main", "para2", "80", path, 1024, "./MD.ini");
	double para2 = stof(path);


	double min_radius = R0;
	double max_radius = R1;
	HoughCircles(yz, circles, HOUGH_GRADIENT, 1, minDist, para1, para2, min_radius, max_radius);

	for (size_t i = 0; i < circles.size(); i++)
	{
		circle(output, Point2i(circles[MaxL][0], circles[MaxL][1]), circles[i][2], Scalar(0, 255, 0), 1);
		//if (circles[i][3] > R1 || circles[i][3] < R0)continue;

		if (circles[i][2] > area)
		{
			area = (long)circles[i][2];
			MaxL = i;
		}
	}

	if (MaxL >= 0)
	{
		center = Point2i(circles[MaxL][0], circles[MaxL][1]);

		Point2f _tmp = Point2f(Xos, Yos);
		_tmp -= center;

		if (_tmp.x*_tmp.x + _tmp.y*_tmp.y < 50000)
		{
			Xos = center.x;
			Yos = center.y;
		}
		else
		{
			flag = 1;
			goto OutPoint;
		}
	}
	else
	{
		flag = 1;
		goto OutPoint;
	}
#endif // 1
#pragma endregion
#endif // 0
#pragma endregion

	//center.x = Xos;
	//center.y = Yos;

	//circle(output, center, R0, Scalar(255, 0, 0), 2);
	//circle(output, center, R1, Scalar(255, 0, 0), 2);

	circle(output, Point(Xos, Yos), Rw, Scalar(0, 255, 0), 1);
	circle(output, Point(Xos, Yos), Rn, Scalar(0, 255, 0), 1);

#pragma endregion

#pragma region 圆周扫描
	
	
#pragma region 备选算法2--谷值峰值查询
#if 0
	Seg = Rw * 2 * CV_PI * 4;
	
	for (int i = 0; i < Seg; i++)
	{
		float s = sin(i * 360 / Seg * CV_PI / 180);
		float c = cos(i * 360 / Seg * CV_PI / 180);
		for (float j = Rn; j < Rw; j+=0.5)
		{
			//Point sta = Point(Xos + j * s, Yos - j * c);

			Point2f sta = Point2f(Xos, Yos) + j * Point2f(s, -c);
			
			int flag = img.at<uchar>(sta.y, sta.x);
			float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
			_tmp += img.at<uchar>(sta.y - 1, sta.x);
			_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
			_tmp += img.at<uchar>(sta.y, sta.x - 1);
			_tmp += img.at<uchar>(sta.y , sta.x + 1);
			_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
			_tmp += img.at<uchar>(sta.y + 1, sta.x);
			_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

			_tmp /= 8.0;

			float result = (flag - _tmp) / (flag + _tmp);
			//flag = abs(flag);
			
			if (result > Tho1)
			{
				line(output, sta, sta, Scalar(0, 0, 255), 2);
				sum0++;
			}

			if (result < -Tho2)
			{
				line(output, sta, sta, Scalar(0, 255, 0), 2);
				sum1++;
			}

		}
	}

	
#endif // 0
#pragma endregion

#pragma region 备选算法1--利用上下坡度查找
#if 1
	
	
	for (int i = 0; i < Seg; i++)
	{
		
		float s = sin(i * 360 / Seg * CV_PI / 180);
		float c = cos(i * 360 / Seg * CV_PI / 180);
		vector<int> Up2Down, Down2Up;
		for (int j = Rn; j < Rw; j++)
		{
			//Point sta = Point(Xos + j * s, Yos - j * c);

			Point2f sta = Point2f(Xos, Yos) + j * Point2f(s, -c);
			Point2f end0 = Point2f(Xos, Yos) + (j - 1) * Point2f(s, -c);
			Point2f end1 = Point2f(Xos, Yos) + (j + 1) * Point2f(s, -c);

			if (end1.x > nWidth || end1.x<0 || end1.y>nHeight || end1.y < 0)continue;

			int flag1 = img.at<uchar>(sta.y, sta.x) - img.at<uchar>(end0.y, end0.x);
			int flag2 = img.at<uchar>(end1.y, end1.x) - img.at<uchar>(sta.y, sta.x);

			if (flag1*flag2 >= 0)continue;

			if (/*flag1*flag2 < 0 &&*/ flag1<-Tho1 && flag2>Tho2) //暗点
			{

				if (flag1 < -Tho5 || flag2 > Tho6)
				{
				}
				else
				{
					//line(output, sta, sta, Scalar(0, 0, 255), 1);
					Dpoint.push_back(sta);
					//sum0++;

					//sum0 += Seg / j / 2 / CV_PI;
					sum0 += 2 * CV_PI*j / Seg;
				}

				

			}

			if (/*flag1*flag2 < 0 &&*/ flag2<-Tho1 && flag1>Tho2) //亮点
			{
				if (flag2 < -Tho5 || flag1 > Tho6)
				{
				}
				else
				{
					//line(output, sta, sta, Scalar(0, 255, 0), 1);
					Lpoint.push_back(sta);
					//sum1++;

					//sum1 += Seg / j / 2 / CV_PI;

					sum1 += 2 * CV_PI*j / Seg;
				}

				


			}

			//if (flag1 > 0)
			//{
			//	//亮点
			//	Down2Up.push_back(j);
			//}
			//if (flag1 < 0)
			//{
			//	//暗点
			//	Up2Down.push_back(j);
			//}
		}
		
		//if (Down2Up.size() == Up2Down.size())
		//{
		//	//int a = 1;
		//	a++;
		//}

		//int len = Up2Down.size();
		//if (Up2Down.size() > Down2Up.size())
		//{
		//	len = Down2Up.size();
		//}

		//for (size_t j = 0; j < len; j++)
		//{
		//	if (abs(Down2Up[j] - Up2Down[j]) >= 1 && abs(Down2Up[j] - Up2Down[j]) <= 3)
		//	{
		//		Point2f sta = Point2f(Xos, Yos) + (Down2Up[j] + Up2Down[j]) / 2 * Point2f(s, -c);
		//		line(output, sta, sta, Scalar(0, 255, 255), 1);
		//	}
		//}
	}

#pragma region 二次反查
	
	for (size_t i = 0; i < Lpoint.size(); i++)
	{
		Point2f sta = Lpoint[i];

		int flag = img.at<uchar>(sta.y, sta.x);
		float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y - 1, sta.x);
		_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
		_tmp += img.at<uchar>(sta.y, sta.x - 1);
		_tmp += img.at<uchar>(sta.y, sta.x + 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x);
		_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

		_tmp /= 8.0;

		float result = (flag - _tmp) / (flag + _tmp);
		//flag = abs(flag);

		if (result > Tho3)
		{
			line(output, sta, sta, Scalar(255, 0, 255), 2);
			//summ1++;

			sta -= center;
			summ1 += sqrt(center.x*center.x + center.y*center.y) * 2 * CV_PI / Seg;
		}

		if (result < -Tho4)
		{
			line(output, sta, sta, Scalar(0, 255, 255), 2);
			//summ2++;

			sta -= center;
			summ2 += sqrt(center.x*center.x + center.y*center.y) * 2 * CV_PI / Seg;

		}
	}

	for (size_t i = 0; i < Dpoint.size(); i++)
	{
		Point2f sta = Dpoint[i];

		int flag = img.at<uchar>(sta.y, sta.x);
		float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y - 1, sta.x);
		_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
		_tmp += img.at<uchar>(sta.y, sta.x - 1);
		_tmp += img.at<uchar>(sta.y, sta.x + 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x);
		_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

		_tmp /= 8.0;

		float result = (flag - _tmp) / (flag + _tmp);
		//flag = abs(flag);

		if (result > Tho3)
		{
			line(output, sta, sta, Scalar(255, 0, 255), 2);
			//summ1++;

			sta -= center;
			summ1 += sqrt(center.x*center.x + center.y*center.y) * 2 * CV_PI / Seg;
		}

		if (result < -Tho4)
		{
			line(output, sta, sta, Scalar(0, 255, 255), 2);
			//summ2++;

			sta -= center;
			summ2 += sqrt(center.x*center.x + center.y*center.y) * 2 * CV_PI / Seg;

		}
	}


#pragma endregion

	//Sleep(1000);
	//OK_ss << "OK角度数:" << a << endl;
	OK_ss << "红点数:" << sum0 << endl;
	OK_ss << "绿点数:" << sum1 << endl;

	OK_ss << "有效红点数:" << summ1 << endl;
	OK_ss << "有效绿点数:" << summ2 << endl;

	//int result = 0;
	if (summ1 > Gg1 || summ2 > Gg1)
	{
		OK_ss << "C品" << endl;
		flag = 2;
	}
	else if (summ1 > Gg0 || summ2 > Gg0)
	{
		OK_ss << "B品" << endl;
		flag = 1;
	}
	else
	{
		OK_ss << "A品" << endl;
		flag = 0;

	}


#endif // 0
#pragma endregion
	
#pragma endregion

#pragma region 原图输出
	
	if (isGray == 0)
	{
		cvtColor(img, output, CV_GRAY2BGR);
	}
#pragma endregion

	
#pragma region 参数载入
	////string str = ;;
	//GetPrivateProfileString("Main", "multiple", "1", path, 1024, "./2.ini");
	//int multiple = stoi(path);//倍数

	//GetPrivateProfileString("Main", "gamma", "1", path, 1024, "./2.ini");
	//float gamma = stof(path);//Gamma

	//GetPrivateProfileString("Main", "r0", "1", path, 1024, "./2.ini");
	//int r0 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r1", "1", path, 1024, "./2.ini");
	//int r1 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r2", "1", path, 1024, "./2.ini");
	//int r2 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r3", "1", path, 1024, "./2.ini");
	//int r3 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r4", "1", path, 1024, "./2.ini");
	//int r4 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r5", "1", path, 1024, "./2.ini");
	//int r5 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "threshold0", "128", path, 1024, "./2.ini");
	//int threshold0 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "isGray", "1", path, 1024, "./2.ini");
	//int isGray = stoi(path);//倍数
#pragma endregion

#pragma region 改善原图
	//img *= multiple;

	//Mat Gamma;

	//MyGamma(img, Gamma, gamma);
	//img = Gamma.clone();
#pragma endregion

#pragma region 清除噪声
	//Mat maskEx;
	////Laplacian(img, maskEx, 8, 1);
	//blur(img, maskEx, Size(r2, r3));
#pragma endregion

#pragma region 获取白色孤立点
	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(r4, r5));
	//morphologyEx(maskEx, gray, CV_MOP_OPEN, element);

	//element = getStructuringElement(MORPH_ELLIPSE, Size(r0, r1));
	//morphologyEx(gray, gray, CV_MOP_TOPHAT, element);
	////morphologyEx(img, gray, CV_MOP_BLACKHAT, element);

	//threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值

	//maskEx = gray.clone();
	//blur(maskEx, gray, Size(r2, r3));

	//GetPrivateProfileString("Main", "path2", "", path, 1024, "./2.ini");
	//Mat img2 = imread(path, 0);

	//GetPrivateProfileString("Main", "threshold1", "", path, 1024, "./2.ini");
	//int threshold1 = stoi(path);
	//threshold(img, img, threshold1, 255, THRESH_TOZERO_INV);
	//threshold(img2, img2, threshold1, 255, THRESH_TOZERO_INV);

	//Mat dev = img - img2;

	////imshow("dev", dev);
	////waitKey(2);
#pragma endregion

#pragma endregion

#pragma region 函数出口
	result = 0;
OutPoint:
#pragma endregion

#pragma region 文字输出
	//字体大小
	int text_Size;
	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
	//位置
	Point text_Localtion01;
	text_Localtion01.x = text_Size / 3;
	text_Localtion01.y = text_Size / 3;
	Point text_Localtion02;
	text_Localtion02.x = text_Size / 3;
	text_Localtion02.y = nHeight - text_Size * 4;
	Point text_Localtion03;
	text_Localtion03.x = text_Size / 3;
	text_Localtion03.y = nHeight - text_Size * 3;

	//ERR_ss << "\r" << endl;
	//OK_ss << "\r" << endl;

	std::string test;
	if (result != 0)
	{
		//test = ERR_ss.str();
		//putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	}
	else
	{
		test = OK_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
	}
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	SYSTEMTIME st;
	//	GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), output);
	//}
#pragma endregion		

#pragma region 不良镜片保存
	if (isCap != 0)
	{
		stringstream path;
#pragma region NG-Input图片输出
		//判断是否存在最新输出文件夹
		path << "D:\\picture\\input";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), img);
#pragma endregion

#pragma region NG-Output图片输出
		path.str("");
		//判断是否存在最新输出文件夹
		path << "D:\\picture\\Output";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), output);
#pragma endregion
	}
#pragma endregion

#pragma region 参数输出

	//OutputX = 5;
	//OutputY = 5;
	//
	//tm.st/*o*/p();
	//耗时
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = flag;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	//free(r4);
	//free(r5);
	//free(GPR1);
	//free(GPR2);
	//free(th2);

	free(path);
#pragma endregion

	return 0;
}

bool tmp3(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);

	//GetPrivateProfileString("Main", "path", "", path, 1024, "./MD.ini");
	//Mat img = imread(path, 0);
	//CV_Assert(img.data);

	Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	Mat img = img_Temp.clone();

	//Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	SYSTEMTIME st;
	GetLocalTime(&st);
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = 0;
	//Vec2i ang;
	float size = 360;		//圆周切割段数 这里用360做定指扫描
	//TickMeter tm;
	RotatedRect ell;
	CONTOURS contours;
	stringstream OK_ss;
	//stringstream ERR_ss;
	Point2f center(0, 0);
	vector<bool> GoodPoint;
	vector<Vec4i> hierarchy;
	vector<Point2i> tmpCont;
	Mat gray, output, img_decode, temp;

	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "麻点检测" << endl;

	//ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	//ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	//ERR_ss << "麻点检测" << endl;

	gray = img.clone();

	int flag = 2;

	//tm.start();
#pragma endregion

#pragma region 输出参数
	//int deltaX = 0;
	//int deltaY = 0;
	//float angle = 0;
	int num = 0;
	bool result = 0; //1是ng 0是ok
#pragma endregion

#pragma region 麻点检测
	
#pragma region 参数导入

	Mat gray_Gus, dst, LoG, LoGEx;
	Mat Green = Mat(img.rows, img.cols, CV_8UC3, Scalar(0, 255, 0));

	float sum0 = 0, sum1 = 0;
	int a = 0;
	vector<Point2f> Lpoint, Dpoint;
	vector<Point2f> LpointEx, DpointEx;
	float summ1 = 0, summ2 = 0;
	GetPrivateProfileString("Main", "isgray", "1", path, 1024, "./MD.ini");
	int isGray = stoi(path);//原图显示与否

	GetPrivateProfileString("Main", "isCap", "0", path, 1024, "./MD.ini");
	int isCap = stoi(path);//是否保存图片

	GetPrivateProfileString("Main", "blockSize", "3", path, 1024, "./MD.ini");
	int blockSize = stoi(path);//自适应块大小
	blockSize = blockSize / 2;

	GetPrivateProfileString("Main", "delta", "1", path, 1024, "./MD.ini");
	int Delta = stoi(path);//自适应常数

	GetPrivateProfileString("Main", "r0", "1", path, 1024, "./MD.ini");
	int R0 = stoi(path);//定位下限

	GetPrivateProfileString("Main", "r1", "1", path, 1024, "./MD.ini");
	int R1 = stoi(path);//定位上限

	GetPrivateProfileString("Main", "xos", "1", path, 1024, "./MD.ini");
	int Xos = stoi(path);//x坐标补正

	GetPrivateProfileString("Main", "yos", "1", path, 1024, "./MD.ini");
	int Yos = stoi(path);//y坐标补正

	GetPrivateProfileString("Main", "rw", "1", path, 1024, "./MD.ini");
	int Rw = stoi(path);//外环半径

	GetPrivateProfileString("Main", "rn", "1", path, 1024, "./MD.ini");
	int Rn = stoi(path);//内环半径

	GetPrivateProfileString("Main", "seg", "360", path, 1024, "./MD.ini");
	float Seg = stof(path);//细分

	GetPrivateProfileString("Main", "tho1", "0", path, 1024, "./MD.ini");
	float Tho1 = stof(path);//阶梯差-上坡

	GetPrivateProfileString("Main", "tho2", "0", path, 1024, "./MD.ini");
	float Tho2 = stof(path);//阶梯差-下坡

	GetPrivateProfileString("Main", "tho3", "0", path, 1024, "./MD.ini");
	float Tho3 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho4", "0", path, 1024, "./MD.ini");
	float Tho4 = stof(path);//峰值对比度

	////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("Main", "tho5", "0", path, 1024, "./MD.ini");
	float Tho5 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho6", "0", path, 1024, "./MD.ini");
	float Tho6 = stof(path);//峰值对比度
	////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("Main", "tho12", "0", path, 1024, "./MD.ini");
	float Tho12 = stof(path);//阶梯差-上坡

	GetPrivateProfileString("Main", "tho22", "0", path, 1024, "./MD.ini");
	float Tho22 = stof(path);//阶梯差-下坡

	GetPrivateProfileString("Main", "tho32", "0", path, 1024, "./MD.ini");
	float Tho32 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho42", "0", path, 1024, "./MD.ini");
	float Tho42 = stof(path);//峰值对比度

	////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("Main", "tho52", "0", path, 1024, "./MD.ini");
	float Tho52 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho62", "0", path, 1024, "./MD.ini");
	float Tho62 = stof(path);//峰值对比度
	////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////////////


	GetPrivateProfileString("Main", "Ldh", "0", path, 1024, "./MD.ini");
	float Ldh = stof(path);//亮点规格上限
	GetPrivateProfileString("Main", "Ldl", "0", path, 1024, "./MD.ini");
	float Ldl = stof(path);//亮点规格下限
	GetPrivateProfileString("Main", "Ddh", "0", path, 1024, "./MD.ini");
	float Ddh = stof(path);//暗点规格上限
	GetPrivateProfileString("Main", "Ddl", "0", path, 1024, "./MD.ini");
	float Ddl = stof(path);//暗点规格下限

	GetPrivateProfileString("Main", "mag", "1", path, 1024, "./MD.ini");
	float Mag = stof(path);//暗点规格下限

	GetPrivateProfileString("Main", "gg0", "0", path, 1024, "./MD.ini");
	float Gg0 = stof(path);//A品规格

	GetPrivateProfileString("Main", "gg1", "0", path, 1024, "./MD.ini");
	float Gg1 = stof(path);//B品规格
#pragma endregion

#pragma region 区域绘制
	cvtColor(img, output, CV_GRAY2BGR);
#pragma endregion

#pragma region 圆心定位
#if 1
	Mat xy;
	Mat yz;
	blur(img, yz, Size(7, 7));
	const int maxVal = 255;
#pragma region 轮廓查找
#if 0

	//adaptiveThreshold(yz, xy, maxVal, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, (2 * blockSize) + 1, Delta);

	adaptiveThreshold(img, xy, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, (2 * blockSize) + 1, Delta);
	//threshold(img, xy, Delta, 255, THRESH_BINARY);
	findContours(xy, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//cvtColor(xy, output, CV_GRAY2BGR);
	//goto OutPoint;

	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea = contourArea(tmp);
		double tmpLen = arcLength(tmp, 0);
		if (tmpLen > 2 * CV_PI*R1 || tmpLen < 2 * CV_PI*R0) continue;
		if (tmpArea > CV_PI*R1*R1 || tmpArea < CV_PI*R0*R0) continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}

	if (MaxL >= 0)
	{
		float radius;

		convexHull(contours[MaxL], tmpCont);
		ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
		center = ell.center;
		//deltaX = center.x - BzCenterX;
		//deltaY = center.y - BzCenterY;
		radius = ell.size.width / 2;

		Point2f _tmp = Point2f(Xos, Yos);
		_tmp -= center;

		if (_tmp.x*_tmp.x + _tmp.y*_tmp.y < 50000)
		{
			Xos = center.x;
			Yos = center.y;
		}
		else
		{
			flag = 1;
			goto OutPoint;
		}

	}
	else
	{
		flag = 1;
		goto OutPoint;
	}
#endif // 0
#pragma endregion

#pragma region 霍夫圆定位
#if 1
	vector<Vec3f> circles;
	GetPrivateProfileString("Main", "dp", "1", path, 1024, "./MD.ini");
	double dp = stof(path);
	GetPrivateProfileString("Main", "minDist", "25", path, 1024, "./MD.ini");
	double minDist = stof(path);
	GetPrivateProfileString("Main", "para1", "70", path, 1024, "./MD.ini");
	double para1 = stof(path);
	GetPrivateProfileString("Main", "para2", "80", path, 1024, "./MD.ini");
	double para2 = stof(path);


	double min_radius = R0;
	double max_radius = R1;
	HoughCircles(yz, circles, HOUGH_GRADIENT, 1, minDist, para1, para2, min_radius, max_radius);

	for (size_t i = 0; i < circles.size(); i++)
	{
		//if (circles[i][3] > R1 || circles[i][3] < R0)continue;

		if (circles[i][2] > area)
		{
			area = (long)circles[i][2];
			MaxL = i;
		}
	}

	if (MaxL >= 0)
	{
		//circle(output, Point2i(circles[MaxL][0], circles[MaxL][1]), circles[MaxL][2], Scalar(255, 0, 0), 1);

		center = Point2i(circles[MaxL][0], circles[MaxL][1]);

		Point2f _tmp = Point2f(Xos, Yos);
		_tmp -= center;

		if (_tmp.x*_tmp.x + _tmp.y*_tmp.y < 500000)
		{
			Xos = center.x;
			Yos = center.y;
		}
		else
		{
			flag = 1;
			goto OutPoint;
		}
	}
	else
	{
		flag = 1;
		goto OutPoint;
	}
#endif // 1
#pragma endregion
#endif // 0
	circle(output, Point(Xos, Yos), Rw, Scalar(0, 255, 0), 1);
	circle(output, Point(Xos, Yos), Rn, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 圆周扫描

#pragma region 备选算法2--谷值峰值查询
#if 0
	Seg = Rw * 2 * CV_PI * 4;

	for (int i = 0; i < Seg; i++)
	{
		float s = sin(i * 360 / Seg * CV_PI / 180);
		float c = cos(i * 360 / Seg * CV_PI / 180);
		for (float j = Rn; j < Rw; j += 0.5)
		{
			//Point sta = Point(Xos + j * s, Yos - j * c);

			Point2f sta = Point2f(Xos, Yos) + j * Point2f(s, -c);

			int flag = img.at<uchar>(sta.y, sta.x);
			float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
			_tmp += img.at<uchar>(sta.y - 1, sta.x);
			_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
			_tmp += img.at<uchar>(sta.y, sta.x - 1);
			_tmp += img.at<uchar>(sta.y, sta.x + 1);
			_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
			_tmp += img.at<uchar>(sta.y + 1, sta.x);
			_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

			_tmp /= 8.0;

			float result = (flag - _tmp) / (flag + _tmp);
			//flag = abs(flag);

			if (result > Tho1)
			{
				line(output, sta, sta, Scalar(0, 0, 255), 2);
				sum0++;
			}

			if (result < -Tho2)
			{
				line(output, sta, sta, Scalar(0, 255, 0), 2);
				sum1++;
			}

		}
	}


#endif // 0
#pragma endregion

#pragma region 备选算法1--利用上下坡度查找
#if 1
#pragma region 一次查找
	for (int i = 0; i < Seg; i++)
	{

		float s = sin(i * 360 / Seg * CV_PI / 180);
		float c = cos(i * 360 / Seg * CV_PI / 180);
		vector<int> Up2Down, Down2Up;
		for (int j = Rn; j < Rw; j++)
		{
			//Point sta = Point(Xos + j * s, Yos - j * c);

			Point2f sta = Point2f(Xos, Yos) + j * Point2f(s, -c);
			Point2f end0 = Point2f(Xos, Yos) + (j - 2) * Point2f(s, -c);
			Point2f end1 = Point2f(Xos, Yos) + (j + 2) * Point2f(s, -c);

			if (end1.x > nWidth || end1.x<0 || end1.y>nHeight || end1.y < 0)continue;

			int flag1 = img.at<uchar>(sta.y, sta.x) - img.at<uchar>(end0.y, end0.x);
			int flag2 = img.at<uchar>(end1.y, end1.x) - img.at<uchar>(sta.y, sta.x);

			if (flag1*flag2 >= 0)continue;

			//if (/*flag1*flag2 < 0 &&*/ flag1<-Tho1 && flag2>Tho2) //暗点
			if (flag1<-Tho1 && flag2>Tho2) //暗点
			{

				if (flag1 < -Tho3 || flag2 > Tho4)
				{
				}
				else
				{
					//line(output, sta, sta, Scalar(0, 0, 255), 1);
					Dpoint.push_back(sta);
					//sum0++;

					//sum0 += Seg / j / 2 / CV_PI;
					sum0 += 2 * CV_PI*j / Seg;
				}
			}

			if (flag2<-Tho12 && flag1>Tho22) //亮点
			{
				if (flag2 < -Tho32 || flag1 > Tho42)
				{
				}
				else
				{
					//line(output, sta, sta, Scalar(0, 255, 0), 1);
					Lpoint.push_back(sta);
					//sum1++;

					//sum1 += Seg / j / 2 / CV_PI;

					sum1 += 2 * CV_PI*j / Seg;
				}
			}
		}
	}

	OK_ss << "一次暗点数:" << sum0 << endl;
	OK_ss << "一次亮点数:" << sum1 << endl;
#pragma endregion

#pragma region 二次反查
#if 1
	for (size_t i = 0; i < Dpoint.size(); i++)
	{
		Point2f sta = Dpoint[i];

		int flag = img.at<uchar>(sta.y, sta.x);
		float _tmp = img.at<uchar>(sta.y - 2, sta.x - 2);
		_tmp += img.at<uchar>(sta.y - 2, sta.x);
		_tmp += img.at<uchar>(sta.y - 2, sta.x + 2);
		_tmp += img.at<uchar>(sta.y, sta.x - 2);
		_tmp += img.at<uchar>(sta.y, sta.x + 2);
		_tmp += img.at<uchar>(sta.y + 2, sta.x - 2);
		_tmp += img.at<uchar>(sta.y + 2, sta.x);
		_tmp += img.at<uchar>(sta.y + 2, sta.x + 2);

		_tmp /= 8.0;

		float result = (flag - _tmp) / (flag + _tmp);
		//flag = abs(flag);

		//if (result > Tho5)
		//{
		//	//line(output, sta, sta, Scalar(255, 0, 255), 2);
		//	//summ1++;
		//	sta -= center;
		//	summ1 += sqrt(center.x*center.x + center.y*center.y) * 2 * CV_PI / Seg;
		//	DpointEx.push_back(Dpoint[i]);
		//}

		if (result < -Tho6 && result > -Tho5)
		{
			//line(output, sta, sta, Scalar(0, 255, 255), 2);
			//summ2++;

			sta -= center;
			summ2 += sqrt(center.x*center.x + center.y*center.y) * 2 * CV_PI / Seg;
			DpointEx.push_back(Dpoint[i]);
		}
	}

	for (size_t i = 0; i < Lpoint.size(); i++)
	{
		Point2f sta = Lpoint[i];

		//int flag = img.at<uchar>(sta.y, sta.x);
		//float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
		//_tmp += img.at<uchar>(sta.y - 1, sta.x);
		//_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
		//_tmp += img.at<uchar>(sta.y, sta.x - 1);
		//_tmp += img.at<uchar>(sta.y, sta.x + 1);
		//_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
		//_tmp += img.at<uchar>(sta.y + 1, sta.x);
		//_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

		if (sta.x + 2 >= nWidth || sta.y + 2 >= nHeight)continue;
		if (sta.x - 2 < 0 || sta.y - 2 < 0)continue;

		int flag = img.at<uchar>(sta.y, sta.x);
		float _tmp = img.at<uchar>(sta.y - 2, sta.x - 2);
		_tmp += img.at<uchar>(sta.y - 2, sta.x);
		_tmp += img.at<uchar>(sta.y - 2, sta.x + 2);
		_tmp += img.at<uchar>(sta.y, sta.x - 2);
		_tmp += img.at<uchar>(sta.y, sta.x + 2);
		_tmp += img.at<uchar>(sta.y + 2, sta.x - 2);
		_tmp += img.at<uchar>(sta.y + 2, sta.x);
		_tmp += img.at<uchar>(sta.y + 2, sta.x + 2);

		_tmp /= 8.0;

		float result = (flag - _tmp) / (flag + _tmp);
		//flag = abs(flag);

		if (result > Tho52 && result < Tho62)
		{
			//line(output, sta, sta, Scalar(255, 0, 255), 2);
			//summ1++;

			sta -= center;
			summ1 += sqrt(sta.x*sta.x + sta.y*sta.y) * 2 * CV_PI / Seg;

			LpointEx.push_back(Lpoint[i]);
		}

		//if (result < -Tho62)
		//{
		//	//line(output, sta, sta, Scalar(0, 255, 255), 2);
		//	//summ2++;
		//	sta -= center;
		//	summ2 += sqrt(sta.x*sta.x + sta.y*sta.y) * 2 * CV_PI / Seg;
		//	LpointEx.push_back(Lpoint[i]);
		//}
	}

	OK_ss << "二次暗点数:" << summ2 << endl;
	OK_ss << "二次亮点数:" << summ1 << endl;
#endif // 0

	
#pragma endregion

#pragma region 三次反差
#if 0
	gray = img.clone();

	summ1 = 0;
	summ2 = 0;

	blockSize = 2 * blockSize + 1;
	GaussianBlur(gray, gray_Gus, Size(blockSize, blockSize), 0);

	Laplacian(gray_Gus, dst, CV_16S, 5);

	convertScaleAbs(dst, LoGEx);

	LoGEx -= img * Mag;

	for (size_t i = 0; i < DpointEx.size(); i++)
	{
		Point2f Wz = DpointEx[i];

		if (Wz.x >= nWidth || Wz.x < 0)continue;
		if (Wz.y >= nHeight || Wz.y < 0)continue;


		int Ld = LoGEx.at<uchar>(Wz.y, Wz.x);
		if (Ld > Ddl && Ld < Ddh)
		{
			line(output, Wz, Wz, Scalar(255, 0, 0), 2);

			Wz -= center;
			summ1 += sqrt(Wz.x*Wz.x + Wz.y*Wz.y) * 2 * CV_PI / Seg;

		}
	}

	for (size_t i = 0; i < LpointEx.size(); i++)
	{
		Point2f Wz = LpointEx[i];

		if (Wz.x >= nWidth || Wz.x < 0)continue;
		if (Wz.y >= nHeight || Wz.y < 0)continue;

		int Ld = LoGEx.at<uchar>(Wz.y, Wz.x);
		if (Ld > Ldl && Ld < Ldh)
		{
			line(output, Wz, Wz, Scalar(0, 0, 255), 2);

			Wz -= center;
			summ2 += sqrt(Wz.x*Wz.x + Wz.y*Wz.y) * 2 * CV_PI / Seg;

		}
	}

	OK_ss << "三次暗点数:" << summ1 << endl;
	OK_ss << "三次亮点数:" << summ2 << endl;
#endif // 0
#pragma endregion

#pragma region 文本输出
	//Sleep(1000);
	//OK_ss << "OK角度数:" << a << endl;
	

	//OK_ss << "有效红点数:" << summ1 << endl;
	//OK_ss << "有效绿点数:" << summ2 << endl;

	//int result = 0;
	if (summ1 > Gg1 || summ2 > Gg1)
	{
		OK_ss << "C品" << endl;
		flag = 2;
	}
	else if (summ1 > Gg0 || summ2 > Gg0)
	{
		OK_ss << "B品" << endl;
		flag = 1;
	}
	else
	{
		OK_ss << "A品" << endl;
		flag = 0;

	}
#pragma endregion

#endif // 0
#pragma endregion

#pragma region LoG算子
#if 0
	gray = img.clone();

#pragma region 第一次
	blockSize = 2 * blockSize + 1;
	GaussianBlur(gray, gray_Gus, Size(blockSize, blockSize), 0);

	Laplacian(gray_Gus, dst, CV_16S, 5);

	convertScaleAbs(dst, LoGEx);
#pragma endregion

#pragma region 第二次
	//GaussianBlur(LoGEx, gray_Gus, Size(3, 3), 0);

	//Laplacian(gray_Gus, dst, CV_16S, 3);

	//convertScaleAbs(dst, LoG);
	//LoG *= 1;
#pragma endregion

	threshold(LoGEx, LoGEx, Delta, 255, THRESH_BINARY);

	findContours(LoGEx, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	//Green.copyTo(output, LoGEx);

	for (size_t i = 0; i < contours.size(); i++)
	{
		double tmpArea = contourArea(contours[i]);
		if (tmpArea > 5 && tmpArea < 30)
		{
			drawContours(output, contours, i, Scalar(255, 0, 0), -1);
		}
	}
	//if (isGray)
	//{
	//	cvtColor(img, output, CV_GRAY2BGR);
	//	Green.copyTo(output, gray);
	//	//drawContours(output, contours, -1, Scalar(0, 255, 0), -1);
	//}
	//else
	//{
	//	cvtColor(gray, output, CV_GRAY2BGR);
	//	
	//}



	//cvtColor(LoG, output, CV_GRAY2BGR);
	//imshow("log", LoG);
	//waitKey(3);
#endif // 0

#pragma endregion

#pragma endregion

#pragma region 原图输出

	if (isGray == 0)
	{
		cvtColor(img, output, CV_GRAY2BGR);
	}
#pragma endregion

#pragma endregion

#pragma region 函数出口
	result = 0;
OutPoint:
#pragma endregion

#pragma region 文字输出
	//字体大小
	int text_Size;
	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
	text_Size /= 2;
	//位置
	Point text_Localtion01;
	text_Localtion01.x = text_Size / 3;
	text_Localtion01.y = text_Size / 3;
	Point text_Localtion02;
	text_Localtion02.x = text_Size / 3;
	text_Localtion02.y = nHeight - text_Size * 4;
	Point text_Localtion03;
	text_Localtion03.x = text_Size / 3;
	text_Localtion03.y = nHeight - text_Size * 3;

	//ERR_ss << "\r" << endl;
	//OK_ss << "\r" << endl;

	std::string test;
	if (result != 0)
	{
		//test = ERR_ss.str();
		//putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	}
	else
	{
		test = OK_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
	}
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	SYSTEMTIME st;
	//	GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), output);
	//}
#pragma endregion		

#pragma region 不良镜片保存
	if (isCap != 0)
	{
		stringstream path;
#pragma region NG-Input图片输出
		//判断是否存在最新输出文件夹
		path << "D:\\picture\\input";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), img);
#pragma endregion

#pragma region NG-Output图片输出
		path.str("");
		//判断是否存在最新输出文件夹
		path << "D:\\picture\\Output";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), output);
#pragma endregion
	}
#pragma endregion

#pragma region 参数输出

	//OutputX = 5;
	//OutputY = 5;
	//
	//tm.st/*o*/p();
	//耗时
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = flag;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	//free(r4);
	//free(r5);
	//free(GPR1);
	//free(GPR2);
	//free(th2);

	free(path);
#pragma endregion

	return 0;
}

bool tmp2(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);

	//GetPrivateProfileString("Main", "path", "", path, 1024, "./MD.ini");
	//Mat img = imread(path, 0);
	//CV_Assert(img.data);

	//Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img = img_Temp.clone();

	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img = img2.clone();
	//LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	//GetPrivateProfileString("Main", "Path", "", path, 1024, "./2.ini");
	//Mat img = imread(path, 0);

	SYSTEMTIME st;
	GetLocalTime(&st);

	//bool isGray = (bool)atoi(input_Parameter[0]);	//灰度显示 0表示显示原图 1表示显示灰度图

	//int r0 = atoi(input_Parameter[1]);		//定位-内环半径
	//int r1 = atoi(input_Parameter[2]);		//定位-外环半径
	//int th0 = atoi(input_Parameter[3]);	//定位-灰度阈值

	//int NoUse0 = 0;//占位但是不用，界面上也不需要显示
	//int QKLength = atoi(input_Parameter[4]);	//缺口-长度下限
	//int resultNum = atoi(input_Parameter[5]);	//缺口-缺口数量
	//int NoUse1 = 0;//占位但是不用，界面上也不需要显示

	//int r2 = atoi(input_Parameter[6]);		//缺口-外环半径
	//int r3 = atoi(input_Parameter[7]);			//缺口-内环半径
	//float GPR0 = atof(input_Parameter[8]);	//缺口-径向有效点比例
	////GPR0 /= 100;
	//int th1 = atoi(input_Parameter[9]);		//缺口-灰度阈值

	//vector<string> SaveImg;
	//int SaveImgNum = 0;
	//int CameraNum = 0;
	//int XTNum = 0;
	//SaveImg = Spilt2(input_Parameter[10]);
	//if (SaveImg.size() == 3)
	//{
	//	SaveImgNum = stoi(SaveImg[2]);
	//	CameraNum = stoi(SaveImg[0]);
	//	XTNum = stoi(SaveImg[1]);
	//}

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	//SYSTEMTIME st;
	//	//GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), img);
	//}
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = 0;
	//Vec2i ang;
	float size = 360;		//圆周切割段数 这里用360做定指扫描
	//TickMeter tm;
	RotatedRect ell;
	CONTOURS contours;
	stringstream OK_ss;
	//stringstream ERR_ss;
	Point2f center(0, 0);
	vector<bool> GoodPoint;
	vector<Vec4i> hierarchy;
	vector<Point2i> tmpCont;
	Mat gray, output, img_decode, temp;

	OK_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	OK_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	OK_ss << "麻点检测2" << endl;

	//ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	//ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	//ERR_ss << "麻点检测2" << endl;

	gray = img.clone();

	int flag = 2;

	//tm.start();
#pragma endregion

#pragma region 输出参数
	//int deltaX = 0;
	//int deltaY = 0;
	//float angle = 0;
	int num = 0;
	bool result = 0; //1是ng 0是ok
#pragma endregion

#pragma region 麻点检测
	int sum0 = 0, sum1 = 0;
	int a = 0;
	vector<Point2f> Lpoint, Dpoint;
	int summ1 = 0, summ2 = 0;
	GetPrivateProfileString("Main", "isgray", "1", path, 1024, "./MD2.ini");
	int isGray = stoi(path);//原图显示与否

	GetPrivateProfileString("Main", "isCap", "0", path, 1024, "./MD.ini");
	int isCap = stoi(path);//是否保存图片
#pragma region 参数导入
	//GetPrivateProfileString("Main", "blockSize", "3", path, 1024, "./MD.ini");
	//int blockSize = stoi(path);//自适应块大小
	//blockSize = blockSize / 2;

	GetPrivateProfileString("Main", "delta", "1", path, 1024, "./MD2.ini");
	int Delta = stoi(path);//自适应常数

	GetPrivateProfileString("Main", "r0", "1", path, 1024, "./MD2.ini");
	int R0 = stoi(path);//定位下限

	GetPrivateProfileString("Main", "r1", "1", path, 1024, "./MD2.ini");
	int R1 = stoi(path);//定位上限

	GetPrivateProfileString("Main", "xos", "1", path, 1024, "./MD2.ini");
	int Xos = stoi(path);//x坐标补正

	GetPrivateProfileString("Main", "yos", "1", path, 1024, "./MD2.ini");
	int Yos = stoi(path);//y坐标补正

	GetPrivateProfileString("Main", "rw", "1", path, 1024, "./MD2.ini");
	int Rw = stoi(path);//外环半径

	GetPrivateProfileString("Main", "rn", "1", path, 1024, "./MD2.ini");
	int Rn = stoi(path);//内环半径

	GetPrivateProfileString("Main", "seg", "360", path, 1024, "./MD2.ini");
	float Seg = stof(path);//细分

	GetPrivateProfileString("Main", "tho1", "0", path, 1024, "./MD2.ini");
	float Tho1 = stof(path);//阶梯差-上坡

	GetPrivateProfileString("Main", "tho2", "0", path, 1024, "./MD2.ini");
	float Tho2 = stof(path);//阶梯差-下坡

	GetPrivateProfileString("Main", "tho3", "0", path, 1024, "./MD2.ini");
	float Tho3 = stof(path);//谷值对比度

	GetPrivateProfileString("Main", "tho4", "0", path, 1024, "./MD2.ini");
	float Tho4 = stof(path);//峰值对比度

	GetPrivateProfileString("Main", "gg0", "0", path, 1024, "./MD2.ini");
	float Gg0 = stof(path);//A品规格

	GetPrivateProfileString("Main", "gg1", "0", path, 1024, "./MD2.ini");
	float Gg1 = stof(path);//B品规格
#pragma endregion

#pragma region 区域绘制
	cvtColor(img, output, CV_GRAY2BGR);

#pragma endregion

#pragma region 圆心定位

#pragma region 轮廓定位
#if 1
	Mat xy;
	//Mat gray = img.clone();
	//blur(img, gray, Size(7, 7));
	//adaptiveThreshold(gray, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, (2 * blockSize) + 1, Delta);
	threshold(img, xy, Delta, 255, THRESH_BINARY);
	findContours(xy, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmp;
		convexHull(contours[i], tmp);
		double tmpArea = contourArea(tmp);
		double tmpLen = arcLength(tmp, 0);
		if (tmpLen > 2 * CV_PI*R1 || tmpLen < 2 * CV_PI*R0) continue;
		if (tmpArea > CV_PI*R1*R1 || tmpArea < CV_PI*R0*R0) continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}

	if (MaxL >= 0)
	{
		float radius;

		convexHull(contours[MaxL], tmpCont);
		ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
		center = ell.center;
		//deltaX = center.x - BzCenterX;
		//deltaY = center.y - BzCenterY;
		radius = ell.size.width / 2;

		Point2f _tmp = Point2f(Xos, Yos);
		_tmp -= center;

		if (_tmp.x*_tmp.x + _tmp.y*_tmp.y < 50000)
		{
			Xos = center.x;
			Yos = center.y;
		}
		else
		{
			flag = 1;
			goto OutPoint;
		}

	}
	else
	{
		flag = 1;
		goto OutPoint;
	}

#endif // 0
#pragma endregion

#pragma region 霍夫圆定位
#if 1

#endif // 1

#pragma endregion

	//center.x = Xos;
	//center.y = Yos;

	//circle(output, center, R0, Scalar(255, 0, 0), 2);
	//circle(output, center, R1, Scalar(255, 0, 0), 2);

	circle(output, Point(Xos, Yos), Rw, Scalar(0, 255, 0), 1);
	circle(output, Point(Xos, Yos), Rn, Scalar(0, 255, 0), 1);

#pragma endregion

#pragma region 圆周扫描

#pragma region 备选算法2--谷值峰值查询
#if 0
	Seg = Rw * 2 * CV_PI * 4;

	for (int i = 0; i < Seg; i++)
	{
		float s = sin(i * 360 / Seg * CV_PI / 180);
		float c = cos(i * 360 / Seg * CV_PI / 180);
		for (float j = Rn; j < Rw; j += 0.5)
		{
			//Point sta = Point(Xos + j * s, Yos - j * c);

			Point2f sta = Point2f(Xos, Yos) + j * Point2f(s, -c);

			int flag = img.at<uchar>(sta.y, sta.x);
			float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
			_tmp += img.at<uchar>(sta.y - 1, sta.x);
			_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
			_tmp += img.at<uchar>(sta.y, sta.x - 1);
			_tmp += img.at<uchar>(sta.y, sta.x + 1);
			_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
			_tmp += img.at<uchar>(sta.y + 1, sta.x);
			_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

			_tmp /= 8.0;

			float result = (flag - _tmp) / (flag + _tmp);
			//flag = abs(flag);

			if (result > Tho1)
			{
				line(output, sta, sta, Scalar(0, 0, 255), 2);
				sum0++;
			}

			if (result < -Tho2)
			{
				line(output, sta, sta, Scalar(0, 255, 0), 2);
				sum1++;
			}

		}
	}


#endif // 0
#pragma endregion

#pragma region 备选算法1--利用上下坡度查找
#if 1


	for (int i = 0; i < Seg; i++)
	{

		float s = sin(i * 360 / Seg * CV_PI / 180);
		float c = cos(i * 360 / Seg * CV_PI / 180);
		vector<int> Up2Down, Down2Up;
		for (int j = Rn; j < Rw; j++)
		{
			//Point sta = Point(Xos + j * s, Yos - j * c);

			Point2f sta = Point2f(Xos, Yos) + j * Point2f(s, -c);
			Point2f end0 = Point2f(Xos, Yos) + (j - 1) * Point2f(s, -c);
			Point2f end1 = Point2f(Xos, Yos) + (j + 1) * Point2f(s, -c);

			if (end1.x > nWidth || end1.x<0 || end1.y>nHeight || end1.y < 0)continue;

			int flag1 = img.at<uchar>(sta.y, sta.x) - img.at<uchar>(end0.y, end0.x);
			int flag2 = img.at<uchar>(end1.y, end1.x) - img.at<uchar>(sta.y, sta.x);

			if (flag1*flag2 < 0 && flag1<-Tho1 && flag2>Tho2) //暗点
			{
				line(output, sta, sta, Scalar(0, 0, 255), 1);
				Dpoint.push_back(sta);
				sum0++;
			}

			if (flag1*flag2 < 0 && flag2<-Tho1 && flag1>Tho2) //亮点
			{
				line(output, sta, sta, Scalar(0, 255, 0), 1);
				Lpoint.push_back(sta);
				sum1++;
			}

			//if (flag1 > 0)
			//{
			//	//亮点
			//	Down2Up.push_back(j);
			//}
			//if (flag1 < 0)
			//{
			//	//暗点
			//	Up2Down.push_back(j);
			//}
		}

		//if (Down2Up.size() == Up2Down.size())
		//{
		//	//int a = 1;
		//	a++;
		//}

		//int len = Up2Down.size();
		//if (Up2Down.size() > Down2Up.size())
		//{
		//	len = Down2Up.size();
		//}

		//for (size_t j = 0; j < len; j++)
		//{
		//	if (abs(Down2Up[j] - Up2Down[j]) >= 1 && abs(Down2Up[j] - Up2Down[j]) <= 3)
		//	{
		//		Point2f sta = Point2f(Xos, Yos) + (Down2Up[j] + Up2Down[j]) / 2 * Point2f(s, -c);
		//		line(output, sta, sta, Scalar(0, 255, 255), 1);
		//	}
		//}
	}

#pragma region 二次反查

	for (size_t i = 0; i < Lpoint.size(); i++)
	{
		Point2f sta = Lpoint[i];

		int flag = img.at<uchar>(sta.y, sta.x);
		float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y - 1, sta.x);
		_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
		_tmp += img.at<uchar>(sta.y, sta.x - 1);
		_tmp += img.at<uchar>(sta.y, sta.x + 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x);
		_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

		_tmp /= 8.0;

		float result = (flag - _tmp) / (flag + _tmp);
		//flag = abs(flag);

		if (result > Tho3)
		{
			line(output, sta, sta, Scalar(255, 0, 255), 2);
			summ1++;
		}

		if (result < -Tho4)
		{
			line(output, sta, sta, Scalar(0, 255, 255), 2);
			summ2++;
		}
	}

	for (size_t i = 0; i < Dpoint.size(); i++)
	{
		Point2f sta = Dpoint[i];

		int flag = img.at<uchar>(sta.y, sta.x);
		float _tmp = img.at<uchar>(sta.y - 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y - 1, sta.x);
		_tmp += img.at<uchar>(sta.y - 1, sta.x + 1);
		_tmp += img.at<uchar>(sta.y, sta.x - 1);
		_tmp += img.at<uchar>(sta.y, sta.x + 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x - 1);
		_tmp += img.at<uchar>(sta.y + 1, sta.x);
		_tmp += img.at<uchar>(sta.y + 1, sta.x + 1);

		_tmp /= 8.0;

		float result = (flag - _tmp) / (flag + _tmp);
		//flag = abs(flag);

		if (result > Tho3)
		{
			line(output, sta, sta, Scalar(255, 0, 255), 2);
			summ1++;
		}

		if (result < -Tho4)
		{
			line(output, sta, sta, Scalar(0, 255, 255), 2);
			summ2++;
		}
	}


#pragma endregion

	//OK_ss << "OK角度数:" << a << endl;
	OK_ss << "红点数:" << sum0 << endl;
	OK_ss << "绿点数:" << sum1 << endl;

	OK_ss << "有效红点数:" << summ1 << endl;
	OK_ss << "有效绿点数:" << summ2 << endl;

	//int result = 0;
	if (summ1 > Gg1 || summ2 > Gg1)
	{
		OK_ss << "C品" << endl;
		flag = 2;
	}
	else if (summ1 > Gg0 || summ2 > Gg0)
	{
		OK_ss << "B品" << endl;
		flag = 1;
	}
	else
	{
		OK_ss << "A品" << endl;
		flag = 0;

	}


#endif // 0
#pragma endregion

#pragma endregion

#pragma region 原图输出

	if (isGray == 0)
	{
		cvtColor(img, output, CV_GRAY2BGR);
	}
#pragma endregion

#pragma region 参数载入
	////string str = ;;
	//GetPrivateProfileString("Main", "multiple", "1", path, 1024, "./2.ini");
	//int multiple = stoi(path);//倍数

	//GetPrivateProfileString("Main", "gamma", "1", path, 1024, "./2.ini");
	//float gamma = stof(path);//Gamma

	//GetPrivateProfileString("Main", "r0", "1", path, 1024, "./2.ini");
	//int r0 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r1", "1", path, 1024, "./2.ini");
	//int r1 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r2", "1", path, 1024, "./2.ini");
	//int r2 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r3", "1", path, 1024, "./2.ini");
	//int r3 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r4", "1", path, 1024, "./2.ini");
	//int r4 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "r5", "1", path, 1024, "./2.ini");
	//int r5 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "threshold0", "128", path, 1024, "./2.ini");
	//int threshold0 = stoi(path);//倍数

	//GetPrivateProfileString("Main", "isGray", "1", path, 1024, "./2.ini");
	//int isGray = stoi(path);//倍数
#pragma endregion

#pragma region 改善原图
	//img *= multiple;

	//Mat Gamma;

	//MyGamma(img, Gamma, gamma);
	//img = Gamma.clone();
#pragma endregion

#pragma region 清除噪声
	//Mat maskEx;
	////Laplacian(img, maskEx, 8, 1);
	//blur(img, maskEx, Size(r2, r3));
#pragma endregion

#pragma region 获取白色孤立点
	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(r4, r5));
	//morphologyEx(maskEx, gray, CV_MOP_OPEN, element);

	//element = getStructuringElement(MORPH_ELLIPSE, Size(r0, r1));
	//morphologyEx(gray, gray, CV_MOP_TOPHAT, element);
	////morphologyEx(img, gray, CV_MOP_BLACKHAT, element);

	//threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值

	//maskEx = gray.clone();
	//blur(maskEx, gray, Size(r2, r3));

	//GetPrivateProfileString("Main", "path2", "", path, 1024, "./2.ini");
	//Mat img2 = imread(path, 0);

	//GetPrivateProfileString("Main", "threshold1", "", path, 1024, "./2.ini");
	//int threshold1 = stoi(path);
	//threshold(img, img, threshold1, 255, THRESH_TOZERO_INV);
	//threshold(img2, img2, threshold1, 255, THRESH_TOZERO_INV);

	//Mat dev = img - img2;

	////imshow("dev", dev);
	////waitKey(2);
#pragma endregion

#pragma endregion

#pragma region 图像预处理
	//cvtColor(gray, img_decode, CV_GRAY2BGR);

	//Mat gray_Temp = gray.clone();
	//blur(gray_Temp, gray, Size(3, 3));	//高斯滤波

	//threshold(gray, gray, th0, 255, THRESH_BINARY);//二值化阈值
	//findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
#pragma endregion

#pragma region 轮廓筛选
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	vector<Point2i> tmp;
	//	convexHull(contours[i], tmp);
	//	double tmpArea = contourArea(tmp);
	//	double tmpLen = arcLength(tmp, 0);

	//	if (tmpLen > 2 * CV_PI*r1 || tmpLen < 2 * CV_PI*r0) continue;
	//	if (tmpArea > CV_PI*r1*r1 || tmpArea < CV_PI*r0*r0) continue;

	//	if (tmpArea > area)
	//	{
	//		area = (long)tmpArea;
	//		MaxL = i;
	//	}
	//}
#pragma endregion

#pragma region 灰度显示
	//Mat Green = Mat(gray.rows, gray.cols, CV_8UC3, Scalar(0, 255, 0));
	//if (isGray)
	//{
	//	cvtColor(img, output, CV_GRAY2BGR);
	//	Green.copyTo(output, gray);
	//	//drawContours(output, contours, -1, Scalar(0, 255, 0), -1);
	//}
	//else
	//{
	//	cvtColor(gray, output, CV_GRAY2BGR);
	//	
	//}

	//if (MaxL < 0)
	//	goto OutPoint;
#pragma endregion

#pragma region 圆心确定
	//float radius;

	//convexHull(contours[MaxL], tmpCont);
	//ell = fitEllipse(tmpCont);
	//ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
	//center = ell.center;
	////deltaX = center.x - BzCenterX;
	////deltaY = center.y - BzCenterY;
	//radius = ell.size.width / 2;

	//OK_ss << "半径：" << radius << endl;
	////OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

	//ERR_ss << "半径：" << radius << endl;
	////ERR_ss << " " << endl;

	////这里需要增加一个以图像中心为远点的ROI边界绘制
	//circle(output, center, r0, Scalar(0, 255, 0), 2);
	//circle(output, center, r1, Scalar(0, 255, 0), 2);
#pragma endregion

#pragma region 轮廓判定
	//if (MaxL < 0)
	//{
	//	ERR_ss << "部品定位异常" << endl;
	//	goto OutPoint;
	//}
#pragma endregion

#pragma region 缺口查找
	//Ana_DefectDetection_2(img, GoodPoint, center, r2, r3, th1, size, (r2 - r3) * GPR0);

	//for (size_t i = 0; i < size; i++)
	//{
	//	float s = sin(i * 360 / size * CV_PI / 180);
	//	float c = cos(i * 360 / size * CV_PI / 180);
	//	Point2f end = center + r3 * Point2f(s, -c);
	//	Point2f sta = center + r2 * Point2f(s, -c);
	//	if (GoodPoint[i])//BG
	//	{
	//		circle(output, end, 1, Scalar(255, 0, 0), 2);
	//		circle(output, sta, 1, Scalar(255, 0, 0), 2);
	//	}
	//	else
	//	{
	//		line(output, end, sta, Scalar(0, 0, 255), 2);
	//		circle(output, end, 1, Scalar(0, 0, 255), 2);
	//		circle(output, sta, 1, Scalar(0, 0, 255), 2);
	//	}
	//}

	////Vec2i len = Ana_FindMaxLen(GoodPoint);
	////if (len[0] < 0)len[0] += size;
	////int angle = len[0] - len[1];

	//num = Ana_LenFilter(GoodPoint, QKLength);
	//if (num != resultNum)
	//{
	//	ERR_ss << "缺口数目不匹配：" << num << endl;
	//	goto OutPoint;
	//}

	//result = true;
	//OK_ss << "缺口个数：" << num << endl;

	//Para.deltaX = center.x - output.cols / 2;
	//Para.deltaY = center.y - output.rows / 2;
#pragma endregion

#pragma region 函数出口
	result = 0;
OutPoint:
#pragma endregion

#pragma region 文字输出
	//字体大小
	int text_Size;
	text_Size = ((nWidth* nHeight / 10000 - 30) * 0.078 + 25) * 2;
	//位置
	Point text_Localtion01;
	text_Localtion01.x = text_Size / 3;
	text_Localtion01.y = text_Size / 3;
	Point text_Localtion02;
	text_Localtion02.x = text_Size / 3;
	text_Localtion02.y = nHeight - text_Size * 4;
	Point text_Localtion03;
	text_Localtion03.x = text_Size / 3;
	text_Localtion03.y = nHeight - text_Size * 3;

	//ERR_ss << "\r" << endl;
	//OK_ss << "\r" << endl;

	std::string test;
	if (result != 0)
	{
		//test = ERR_ss.str();
		//putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	}
	else
	{
		test = OK_ss.str();
		putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
	}
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	SYSTEMTIME st;
	//	GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), output);
	//}
#pragma endregion		

#pragma region 不良镜片保存
	if (true)if (isCap != 0)
	{
		stringstream path;
#pragma region NG-Input图片输出
		//判断是否存在最新输出文件夹
		path << "D:\\picture2\\input";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), img);
#pragma endregion

#pragma region NG-Output图片输出
		path.str("");
		//判断是否存在最新输出文件夹
		path << "D:\\picture2\\Output";
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		path << "\\" << st.wYear << "-" << setfill('0') << setw(2) << st.wMonth << "-" << setfill('0') << setw(2) << st.wDay;
		if (!dirExists(path.str()))
		{
			CreateDirectory(path.str().c_str(), NULL);
		}
		//生成NG的输出图片
		path << "\\" << setfill('0') << setw(2) << st.wHour << "-" << setfill('0') << setw(2) << st.wMinute << "-" << setfill('0') << setw(2) << st.wSecond << ".bmp";
		imwrite(path.str(), output);
#pragma endregion
	}
#pragma endregion

#pragma region 参数输出

	//OutputX = 5;
	//OutputY = 5;
	//
	//tm.stop();
	//耗时
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = flag;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	//free(r4);
	//free(r5);
	//free(GPR1);
	//free(GPR2);
	//free(th2);

	free(path);
#pragma endregion

	
	return 0;
}

bool cut(int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
#pragma region 参数定义
	LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);

	//GetPrivateProfileString("Main", "path", "", path, 1024, "./MD.ini");
	//Mat img = imread(path, 0);
	//CV_Assert(img.data);

	//Mat img_Temp(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img = img_Temp.clone();

	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像
	//Mat img = img2.clone();
	//LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	//GetPrivateProfileString("Main", "Path", "", path, 1024, "./2.ini");
	//Mat img = imread(path, 0);

	//bool isGray = (bool)atoi(input_Parameter[0]);	//灰度显示 0表示显示原图 1表示显示灰度图

	//int r0 = atoi(input_Parameter[1]);		//定位-内环半径
	//int r1 = atoi(input_Parameter[2]);		//定位-外环半径
	//int th0 = atoi(input_Parameter[3]);	//定位-灰度阈值

	//int NoUse0 = 0;//占位但是不用，界面上也不需要显示
	//int QKLength = atoi(input_Parameter[4]);	//缺口-长度下限
	//int resultNum = atoi(input_Parameter[5]);	//缺口-缺口数量
	//int NoUse1 = 0;//占位但是不用，界面上也不需要显示

	//int r2 = atoi(input_Parameter[6]);		//缺口-外环半径
	//int r3 = atoi(input_Parameter[7]);			//缺口-内环半径
	//float GPR0 = atof(input_Parameter[8]);	//缺口-径向有效点比例
	////GPR0 /= 100;
	//int th1 = atoi(input_Parameter[9]);		//缺口-灰度阈值

	int x = atoi(input_Parameter[0]);		//缺口-外环半径
	int y = atoi(input_Parameter[1]);		//缺口-外环半径
	int w = atoi(input_Parameter[2]);		//缺口-外环半径
	int h = atoi(input_Parameter[3]);		//缺口-外环半径

	//vector<string> SaveImg;
	//int SaveImgNum = 0;
	//int CameraNum = 0;
	//int XTNum = 0;
	//SaveImg = Spilt2(input_Parameter[10]);
	//if (SaveImg.size() == 3)
	//{
	//	SaveImgNum = stoi(SaveImg[2]);
	//	CameraNum = stoi(SaveImg[0]);
	//	XTNum = stoi(SaveImg[1]);
	//}

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	//SYSTEMTIME st;
	//	//GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Input\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), img);
	//}
#pragma endregion

#pragma region 图像裁剪
	Mat color;

	cvtColor(img, color, CV_GRAY2BGR);

	Mat output(color, Rect(x, y, w, h));
#pragma endregion



#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	//if (SaveImgNum > 0 & SaveImgNum < 100)
	//{
	//	stringstream src_path;
	//	std::vector<String> filenames;
	//	src_path << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output";
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	if (filenames.size() >= SaveImgNum)
	//	{
	//		for (int i = 0; i < filenames.size(); i++)
	//		{
	//			remove(filenames[i].c_str());
	//		}
	//	}
	//	cv::glob(src_path.str(), filenames);//获取路径下所有文件名
	//	stringstream picfile;
	//	SYSTEMTIME st;
	//	GetLocalTime(&st);
	//	picfile << ".\\picture\\Camera0" << CameraNum << "\\" << "0" << XTNum << "\\Output\\" << (filenames.size() + 1) << ".bmp";
	//	imwrite(picfile.str(), output);
	//}
#pragma endregion		


#pragma region 参数输出

	//OutputX = 5;
	//OutputY = 5;
	//
	//tm.stop();
	//耗时
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//角度
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	//free(r4);
	//free(r5);
	//free(GPR1);
	//free(GPR2);
	//free(th2);

	free(path);
#pragma endregion


	return 0;
}

bool MV_EntryPoint(int type, int nHeight, int nWidth, BmpBuf &data, char** input_Parameter, float* output_Parameter_Float)
{
	switch (type)
	{
	//case 1:cut(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	//case 2:tmp3(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	//case 3:tmp2(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	case 0:MV_JT(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	case 1:MV_JP(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	case 2:MV_DGP(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	case 3:MV_JS(nHeight, nWidth, data, input_Parameter,  output_Parameter_Float); break;
	case 4:MV_MTJS(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	case 5:MV_TP(nHeight, nWidth, data, input_Parameter, output_Parameter_Float); break;
	default:
		break;
	}
	//Sleep(500);
	return 0;
}


void Make_BMPResult(int* resultArray)
{

    struct Point
{
    int X1;
    int Y1;
    int X2;
    int Y2;
};
Point _points[32];
/*for (int i = 0; i < 8; i++)
{
    _points[i].X1 = 6 ;16
    _points[i].Y1 = 2 + (8 * i);
    _points[i].X2 = _points[i].X1 +1;
    _points[i].Y2 = _points[i].Y1 + 1;

}*/
_points[0].X1 = 6; _points[0].Y1 = 2; _points[0].X2 = 7; _points[0].Y2 = 3;
	_points[1].X1 = 6; _points[1].Y1 = 10; _points[1].X2 = 7; _points[1].Y2 = 11;
	_points[2].X1 = 6; _points[2].Y1 = 19; _points[2].X2 = 7; _points[2].Y2 = 20;
	_points[3].X1 = 6; _points[3].Y1 = 27; _points[3].X2 = 7; _points[3].Y2 = 28;
	_points[4].X1 = 6; _points[4].Y1 = 36; _points[4].X2 = 7; _points[4].Y2 = 37;
	_points[5].X1 = 6; _points[5].Y1 = 44; _points[5].X2 = 7; _points[5].Y2 = 45;
	_points[6].X1 = 6; _points[6].Y1 = 53; _points[6].X2 = 7; _points[6].Y2 = 54;
	_points[7].X1 = 6; _points[7].Y1 = 61; _points[7].X2 = 7; _points[7].Y2 = 62;

	_points[8].X1 = 12; _points[8].Y1 = 61; _points[8].X2 = 13; _points[8].Y2 = 62;
	_points[9].X1 = 12; _points[9].Y1 = 53; _points[9].X2 = 13; _points[9].Y2 = 54;
	_points[10].X1 = 12; _points[10].Y1 = 44; _points[10].X2 = 13; _points[10].Y2 = 45;
	_points[11].X1 = 12; _points[11].Y1 = 36; _points[11].X2 = 13; _points[11].Y2 = 37;
	_points[12].X1 = 12; _points[12].Y1 = 27; _points[12].X2 = 13; _points[12].Y2 = 28;
	_points[13].X1 = 12; _points[13].Y1 = 19; _points[13].X2 = 13; _points[13].Y2 = 20;
	_points[14].X1 = 12; _points[14].Y1 = 10; _points[14].X2 = 13; _points[14].Y2 = 11;
	_points[15].X1 = 12; _points[15].Y1 = 2; _points[15].X2 = 13; _points[15].Y2 = 3;

	_points[16].X1 = 18; _points[16].Y1 = 2; _points[16].X2 = 19; _points[16].Y2 = 3;
	_points[17].X1 = 18; _points[17].Y1 = 10; _points[17].X2 = 19; _points[17].Y2 = 11;
	_points[18].X1 = 18; _points[18].Y1 = 19; _points[18].X2 = 19; _points[18].Y2 = 20;
	_points[19].X1 = 18; _points[19].Y1 = 27; _points[19].X2 = 19; _points[19].Y2 = 28;
	_points[20].X1 = 18; _points[20].Y1 = 36; _points[20].X2 = 19; _points[20].Y2 = 37;
	_points[21].X1 = 18; _points[21].Y1 = 44; _points[21].X2 = 19; _points[21].Y2 = 45;
	_points[22].X1 = 18; _points[22].Y1 = 53; _points[22].X2 = 19; _points[22].Y2 = 54;
	_points[23].X1 = 18; _points[23].Y1 = 61; _points[23].X2 = 19; _points[23].Y2 = 62;

	_points[24].X1 = 24; _points[24].Y1 = 61; _points[24].X2 = 25; _points[24].Y2 = 62;
	_points[25].X1 = 24; _points[25].Y1 = 53; _points[25].X2 = 25; _points[25].Y2 = 54;
	_points[26].X1 = 24; _points[26].Y1 = 44; _points[26].X2 = 25; _points[26].Y2 = 45;
	_points[27].X1 = 24; _points[27].Y1 = 36; _points[27].X2 = 25; _points[27].Y2 = 37;
	_points[28].X1 = 24; _points[28].Y1 = 27; _points[28].X2 = 25; _points[28].Y2 = 28;
	_points[29].X1 = 24; _points[29].Y1 = 19; _points[29].X2 = 25; _points[29].Y2 = 20;
	_points[30].X1 = 24; _points[30].Y1 = 10; _points[30].X2 = 25; _points[30].Y2 = 11;
	_points[31].X1 = 24; _points[31].Y1 = 2; _points[31].X2 = 25; _points[31].Y2 = 3;
	//int led_x[32] = { 24,24,25,25,18,18,19,19,1,1,1,1,1,1,4,8,11,15,15,11,8,4,4,8,11,15,15,11,8,4,1,1 };
	//int led_y[32] = { 61,62,61,62,61,62,61,62,7,10,14,18,21,25,25,25,25,25,21,21,21,21,18,18,18,18,14,14,14,14,1,1 };
	Mat result = Mat::zeros(32, 64, CV_8UC1);
	for (size_t i = 0; i< 32; i++)
	{
		if (resultArray[i] == 1)
		{
			result.at<uchar>(_points[i].X1, _points[i].Y1) = 255;
			result.at<uchar>(_points[i].X1, _points[i].Y2) = 255;
			result.at<uchar>(_points[i].X2, _points[i].Y1) = 255;
			result.at<uchar>(_points[i].X2, _points[i].Y2) = 255;
		}
	}

	imwrite("LEDResult.bmp", result);
}