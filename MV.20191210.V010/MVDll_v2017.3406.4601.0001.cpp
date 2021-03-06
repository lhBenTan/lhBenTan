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

bool img(int nHeight, int nWidth, BmpBuf &data)
{

	stringstream ss;
	Mat Color = imread("d:\\zsliujunj\\桌面\\1.bmp", 1);
	Mat Gray, img;

	Color -= Scalar(0, 20, 0);
	Color *= 8;
	//cvtColor(Gray, Color, CV_GRAY2BGR);
	//提取绿色通道
	{
		Mat mv[3];
		split(Color, mv);

		Gray = mv[1].clone();
	}

	//imshow("1", Gray);
	//waitKey(2);

	TickMeter tm;
	tm.start();

	Point2f center(0, 0);

	float radius;


	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	img = Gray.clone();


	//计算画面

	blur(Gray, Gray, Size(5, 5));
	threshold(Gray, Gray, 150, 255, CV_THRESH_BINARY);
	cv::findContours(Gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	int MaxL;
	LONG area = -1;

#pragma region 整段点胶
#if 0
	for (int i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][3] < 0)continue;
		int tmpArea = contourArea(contours[i]);
		if (tmpArea < 20000)continue;

		if (tmpArea > area)
		{
			area = tmpArea;
			MaxL = i;
		}
	}
	float num = 0;
	if (area > 0)
	{
		minEnclosingCircle(contours[MaxL], center, radius);
		circle(Color, center, radius, Scalar(0, 255, 0), 2);
		circle(Color, center, radius + 30, Scalar(0, 255, 0), 2);

		//判断胶圈完整
		float size = 360;
		vector<int> OutContour;
		Ana_DefectDetection_2(img, Color, OutContour, center, radius + 30, radius, 20, size, 1);
		//判断外圈是否有溢胶
		Ana_DefectDetection_2(img, Color, OutContour, center, radius + 40, radius + 35, 20, size, 0);
		//判断内圈是否有溢胶
		Ana_DefectDetection_2(img, Color, OutContour, center, radius - 5, radius - 10, 20, size, 0);
	}
	else
	{

		ss << "找不到胶圈" << endl;//rows

	}
#endif
#pragma endregion

#pragma region 三段点胶
	MaxL = Ana_FindBigestContours(contours);

	minEnclosingCircle(contours[MaxL], center, radius);

	vector<bool> OutContour[4];
	float size = 360;
	//外环溢胶检测
	Ana_DefectDetection_2(img, OutContour[0], center, 250, 245, 10, size, 3);

	//欠胶检测
	Ana_DefectDetection_2(img, OutContour[1], center, 210, 200, 10, size, 3);

	//胶环检测
	Ana_DefectDetection_2(img, OutContour[2], center, 240, 210, 10, size, 19);

	//内环溢胶检测
	Ana_DefectDetection_2(img, OutContour[3], center, 195, 190, 10, size, 3);

	vector<bool> test;

	//欠胶检测
	for (int i = 0; i < size; i++)
	{
		if (OutContour[2][i] && OutContour[1][i])
		{
			test.push_back(0);

			float s = sin(i*CV_PI / 180);
			float c = cos(i*CV_PI / 180);

			Point2f start = center + 210 * Point2f(s, -c);
			Point2f end = center + 240 * Point2f(s, -c);


			line(Color, start, end, Scalar(255, 255, 0), 1);
		}
		else
			test.push_back(1);
	}
	ss << "有效胶水数:" << Ana_LenFilter(test, 50) << endl;

	test.clear();
	for (int i = 0; i < size; i++)
	{
		if (OutContour[1][i] && OutContour[0][i])
		{
			test.push_back(0);

			float s = sin(i*CV_PI / 180);
			float c = cos(i*CV_PI / 180);

			Point2f start = center + 250 * Point2f(s, -c);
			Point2f end = center + 245 * Point2f(s, -c);


			line(Color, start, end, Scalar(255, 255, 0), 1);
		}
		else
			test.push_back(1);
	}
	if (Ana_LenFilter(test, 1))
		ss << "外环有溢胶" << endl;
	else
		ss << "外环点胶正常" << endl;

	test.clear();
	for (int i = 0; i < size; i++)
	{
		if (OutContour[1][i] && OutContour[3][i])
		{
			test.push_back(0);

			float s = sin(i*CV_PI / 180);
			float c = cos(i*CV_PI / 180);

			Point2f start = center + 190 * Point2f(s, -c);
			Point2f end = center + 195 * Point2f(s, -c);


			line(Color, start, end, Scalar(255, 255, 0), 1);
		}
		else
			test.push_back(1);
	}
	if (Ana_LenFilter(test, 1))
		ss << "内环有溢胶" << endl;
	else
		ss << "内环点胶正常" << endl;

	radius = 250;
	circle(Color, center, radius, Scalar(0, 0, 255), 2);

	radius = 245;
	circle(Color, center, radius, Scalar(0, 0, 255), 2);

	radius = 240;
	circle(Color, center, radius, Scalar(0, 255, 0), 2);

	radius = 210;
	circle(Color, center, radius, Scalar(255, 255, 0), 2);

	radius = 200;
	circle(Color, center, radius, Scalar(0, 255, 0), 2);

	radius = 195;
	circle(Color, center, radius, Scalar(0, 0, 255), 2);

	radius = 190;
	circle(Color, center, radius, Scalar(0, 0, 255), 2);
#pragma endregion

	//imshow("1", Color);
	//waitKey(2);
	//data.h = Color.rows;
	//data.w = Color.cols;

	tm.stop();
	ss << "时间 = " << tm.getTimeMilli() << endl;//rows
	putTextZH(Color, ss.str().c_str(), Point(0, 0 + 20), Scalar(0, 255, 0), 50, "黑体", 0, 0);
	//
	//flip(Color, Color, 1);  //rotate 180
	//flip(Color, Color, -1);  //rotate 180
	////Mat img_decode(nHeight, nWidth, CV_8UC1, data.data_Input);
	resize(Color, Color, Size(nWidth, nHeight), 0, 0, INTER_LINEAR);
	//输出
	std::vector<uchar>buf;
	cv::imencode(".bmp", Color, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	return 0;

}

bool MV_EntryPoint(int type,int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float);

bool MV_GW01_JT(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	return MV_EntryPoint(0, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
	 
#pragma region 载入图像
	//载入图像
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
	//MV_Release(data);
	//Mat img = imread("C:\\1.bmp", 0);
#pragma endregion

#pragma region 参数载入
	Redate Para;

	int MaxL = -1;
	long area = -1;
	TickMeter tm;
	stringstream ss;
	Mat gray, output;
	CONTOURS contours;
	vector<Vec4i> hierarchy;

	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数

	//ss << "镜筒检测" << endl;

	gray = img.clone();
	Mat img_decode;

	tm.start();


	int result = 1;//1是ng 0是ok

	float size = input_Parameter_Int[0];//扫描点数

	bool isGray = input_Parameter_Int[1];//是否灰度显示

	int threshold0 = input_Parameter_Int[2];//二值化阈值

	int Radius1 = input_Parameter_Int[3];//轮廓外径
	int Radius2 = input_Parameter_Int[4];//轮廓内径

	int Radius3 = input_Parameter_Int[5];//角度-计算轮廓-外径
	int Radius4 = input_Parameter_Int[6];//角度-计算轮廓-内径
	int threshold1 = input_Parameter_Int[7];//角度-灰度阈值
	float GPRate0 = input_Parameter_Int[8];//角度-有效点占比 0~1

	float Radius5 = input_Parameter_Int[9];//缺陷深度下限
	float Radius6 = input_Parameter_Int[10];//缺陷深度上限
	float GPRate1 = input_Parameter_Int[11];//合格点比例

	int Radius7 = input_Parameter_Int[12];//匹配轮廓-外径
	int Radius8 = input_Parameter_Int[13];//匹配轮廓-内径
	int threshold2 = input_Parameter_Int[14];//匹配-灰度阈值
	float GPRate2 = input_Parameter_Int[15];//匹配-有效点占比
	float GPRate3 = input_Parameter_Int[16];//匹配-有效角度占比
	float BzCenterX = input_Parameter_Int[17];//画面补正中心X坐标
	float BzCenterY = input_Parameter_Int[18];//画面补正中心Y坐标

	int BzAngle = input_Parameter_Int[19];//补正角度
	int isCap = input_Parameter_Int[20];//是否保存截图

	float rule_X = input_Parameter_Int[21];//比例尺
	float rule_Y = input_Parameter_Int[22];//比例尺
#pragma endregion

#pragma region 原图截图
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\JT\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), img);

	}

	tm.stop();
	//ss << "1" <<tm.getTimeMilli()<< endl;
	tm.start();
#pragma endregion

#pragma region 图像预处理

	cvtColor(gray, img_decode, CV_GRAY2BGR);
	Mat gray_Temp = gray.clone();
	blur(gray_Temp, gray, Size(5, 5));
	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
	//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1));
	//threshold(gray, gray, threshold0 + 70, 255, THRESH_TOZERO_INV);//二值化阈值
	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	tm.stop();
	//ss << "1" << tm.getTimeMilli() << endl;
	tm.start();
#pragma endregion
	/////////////////////////////////////////////
#pragma region 筛选特定大小的轮廓
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmpCont;
		convexHull(contours[i], tmpCont);
		double tmpArea = arcLength(tmpCont, 0);
		if (tmpArea > 2 * CV_PI*Radius1 || tmpArea < 2 * CV_PI*Radius2)continue;
		if (tmpArea > area)
		{
			//double Area = contourArea(tmpCont);
			//检测圆形是否规则
			//if (Area < Radius2*Radius2*CV_PI || Area > Radius1*Radius1*CV_PI)continue;
			area = (long)tmpArea;
			MaxL = i;
		}
	}
	//tm.stop();
	//ss << "2" << tm.getTimeMilli() << endl;
	//tm.start();
#pragma endregion

#pragma region 判定是否二值化显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
	//tm.stop();
	//ss << "3" << tm.getTimeMilli() << endl;
	//tm.start();
#pragma endregion

	for (int i = 0; i < 1; i++)
	{

#pragma region 检测轮廓是否有效
		if (MaxL<0 || 6>contours[MaxL].size())
		{
			ss << "无有效目标" << endl;
			break;
		}
		//tm.stop();
		//ss << "4" << tm.getTimeMilli() << endl;
		//tm.start();
#pragma endregion

#pragma region 检测圆度
		drawContours(output, contours, MaxL, Scalar(255, 0, 0));
		//tm.stop();
		//ss << "5" << tm.getTimeMilli() << endl;
		//tm.start();
#pragma endregion

#pragma region 获取圆心及半径
		Point2f center(0, 0);
		float radius;
		vector<Point2i> tmpCont;
		convexHull(contours[MaxL], tmpCont);
		RotatedRect ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(255, 0, 0), 2);
		center = ell.center;
		Para.deltaX = center.x - BzCenterX;
		Para.deltaY = center.y - BzCenterY;

		radius = ell.size.width / 2;
		ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
		ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;
		ss << "半径：" << radius << endl;
		//tm.stop();
		//ss << "6" << tm.getTimeMilli() << endl;
		//tm.start();

		//circle(output, center, Radius1, Scalar(0, 0, 255), 2);
		//circle(output, center, Radius2, Scalar(0, 0, 255), 2);
		//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 利用圆周灰度获取部品角度
		vector<bool> GoodPoint;
		//这里的Para1跟Para1是设置值
		cvtColor(img_decode, gray, CV_BGR2GRAY);
		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			Radius3,
			Radius4,
			threshold1,
			size,
			(Radius3 - Radius4) *GPRate0);

		if (0 >= GoodPoint.size())
		{
			ss << "轮廓过小" << endl;
			break;
		}

		//circle(output, center, Radius3, Scalar(0, 255, 0), 2);
		//circle(output, center, Radius4, Scalar(0, 255, 0), 2);

		//for (int i = 0; i < size; i++)
		//{
		//	float s = sin(i * 360 / size * CV_PI / 180);
		//	float c = cos(i * 360 / size * CV_PI / 180);
		//	Point2f start = center + para2 * Point2f(s, -c);
		//	Point2f end = center + para1 * Point2f(s, -c);
		//	if (GoodPoint[i])
		//	{
		//		line(output, start, end, Scalar(255, 255, 0), 1);
		//	}
		//	else
		//	{
		//		line(output, start, end, Scalar(0, 0, 255), 1);
		//	}
		//}

		Vec2i ang = Ana_FindMaxLen(GoodPoint);
		//ang[0] = (ang[0] + (int)size) % (int)size;
		Para.angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
		Para.angle = ((int)Para.angle + 360) % 360;

		ss << "角度：" << Para.angle << endl;

		line(output,
			center,
			Point(center.x + ell.size.width / 2 * sin(Para.angle * CV_PI / 180.0),
				center.y - ell.size.width / 2 * cos(Para.angle * CV_PI / 180.0)),
			Scalar(255, 0, 0),
			4);
		//tm.stop();
		//ss << "7" << tm.getTimeMilli() << endl;
		//tm.start();
#pragma endregion

#pragma region 遍历轮廓，获取部品方向(异常 待修改)
		//GoodPoint.clear();
		//float para[2] = { Radius5,Radius6 };
		//ss << "lunkuoyusndu:" << contours[MaxL].size() << endl;
		//Ana_DefectDetection_1(contours[MaxL], para, GoodPoint, center);

		//for (size_t i = 0; i < GoodPoint.size(); i++)
		//{
		//	if (GoodPoint[i])
		//		circle(output, contours[MaxL][i], 1, Scalar(0, 255, 0), 2);
		//	else
		//		circle(output, contours[MaxL][i], 1, Scalar(0, 0, 255), 2);
		//}

		//Vec2i len = Ana_FindMaxLen(GoodPoint);
		//Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
		//double ang2 = Ana_ComputeAngle(center, end);
		//line(output, center, end, Scalar(0, 255, 0), 1);

		//float sum = 0;
		//for (size_t i = 0; i < GoodPoint.size(); i++)
		//{
		//	sum += GoodPoint[i];
		//}
		//sum /= GoodPoint.size();
		//ss << "轮廓类圆率：" << sum << endl;

		//if (sum < GPRate1)
		//{
		//	ss << "部品外轮廓可能存在脏污" << endl;
		//	break;
		//}
		//tm.stop();
		//ss << "8" << tm.getTimeMilli() << endl;
		//tm.start();
		//ss << "轮廓距离角度:" << ang2 << endl;
#pragma endregion

#pragma region 利用圆周灰度进行面别匹配
		GoodPoint.clear();

		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			Radius7,
			Radius8,
			threshold2,
			size,
			(Radius7 - Radius8)*GPRate2);

		//circle(output, center, Radius7, Scalar(255, 0, 0), 2);
		//circle(output, center, Radius8, Scalar(255, 0, 0), 2);

		for (int i = 0; i < size; i++)
		{
			float s = sin(i * 360 / size * CV_PI / 180);
			float c = cos(i * 360 / size * CV_PI / 180);
			//Point2f start = center + Radius8 * Point2f(s, -c);
			Point2f end = center + (Radius7 + Radius8) * Point2f(s, -c) / 2;
			if (GoodPoint[i])//BGR
			{
				circle(output, end, 1, Scalar(0, 255, 0), 2);
				//line(output, start, end, Scalar(0, 255, 0), 1.5);
			}
			else
			{
				circle(output, end, 1, Scalar(0, 0, 255), 2);
				//line(output, start, end, Scalar(0, 0, 255), 1.5);
			}
		}

		////////////////////
		Para.delta = 0;
		for (size_t i = 0; i < GoodPoint.size(); i++)
		{
			Para.delta += GoodPoint[i];
		}
		Para.delta /= GoodPoint.size();
		ss << "匹配度:" << Para.delta << endl;

		if (Para.delta >= GPRate3)
		{
			Para.isMatched = 0;
		}
		else
		{
			Para.isMatched = 1;
			break;
		}

		//tm.stop();
		//ss << "9" << tm.getTimeMilli() << endl;
		//tm.start();
#pragma endregion
		result = 0;
		ss << "结果：" << result << endl;
		ss << "镜筒判定通过" << endl;
	}


	//ss << "偏差X = " << Para.deltaX << endl;
	//ss << "偏差Y = " << Para.deltaY << endl;
	if (isGray)
	{
		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}
	else
	{
		stringstream ss2;

		ss2 << "结果:" << result << endl;
		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}
	/////////////////////////////////////////////
#pragma region 结果图像输出
	tm.stop();
#pragma endregion

	//字体大小
	int text_Size;
	text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
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


	//return tm.getTimeMilli();

	//stringstream ss;
	/*ss << "时间 = " << tm.getTimeMilli() << endl;
	if (result == 0)
	{
		ss << "结果 = OK"  << endl;

	}
	else { ss << "结果 = NG" << endl; }
	ss << "偏差X = " << Para.deltaX << endl;
	ss << "偏差Y = " << Para.deltaY << endl;
	ss << "角度 = " << Para.angle << endl;
	ss << "二值化阈值："<< Para.threshold<< endl;

	std::string test = ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体");*/

	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\JTOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), output);
	}

	data.Height = output.rows;
	data.Width = output.cols;
	//输出
	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = static_cast<int>(buf.size() - 54);
	data.data_Output = new unsigned char[data.size];
	std::copy(buf.begin() + 54
		, buf.end(), data.data_Output);

	//Output.output_Parameter_Float[1] = distance;
	*output_Parameter_Int = tm.getTimeMilli();
	output_Parameter_Int++;

	*output_Parameter_Float = Para.angle + BzAngle;
	output_Parameter_Float++;

	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = Para.deltaX * rule_X;
	output_Parameter_Float++;
	*output_Parameter_Float = Para.deltaY * rule_Y;
	output_Parameter_Float++;
	//镜筒结果
	*output_Parameter_Float = result;
	output_Parameter_Float++;
	return 0;



}

bool MV_GW01_Lens(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	return MV_EntryPoint(1, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
#pragma region 载入图像

	//载入图像
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
	//MV_Release(data);
	//Mat img = imread("C:\\Image_20200701214915979.bmp", 0);
	Redate Para;

	int MaxL = -1;
	long area = -1;
	TickMeter tm;
	stringstream ss;
	Mat gray, output;
	CONTOURS contours;
	vector<Vec4i> hierarchy;

	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数

	gray = img.clone();
	Mat img_decode;

	tm.start();


	int result = 1;//1是ng 0是ok

	float size = input_Parameter_Int[0];//扫描点数

	bool isGray = input_Parameter_Int[1];//是否灰度显示

	int threshold0 = input_Parameter_Int[2];//二值化阈值

	int Radius1 = input_Parameter_Int[3];//轮廓外径
	int Radius2 = input_Parameter_Int[4];//轮廓内径

	int Radius3 = input_Parameter_Int[5];//角度-计算轮廓-外径
	int Radius4 = input_Parameter_Int[6];//角度-计算轮廓-内径
	int threshold1 = input_Parameter_Int[7];//角度-灰度阈值
	float GPRate0 = input_Parameter_Int[8];//角度-有效点占比 0~1

	float Radius5 = input_Parameter_Int[9];//缺陷深度下限
	float Radius6 = input_Parameter_Int[10];//缺陷深度上限
	float GPRate1 = input_Parameter_Int[11];//合格点比例

	int Radius7 = input_Parameter_Int[12];//匹配轮廓-外径
	int Radius8 = input_Parameter_Int[13];//匹配轮廓-内径
	int threshold2 = input_Parameter_Int[14];//匹配-灰度阈值
	float GPRate2 = input_Parameter_Int[15];//匹配-有效点占比
	float GPRate3 = input_Parameter_Int[16];//匹配-有效角度占比
	float BzCenterX = input_Parameter_Int[17];//画面补正中心X坐标
	float BzCenterY = input_Parameter_Int[18];//画面补正中心Y坐标

	int BzAngle = input_Parameter_Int[19];//补正角度
	int isCap = input_Parameter_Int[20];//是否保存截图

	float rule_X = input_Parameter_Int[21];//比例尺
	float rule_Y = input_Parameter_Int[22];//比例尺
#pragma region 图像预处理

	cvtColor(gray, img_decode, CV_GRAY2BGR);
	Mat gray_Temp = gray.clone();
	blur(gray_Temp, gray, Size(3, 3));
	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
	//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1));
	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值

	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\JP\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), img);

	}
#pragma endregion
	/////////////////////////////////////////////
#pragma region 筛选特定大小的轮廓
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmpCont;
		convexHull(contours[i], tmpCont);
		double tmpArea = arcLength(tmpCont, 0);
		if (tmpArea > 2 * CV_PI*Radius1 || tmpArea < 2 * CV_PI*Radius2)continue;
		if (tmpArea > area)
		{
			double Area = contourArea(tmpCont);
			//检测圆形是否规则
			if (Area < Radius2*Radius2*CV_PI || Area > Radius1*Radius1*CV_PI)continue;
			area = (long)tmpArea;
			MaxL = i;
		}
	}
#pragma endregion

#pragma region 判定是否二值化显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
#pragma endregion

	for (int i = 0; i < 1; i++)
	{
#pragma region 检测轮廓是否有效
		if (MaxL<0 || 6>contours[MaxL].size())
		{
			ss << "无有效目标" << endl;
			break;
		}
#pragma endregion

#pragma region 检测圆度
		drawContours(output, contours, MaxL, Scalar(255, 0, 0));
#pragma endregion

#pragma region 获取圆心及半径
		Point2f center(0, 0);
		float radius;
		vector<Point2i> tmpCont;
		convexHull(contours[MaxL], tmpCont);
		RotatedRect ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(255, 0, 0), 2);
		center = ell.center;
		Para.deltaX = center.x - BzCenterX;
		Para.deltaY = center.y - BzCenterY;

		radius = ell.size.width / 2;
		ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;

		ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

		circle(output, center, Radius1, Scalar(0, 0, 255), 2);
		circle(output, center, Radius2, Scalar(0, 255, 0), 2);
		//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 利用圆周灰度获取部品角度
		vector<bool> GoodPoint;
		//这里的Para1跟Para1是设置值
		cvtColor(img_decode, gray, CV_BGR2GRAY);
		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			Radius3,
			Radius4,
			threshold1,
			size,
			(Radius3 - Radius4) *GPRate0);

		if (0 >= GoodPoint.size())
		{
			ss << "轮廓过小" << endl;
			break;
		}

		//for (int i = 0; i < size; i++)
		//{
		//	float s = sin(i * 360 / size * CV_PI / 180);
		//	float c = cos(i * 360 / size * CV_PI / 180);
		//	Point2f start = center + para2 * Point2f(s, -c);
		//	Point2f end = center + para1 * Point2f(s, -c);
		//	if (GoodPoint[i])
		//	{
		//		line(output, start, end, Scalar(255, 255, 0), 1);
		//	}
		//	else
		//	{
		//		line(output, start, end, Scalar(0, 0, 255), 1);
		//	}
		//}

		Vec2i ang = Ana_FindMaxLen(GoodPoint);
		//ang[0] = (ang[0] + (int)size) % (int)size;
		Para.angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
		Para.angle = ((int)Para.angle + 360) % 360;

		ss << "角度：" << Para.angle << endl;

		line(output,
			center,
			Point(center.x + ell.size.width / 2 * sin(Para.angle * CV_PI / 180.0),
				center.y - ell.size.width / 2 * cos(Para.angle * CV_PI / 180.0)),
			Scalar(255, 0, 0),
			4);
#pragma endregion

#pragma region 遍历轮廓，获取部品方向
		//GoodPoint.clear();
		//float para[2] = { Radius5,Radius6 };
		//Ana_DefectDetection_1(contours[MaxL], para, GoodPoint, center);

		//for (size_t i = 0; i < GoodPoint.size(); i++)
		//{
		//	if (GoodPoint[i])
		//		circle(output, contours[MaxL][i], 1, Scalar(0, 255, 0), 2);
		//	else
		//		circle(output, contours[MaxL][i], 1, Scalar(0, 0, 255), 2);
		//}

		//Vec2i len = Ana_FindMaxLen(GoodPoint);
		//Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
		//double ang2 = Ana_ComputeAngle(center, end);
		//line(output, center, end, Scalar(0, 255, 0), 1);

		//float sum = 0;
		//for (size_t i = 0; i < GoodPoint.size(); i++)
		//{
		//	sum += GoodPoint[i];
		//}
		//sum /= GoodPoint.size();
		//ss << "轮廓类圆率：" << sum << endl;

		//if (sum < GPRate1)
		//{
		//	ss << "部品外轮廓可能存在脏污" << endl;
		//	break;
		//}

		//ss << "轮廓距离角度:" << ang2 << endl;
#pragma endregion

#pragma region 利用圆周灰度进行面别匹配
		GoodPoint.clear();

		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			Radius7,
			Radius8,
			threshold2,
			size,
			(Radius7 - Radius8)*GPRate2);

		for (int i = 0; i < size; i++)
		{
			float s = sin(i * 360 / size * CV_PI / 180);
			float c = cos(i * 360 / size * CV_PI / 180);
			Point2f end = center + Radius7 * Point2f(s, -c);
			if (GoodPoint[i])//BGR
			{
				circle(output, end, 1, Scalar(0, 255, 0), 2);
			}
			else
			{
				circle(output, end, 1, Scalar(0, 0, 255), 2);
			}
		}

		////////////////////
		Para.delta = 0;
		for (size_t i = 0; i < GoodPoint.size(); i++)
		{
			Para.delta += GoodPoint[i];
		}
		Para.delta /= GoodPoint.size();
		ss << "匹配度:" << Para.delta << "\\" << GPRate3 << endl;
#pragma endregion

#pragma region 结果综合判定
		if (Para.delta >= GPRate3)
		{
			Para.isMatched = 0;
		}
		else
		{
			Para.isMatched = 1;
			break;
		}

		result = 0;
		//break;
#pragma endregion
	}
	//ss << "镜片测试：" << result << endl;
	//ss << "偏差X = " << Para.deltaX << endl;
	//ss << "偏差Y = " << Para.deltaY << endl;
	if (isGray)
	{
		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}
	else {
		stringstream ss2;

		ss2 << "结果:" << result << endl;
		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}
	/////////////////////////////////////////////
#pragma region 结果图像输出


	tm.stop();
#pragma endregion

	//字体大小
	int text_Size;
	text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
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


	//return tm.getTimeMilli();

	//stringstream ss;
	/*ss << "时间 = " << tm.getTimeMilli() << endl;
	if (result == 0)
	{
		ss << "结果 = OK"  << endl;

	}
	else { ss << "结果 = NG" << endl; }
	ss << "偏差X = " << Para.deltaX << endl;
	ss << "偏差Y = " << Para.deltaY << endl;
	ss << "角度 = " << Para.angle << endl;
	ss << "二值化阈值："<< Para.threshold<< endl;

	std::string test = ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体");*/
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\JPOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), output);

	}
	data.Height = output.rows;
	data.Width = output.cols;
	//输出
	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = static_cast<int>(buf.size() - 54);
	data.data_Output = new unsigned char[data.size];
	std::copy(buf.begin() + 54
		, buf.end(), data.data_Output);

	//Output.output_Parameter_Float[1] = distance;
	*output_Parameter_Int = tm.getTimeMilli();
	output_Parameter_Int++;

	*output_Parameter_Float = Para.angle +(360-BzAngle);;
	output_Parameter_Float++;

	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = Para.deltaX * rule_X;
	output_Parameter_Float++;
	*output_Parameter_Float = Para.deltaY * rule_Y;
	output_Parameter_Float++;
	//镜筒结果
	*output_Parameter_Float = result;
	output_Parameter_Float++;
	return 0;

#pragma endregion
}

bool MV_GW01_DGP(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	return MV_EntryPoint(2, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
	//载入图像
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
	//MV_Release(data);
	//Mat img = imread("C:\\Users\\10056\\Desktop\\tlh\\Image_20200908232018546.bmp", 0);

	string systemtime;
	stringstream picfile;

	SYSTEMTIME st;
	GetLocalTime(&st);
	picfile << ".\\picture\\DGP\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
	imwrite(picfile.str(), img);


	Redate Para;
	int result = false;
	//Mat img = imread("D:\\projrct\\机器视觉图像匹配\\20200103\\20200103\\9-903\\Image_20200105160407605.bmp", 0);

	Para.threshold = input_Parameter_Int[0];
	Para.minRadiius = input_Parameter_Int[1];
	Para.maxRadiius = input_Parameter_Int[2];

	int Ana_DefectDetection_2_01 = input_Parameter_Int[3];
	int Ana_DefectDetection_2_02 = input_Parameter_Int[4];
	Para.radiusThr = input_Parameter_Int[5];
	Para.isGray = 1;

	int resultNum = input_Parameter_Int[6];
	int QKLength = input_Parameter_Int[7];
	float OutputAngle = 0;
	float OutputX = 0;
	float OutputY = 0;


	Mat Color, Gray;

	TickMeter tm;
	tm.start();

	Mat gray, output;
	CONTOURS contours;
	vector<Vec4i> hierarchy;

	gray = img.clone();
	Mat img_decode;

	cvtColor(gray, img_decode, CV_GRAY2BGR);
	//gray = gray - 20;
	//gray *= 15;
	//imshow("1", gray);
	//waitKey(2);

	Mat gray2 = gray.clone();
	blur(gray2, gray, Size(3, 3));
	//gray = 255 - gray;
	threshold(gray, gray, Para.threshold, 255, THRESH_BINARY);
	//adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, Para.threshold);
	//blur(gray, gray, Size(3, 3));
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	int MaxL = -1;
	long area = -1;

	for (int i = 0; i < contours.size(); i++)
	{
#if 1
		vector<Point2i> tmpCont;
		convexHull(contours[i], tmpCont);
		int tmpArea = arcLength(tmpCont, 0);
		if (tmpArea > 2 * CV_PI*Para.maxRadiius || tmpArea < 2 * CV_PI*Para.minRadiius)continue;

		//double area = contourArea(tmpCont,0);
		//if (area > CV_PI*Para.maxRadiius*Para.maxRadiius || area < CV_PI*Para.minRadiius*Para.minRadiius)
		//	continue;

#else
		int tmpArea = arcLength(contours[i], 0);
#endif

		if (tmpArea > area)
		{
			area = tmpArea;
			MaxL = i;
		}
	}

	if (Para.isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();

	//result = true;
	/////////////////////////////////////////////
	stringstream ss;
	for (int i = 0; i < 1; i++)
	{
		if (MaxL < 0 || 6>contours[MaxL].size())break;

		Point2f center(0, 0);
		float radius;

		vector<Point2i> tmpCont;
		convexHull(contours[MaxL], tmpCont);
		RotatedRect ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(0, 0, 255), 5);
		//minEnclosingCircle(contours[MaxL], center, radius);
		center = ell.center;

		//这里是需要判断的数据 0.99
		Para.delta = ell.size.width / ell.size.height;
		//drawContours(output, contours, MaxL, Scalar(0, 255, 0), 2);

		vector<bool> GoodPoint;
		float size = 720;
		//这里的315跟290是设置值
		Ana_DefectDetection_2(img, GoodPoint, center, Ana_DefectDetection_2_01, Ana_DefectDetection_2_02, Para.radiusThr, size, (Ana_DefectDetection_2_01 - Ana_DefectDetection_2_02) / 1.05);
		circle(output, center, Ana_DefectDetection_2_02, Scalar(0, 0, 255), 2);
		circle(output, center, Ana_DefectDetection_2_01, Scalar(0, 255, 0), 2);
		for (int i = 0; i < size; i++)
		{
			float s = sin(i * 360 / size * CV_PI / 180);
			float c = cos(i * 360 / size * CV_PI / 180);
			Point2f start = center + Ana_DefectDetection_2_02 * Point2f(s, -c);
			Point2f end = center + Ana_DefectDetection_2_01 * Point2f(s, -c);
			if (GoodPoint[i])
			{
				line(output, start, end, Scalar(255, 255, 0), 2);
			}
			else
			{
				line(output, start, end, Scalar(0, 0, 255), 2);
			}
		}
		Vec2i len = Ana_FindMaxLen(GoodPoint);
		if (len[0] < 0)len[0] += size;
		Para.deltaX = len[0] - len[1];
		//这里25是设置参数，返回的angle是缺口个数 ==1
		Para.angle = Ana_LenFilter(GoodPoint, QKLength);

		circle(output, center, Para.maxRadiius, Scalar(0, 0, 255), 2);
		circle(output, center, Para.minRadiius, Scalar(0, 255, 0), 2);
		circle(output, center, Para.radius, Scalar(0, 255, 255), 2);


		//判断依据

		if (Para.angle != resultNum)
		{
			ss << "缺口数目不匹配：" << Para.angle << endl;
			break;
		}

		if (Para.delta < 0.9)
		{
			ss << "圆度不足" << endl;
			break;
		}

		result = true;

		ss << "圆度：" << Para.delta << endl;
		ss << "缺口个数：" << Para.angle << endl;

		Para.deltaX = center.x - output.cols / 2;
		Para.deltaY = center.y - output.rows / 2;
	}
	ss << "挡光片";

	float Lens_NG = 1;//不良
	if (result == true)
	{
		Lens_NG = 0;
		ss << ":OK" << endl;
	}
	else
	{
		ss << ":NG" << endl;
	}
	OutputX = 0;
	OutputY = 0;

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

	std::string test = ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);

	data.Height = output.rows;
	data.Width = output.cols;

	GetLocalTime(&st);
	stringstream picfile1;
	picfile1 << ".\\picture\\DGPOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
	imwrite(picfile1.str(), output);

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);

	*output_Parameter_Int = tm.getTimeMilli();
	output_Parameter_Int++;
	//OutputX = 5;
	//OutputY = 5;
	//角度
	*output_Parameter_Float = OutputAngle;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = OutputX;
	output_Parameter_Float++;
	*output_Parameter_Float = OutputY;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = Lens_NG;
	output_Parameter_Float++;


	return 0;
}

bool MV_DJ01(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	return MV_EntryPoint(0, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
#pragma region 载入图像

	//载入图像
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);
	MV_Release(data);
	//Mat img_decode = imread("D:\\projrct\\机器视觉图像匹配\\20200103\\20200103\\新建文件夹\\1-空镜筒.bmp", 0);
	Redate Para;

	int MaxL = -1;
	long area = -1;
	TickMeter tm;
	stringstream ss;
	Mat gray, output;
	CONTOURS contours;
	vector<Vec4i> hierarchy;

	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数

	gray = img.clone();
	Mat img_decode;

	tm.start();


	int result = 1;//1是ng 0是ok

	float size = input_Parameter_Int[0];//扫描点数

	bool isGray = input_Parameter_Int[1];//是否灰度显示

	int threshold0 = input_Parameter_Int[2];//二值化阈值

	int Radius1 = input_Parameter_Int[3];//轮廓外径
	int Radius2 = input_Parameter_Int[4];//轮廓内径

	int Radius3 = input_Parameter_Int[5];//角度-计算轮廓-外径
	int Radius4 = input_Parameter_Int[6];//角度-计算轮廓-内径
	int threshold1 = input_Parameter_Int[7];//角度-灰度阈值
	float GPRate0 = input_Parameter_Int[8];//角度-有效点占比 0~1

	float Radius5 = input_Parameter_Int[9];//缺陷深度下限
	float Radius6 = input_Parameter_Int[10];//缺陷深度上限
	float GPRate1 = input_Parameter_Int[11];//合格点比例

	int Radius7 = input_Parameter_Int[12];//匹配轮廓-外径
	int Radius8 = input_Parameter_Int[13];//匹配轮廓-内径
	int threshold2 = input_Parameter_Int[14];//匹配-灰度阈值
	float GPRate2 = input_Parameter_Int[15];//匹配-有效点占比
	float GPRate3 = input_Parameter_Int[16];//匹配-有效角度占比
	float BzCenterX = input_Parameter_Int[17];//画面补正中心X坐标
	float BzCenterY = input_Parameter_Int[18];//画面补正中心Y坐标

	int BzAngle = input_Parameter_Int[19];//补正角度
	int isCap = input_Parameter_Int[20];//是否保存截图

	float rule_X = input_Parameter_Int[21];//比例尺
	float rule_Y = input_Parameter_Int[22];//比例尺
#pragma region 图像预处理
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\DJ\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), img);

	}
	cvtColor(gray, img_decode, CV_GRAY2BGR);
	Mat gray_Temp = gray.clone();
	blur(gray_Temp, gray, Size(3, 3));
	//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
	//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1));
	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
#pragma endregion
	/////////////////////////////////////////////
#pragma region 筛选特定大小的轮廓
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmpCont;
		convexHull(contours[i], tmpCont);
		double tmpArea = arcLength(tmpCont, 0);
		if (tmpArea > 2 * CV_PI*Radius1 || tmpArea < 2 * CV_PI*Radius2)continue;
		if (tmpArea > area)
		{
			//double Area = contourArea(tmpCont);
			//检测圆形是否规则
			//if (Area < Radius2*Radius2*CV_PI || Area > Radius1*Radius1*CV_PI)continue;
			area = (long)tmpArea;
			MaxL = i;
		}
	}
#pragma endregion

#pragma region 判定是否二值化显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
#pragma endregion

	for (int i = 0; i < 1; i++)
	{
#pragma region 检测轮廓是否有效
		if (MaxL<0 || 6>contours[MaxL].size())
		{
			ss << "无有效目标" << endl;
			break;
		}
#pragma endregion

#pragma region 检测圆度
		drawContours(output, contours, MaxL, Scalar(255, 0, 0));
#pragma endregion

#pragma region 获取圆心及半径
		Point2f center(0, 0);
		float radius;
		vector<Point2i> tmpCont;
		convexHull(contours[MaxL], tmpCont);
		RotatedRect ell = fitEllipse(tmpCont);
		ellipse(output, ell, Scalar(255, 0, 0), 2);
		center = ell.center;
		Para.deltaX = center.x - BzCenterX;
		Para.deltaY = center.y - BzCenterY;

		radius = ell.size.width / 2;
		ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;

		ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

		circle(output, center, Radius1, Scalar(0, 0, 255), 2);
		circle(output, center, Radius2, Scalar(0, 255, 0), 2);
		//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 利用圆周灰度获取部品角度
		vector<bool> GoodPoint;
		//这里的Para1跟Para1是设置值
		cvtColor(img_decode, gray, CV_BGR2GRAY);
		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			Radius3,
			Radius4,
			threshold1,
			size,
			(Radius3 - Radius4) *GPRate0);

		if (0 >= GoodPoint.size())
		{
			ss << "轮廓过小" << endl;
			break;
		}

		//for (int i = 0; i < size; i++)
		//{
		//	float s = sin(i * 360 / size * CV_PI / 180);
		//	float c = cos(i * 360 / size * CV_PI / 180);
		//	Point2f start = center + para2 * Point2f(s, -c);
		//	Point2f end = center + para1 * Point2f(s, -c);
		//	if (GoodPoint[i])
		//	{
		//		line(output, start, end, Scalar(255, 255, 0), 1);
		//	}
		//	else
		//	{
		//		line(output, start, end, Scalar(0, 0, 255), 1);
		//	}
		//}

		Vec2i ang = Ana_FindMaxLen(GoodPoint);
		//ang[0] = (ang[0] + (int)size) % (int)size;
		Para.angle = (int)((ang[1] + ang[0]) * 360 / size / 2);
		Para.angle = ((int)Para.angle + 360) % 360;

		ss << "角度：" << Para.angle << endl;

		line(output,
			center,
			Point(center.x + ell.size.width / 2 * sin(Para.angle * CV_PI / 180.0),
				center.y - ell.size.width / 2 * cos(Para.angle * CV_PI / 180.0)),
			Scalar(255, 0, 0),
			4);
#pragma endregion

#pragma region 遍历轮廓，获取部品方向
		//GoodPoint.clear();
		//float para[2] = { Radius5,Radius6 };
		//Ana_DefectDetection_1(contours[MaxL], para, GoodPoint, center);

		//for (size_t i = 0; i < GoodPoint.size(); i++)
		//{
		//	if (GoodPoint[i])
		//		circle(output, contours[MaxL][i], 1, Scalar(0, 255, 0), 2);
		//	else
		//		circle(output, contours[MaxL][i], 1, Scalar(0, 0, 255), 2);
		//}

		//Vec2i len = Ana_FindMaxLen(GoodPoint);
		//Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
		//double ang2 = Ana_ComputeAngle(center, end);
		//line(output, center, end, Scalar(0, 255, 0), 1);

		//float sum = 0;
		//for (size_t i = 0; i < GoodPoint.size(); i++)
		//{
		//	sum += GoodPoint[i];
		//}
		//sum /= GoodPoint.size();
		//ss << "轮廓类圆率：" << sum << endl;

		//if (sum < GPRate1)
		//{
		//	ss << "部品外轮廓可能存在脏污" << endl;
		//	break;
		//}

		//ss << "轮廓距离角度:" << ang2 << endl;
#pragma endregion

#pragma region 利用圆周灰度进行面别匹配
		GoodPoint.clear();

		Ana_DefectDetection_2(gray,
			GoodPoint,
			center,
			Radius7,
			Radius8,
			threshold2,
			size,
			(Radius7 - Radius8)*GPRate2);

		for (int i = 0; i < size; i++)
		{
			float s = sin(i * 360 / size * CV_PI / 180);
			float c = cos(i * 360 / size * CV_PI / 180);
			Point2f end = center + Radius7 * Point2f(s, -c);
			if (GoodPoint[i])//BGR
			{
				circle(output, end, 1, Scalar(0, 255, 0), 2);
			}
			else
			{
				circle(output, end, 1, Scalar(0, 0, 255), 2);
			}
		}

		////////////////////
		Para.delta = 0;
		for (size_t i = 0; i < GoodPoint.size(); i++)
		{
			Para.delta += GoodPoint[i];
		}
		Para.delta /= GoodPoint.size();
		ss << "匹配度:" << Para.delta << endl;

#pragma endregion

#pragma region 结果综合判定
		if (Para.delta >= GPRate3)
		{
			Para.isMatched = 0;

		}
		else
		{
			Para.isMatched = 1;
			break;
		}

		result = 0;
		//break;
#pragma endregion
	}
	//ss << "偏差X = " << Para.deltaX << endl;
	//ss << "偏差Y = " << Para.deltaY << endl;
	//ss << "结果:" << result << endl;
	if (isGray)
	{
		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}
	else {
		stringstream ss2;

		ss2 << "结果:" << result << endl;
		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}
	/////////////////////////////////////////////
#pragma region 结果图像输出


	tm.stop();
#pragma endregion

	//字体大小
	int text_Size;
	text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
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


	//return tm.getTimeMilli();

	//stringstream ss;
	/*ss << "时间 = " << tm.getTimeMilli() << endl;
	if (result == 0)
	{
		ss << "结果 = OK"  << endl;

	}
	else { ss << "结果 = NG" << endl; }
	ss << "偏差X = " << Para.deltaX << endl;
	ss << "偏差Y = " << Para.deltaY << endl;
	ss << "角度 = " << Para.angle << endl;
	ss << "二值化阈值："<< Para.threshold<< endl;

	std::string test = ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体");*/
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\DJOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), output);

	}
	data.Height = output.rows;
	data.Width = output.cols;
	//输出
	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = static_cast<int>(buf.size() - 54);
	data.data_Output = new unsigned char[data.size];
	std::copy(buf.begin() + 54
		, buf.end(), data.data_Output);

	//Output.output_Parameter_Float[1] = distance;
	*output_Parameter_Int = tm.getTimeMilli();
	output_Parameter_Int++;

	*output_Parameter_Float = Para.angle + BzAngle;
	output_Parameter_Float++;

	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = Para.deltaX*rule_X;
	output_Parameter_Float++;
	*output_Parameter_Float = Para.deltaY*rule_Y;
	output_Parameter_Float++;
	//镜筒结果
	*output_Parameter_Float = result;
	output_Parameter_Float++;
	return 0;

#pragma endregion
}

//bool MV_PJ01(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
//
//{
//
//	stringstream ss;
//	Mat img(nHeight, nWidth, CV_8UC3, data.data_Input);
//	Mat gray, output;
//	cvtColor(img, gray, CV_BGR2GRAY);
//	output = img.clone();
////	Mat Color = imread("d:\\zsliujunj\\桌面\\1.bmp", 1);
//	//Mat Gray, img;
//
////
////	Color -= Scalar(0, 20, 0);
////	Color *= 8;
////cvtColor(img, img, CV_GRAY2BGR);
////	//提取绿色通道
////	{
////		Mat mv[3];
////		split(Color, mv);
////
////		Gray = mv[1].clone();
////	}
////
////	//imshow("1", Gray);
////	//waitKey(2);
////
////	TickMeter tm;
////	tm.start();
////
////	Point2f center(0, 0);
////
////	float radius;
////
////
////	vector<vector<Point>> contours;
////	vector<Vec4i> hierarchy;
////
////	img = Gray.clone();
////
////
////	//计算画面
////	Mat Gray_Blur = Gray.clone();
////	blur(Gray_Blur, Gray, Size(5, 5));
////	threshold(Gray, Gray, 150, 255, CV_THRESH_BINARY);
////	cv::findContours(Gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
////
////	int MaxL;
////	LONG area = -1;
////
////#pragma region 整段点胶
////#if 0
////	for (int i = 0; i < contours.size(); i++)
////	{
////		if (hierarchy[i][3] < 0)continue;
////		int tmpArea = contourArea(contours[i]);
////		if (tmpArea < 20000)continue;
////
////		if (tmpArea > area)
////		{
////			area = tmpArea;
////			MaxL = i;
////		}
////	}
////	float num = 0;
////	if (area > 0)
////	{
////		minEnclosingCircle(contours[MaxL], center, radius);
////		circle(Color, center, radius, Scalar(0, 255, 0), 2);
////		circle(Color, center, radius + 30, Scalar(0, 255, 0), 2);
////
////		//判断胶圈完整
////		float size = 360;
////		vector<int> OutContour;
////		Ana_DefectDetection_2(img, Color, OutContour, center, radius + 30, radius, 20, size, 1);
////		//判断外圈是否有溢胶
////		Ana_DefectDetection_2(img, Color, OutContour, center, radius + 40, radius + 35, 20, size, 0);
////		//判断内圈是否有溢胶
////		Ana_DefectDetection_2(img, Color, OutContour, center, radius - 5, radius - 10, 20, size, 0);
////	}
////	else
////	{
////
////		ss << "找不到胶圈" << endl;//rows
////
////	}
////#endif
////#pragma endregion
////
////#pragma region 三段点胶
////	MaxL = Ana_FindBigestContours(contours);
////
////	minEnclosingCircle(contours[MaxL], center, radius);
////
////	vector<bool> OutContour[4];
////	float size = 360;
////	//外环溢胶检测
////	Ana_DefectDetection_2(img, OutContour[0], center, 250, 245, 10, size, 3);
////
////	//欠胶检测
////	Ana_DefectDetection_2(img, OutContour[1], center, 210, 200, 10, size, 3);
////
////	//胶环检测
////	Ana_DefectDetection_2(img, OutContour[2], center, 240, 210, 10, size, 19);
////
////	//内环溢胶检测
////	Ana_DefectDetection_2(img, OutContour[3], center, 195, 190, 10, size, 3);
////
////	vector<bool> test;
////
////	//欠胶检测
////	for (int i = 0; i < size; i++)
////	{
////		if (OutContour[2][i] && OutContour[1][i])
////		{
////			test.push_back(0);
////
////			float s = sin(i*CV_PI / 180);
////			float c = cos(i*CV_PI / 180);
////
////			Point2f start = center + 210 * Point2f(s, -c);
////			Point2f end = center + 240 * Point2f(s, -c);
////
////
////			line(Color, start, end, Scalar(255, 255, 0), 1);
////		}
////		else
////			test.push_back(1);
////	}
////	ss << "有效胶水数:" << Ana_LenFilter(test, 50) << endl;
////
////	test.clear();
////	for (int i = 0; i < size; i++)
////	{
////		if (OutContour[1][i] && OutContour[0][i])
////		{
////			test.push_back(0);
////
////			float s = sin(i*CV_PI / 180);
////			float c = cos(i*CV_PI / 180);
////
////			Point2f start = center + 250 * Point2f(s, -c);
////			Point2f end = center + 245 * Point2f(s, -c);
////
////
////			line(Color, start, end, Scalar(255, 255, 0), 1);
////		}
////		else
////			test.push_back(1);
////	}
////	if (Ana_LenFilter(test, 1))
////		ss << "外环有溢胶" << endl;
////	else
////		ss << "外环点胶正常" << endl;
////
////	test.clear();
////	for (int i = 0; i < size; i++)
////	{
////		if (OutContour[1][i] && OutContour[3][i])
////		{
////			test.push_back(0);
////
////			float s = sin(i*CV_PI / 180);
////			float c = cos(i*CV_PI / 180);
////
////			Point2f start = center + 190 * Point2f(s, -c);
////			Point2f end = center + 195 * Point2f(s, -c);
////
////
////			line(Color, start, end, Scalar(255, 255, 0), 1);
////		}
////		else
////			test.push_back(1);
////	}
////	if (Ana_LenFilter(test, 1))
////		ss << "内环有溢胶" << endl;
////	else
////		ss << "内环点胶正常" << endl;
////
////	radius = 250;
////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
////
////	radius = 245;
////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
////
////	radius = 240;
////	circle(Color, center, radius, Scalar(0, 255, 0), 1);
////
////	radius = 210;
////	circle(Color, center, radius, Scalar(255, 255, 0), 1);
////
////	radius = 200;
////	circle(Color, center, radius, Scalar(0, 255, 0), 1);
////
////	radius = 195;
////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
////
////	radius = 190;
////	circle(Color, center, radius, Scalar(0, 0, 255), 1);
////#pragma endregion
////
////	//imshow("1", Color);
////	//waitKey(2);
////	//data.h = Color.rows;
////	//data.w = Color.cols;
////
////	tm.stop();
////	ss << "时间 = " << tm.getTimeMilli() << endl;//rows
////	putTextZH(Color, ss.str().c_str(), Point(0, 0 + 20), Scalar(0, 255, 0), 50, "黑体", 0, 0);
//	//
//	//flip(Color, Color, 1);  //rotate 180
//	//flip(Color, Color, -1);  //rotate 180
//	////Mat img_decode(nHeight, nWidth, CV_8UC1, data.data_Input);
//	//resize(Color, Color, Size(nWidth, nHeight), 0, 0, INTER_LINEAR);
//
////stringstream ss;
//ss << "判胶" << endl;
//
////字体大小
//int text_Size;
//text_Size = 2 * ((nWidth* nHeight / 10000 - 30) * 0.078 + 25);
////位置
//Point text_Localtion01;
//text_Localtion01.x = text_Size / 3;
//text_Localtion01.y = text_Size / 3;
//Point text_Localtion02;
//text_Localtion02.x = text_Size / 3;
//text_Localtion02.y = nHeight - text_Size * 4;
//Point text_Localtion03;
//text_Localtion03.x = text_Size / 3;
//text_Localtion03.y = nHeight - text_Size * 3;
//
//
//	std::string test = ss.str();
//	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体",0);
//
//
//	//输出
//	std::vector<uchar>buf;
//	cv::imencode(".bmp", output, buf);
//	data.size = buf.size() - 54;
//	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
//	std::copy(buf.begin() + 54, buf.end(), data.data_Output);
//
//	return 0;
//
//}

bool img_JS(Mat &img_decode, PJ_Para Para, int n)//这里还要加上参数列表
{
	int result = 1;//1是ng 0是
	CONTOURS contours;
	vector<Vec4i> hierarchy;
	int MaxL = -1;
	long area = -1;

	Mat gray, output, green;
	Mat mv[3];

	stringstream ss;

	ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数

#pragma region 输入参数
	bool isGray = Para.isGray;//是否灰度显示

	//圆心定位
	float B1 = Para.B1, G1 = Para.G1, R1 = Para.R1, Gain1 = Para.Gain1;//颜色通道配比，支持负数 默认值均为1
	int threshold0 = Para.threshold0;
	float radius1 = Para.radius1, radius2 = Para.radius2;

	//胶水判别
	float B2 = Para.B2, G2 = Para.G2, R2 = Para.R2, Gain2 = Para.Gain2;//颜色通道配比，支持负数 默认值均为1

	int threshold1 = Para.threshold1;//默认值-1
	if (n % 5 == 0) { threshold1 -= GetPrivateProfileInt("Main", "R0", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R11", 0, "./1.ini"); }
	if (n % 5 == 1) { threshold1 -= GetPrivateProfileInt("Main", "R1", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R12", 0, "./1.ini"); }
	if (n % 5 == 2) { threshold1 -= GetPrivateProfileInt("Main", "R2", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R13", 0, "./1.ini"); }
	if (n % 5 == 3) { threshold1 -= GetPrivateProfileInt("Main", "R3", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R14", 0, "./1.ini"); }
	if (n % 5 == 4) { threshold1 -= GetPrivateProfileInt("Main", "R4", 0, "./1.ini"); threshold0 -= GetPrivateProfileInt("Main", "R15", 0, "./1.ini"); }

	//欠胶上限 胶水厚度下限
	int thLen0 = Para.thLen0, thLen1 = Para.thLen1;
	//6 7 胶水检测 5 6欠胶检测 9 8内环溢胶检测 3 4 外环溢胶检测 奇数为外径 偶数为内径
	float radius3 = Para.radius3, radius4 = Para.radius4;
	float radius5 = Para.radius5, radius6 = Para.radius6;
	float radius7 = Para.radius7, radius8 = Para.radius8;
	float radius9 = Para.radius9;

	int num = Para.num;//胶水段数
	int len = Para.len;//胶水长度 默认值0

#pragma endregion
	ss << "镜筒编号：" << Para.JTNum << endl;
#pragma region 图像第一次预处理
	//B1 /= 30;
	//G1 /= 30;
	//R1 /= 30;
	//Gain1 /= 10;
	//上面是为了进行归一
	cv::split(img_decode, mv);
	gray = B1 * mv[0] + G1 * mv[1] + R1 * mv[2];
	gray *= Gain1;



	//cvtColor(img_decode, gray, CV_BGR2GRAY);
	Mat grayTemp = gray.clone();
	blur(grayTemp, gray, Size(3, 3));
	threshold(gray, gray, threshold0, 255, THRESH_BINARY);//二值化阈值
#pragma endregion

#pragma region 判定是否二值化显示
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
#pragma endregion

#pragma region 筛选特定大小的轮廓
	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point2i> tmpCont;
		convexHull(contours[i], tmpCont);
		double tmpArea = contourArea(tmpCont, 0);
		if (tmpArea > CV_PI*radius1*radius1 || tmpArea < CV_PI*radius2*radius2)continue;
		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}
#pragma endregion

	for (int i = 0; i < 1; i++)
	{
#pragma region 检测轮廓是否有效
		if (MaxL < 0)
		{
			//ss << "无有效目标" << endl;
			break;
		}

		if (30 > contours[MaxL].size())
		{
			ss << "轮廓无效" << endl;
			break;
		}
#pragma endregion
		if (MaxL >= 0)
		{
#pragma region 检测圆度
			drawContours(output, contours, MaxL, Scalar(255, 0, 0), 2);
			double Area = contourArea(contours[MaxL]);
			//检测圆形是否规则
			if (Area < radius2*radius2*CV_PI || Area > radius1*radius1*CV_PI)
			{
				ss << "轮廓圆度不足" << endl;
				break;
			}
			ss << "轮廓面积:" << Area << endl;
#pragma endregion

#pragma region 获取圆心及半径
			Point2f center(0, 0);
			float radius;
			vector<Point2i> tmpCont;
			convexHull(contours[MaxL], tmpCont);
			RotatedRect ell = fitEllipse(tmpCont);
			ellipse(output, ell, Scalar(255, 0, 0), 2);
			center = ell.center;
			//Para.deltaX = (int)center.x;
			//Para.deltaY = (int)center.y;

			radius = ell.size.width / 2;
			ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;

			ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

			circle(output, center, radius1, Scalar(0, 0, 255), 1);
			circle(output, center, radius2, Scalar(0, 255, 0), 1);
			//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 图像第二次预处理
			if (0 > threshold1)break;

			//B2 /= 30;
			//G2 /= 30;
			//R2 /= 30;
			//Gain2 /= 10;
			//上面是为了进行归一
			cv::split(img_decode, mv);

			green = G2 * mv[1].clone();
			grayTemp = green.clone();
			blur(grayTemp, green, Size(3, 3));
			threshold(green, green, threshold1, 255, THRESH_BINARY);//二值化阈值


			gray = B2 * mv[0] + G2 * mv[1] + R2 * mv[2];
			gray *= Gain2;
			//cvtColor(img_decode, gray, CV_BGR2GRAY);
			grayTemp = gray.clone();
			blur(grayTemp, gray, Size(3, 3));
			threshold(gray, gray, threshold1, 255, THRESH_BINARY);//二值化阈值


#pragma endregion

#pragma region 判定是否二值化显示
			if (isGray)
			{
				cvtColor(gray, output, CV_GRAY2BGR);
			}
			else
				output = img_decode.clone();
#pragma endregion

#pragma region 点胶判断
			radius = radius3;
			circle(output, center, radius, Scalar(0, 0, 255), 1);

			radius = radius4;
			circle(output, center, radius, Scalar(0, 0, 255), 1);

			radius = radius5;
			circle(output, center, radius, Scalar(0, 255, 0), 1);

			radius = radius6;
			circle(output, center, radius, Scalar(255, 255, 0), 1);

			radius = radius7;
			circle(output, center, radius, Scalar(0, 255, 0), 1);

			radius = radius8;
			circle(output, center, radius, Scalar(0, 0, 255), 1);

			radius = radius9;
			circle(output, center, radius, Scalar(0, 0, 255), 1);

			vector<bool> OutContour[4];
			float size = 360;
			//外环溢胶检测
			Ana_DefectDetection_2(gray, OutContour[0], center, radius3, radius4, 0, size, 1);

			//欠胶检测
			Ana_DefectDetection_2(green, OutContour[1], center, radius5, radius6, 0, size, thLen0);

			//胶环检测
			Ana_DefectDetection_2(green, OutContour[2], center, radius7, radius6, 0, size, thLen1);

			//内环溢胶检测
			Ana_DefectDetection_2(gray, OutContour[3], center, radius9, radius8, 0, size, 1);

			vector<bool> test;

			//欠胶检测
			for (int i = 0; i < size; i++)
			{
				if (OutContour[2][i] && OutContour[1][i])
				{
					test.push_back(0);

					float s = sin(i*CV_PI / 180);
					float c = cos(i*CV_PI / 180);

					Point2f start = center + radius6 * Point2f(s, -c);
					Point2f end = center + radius7 * Point2f(s, -c);

					line(output, start, end, Scalar(255, 255, 0), 1);
				}
				else
					test.push_back(1);
			}

			float iii = 0;
			for (size_t i = 0; i < test.size(); i++)
			{
				if (OutContour[2][i])iii++;
			}
			iii /= test.size();
			//ss << iii << endl;
			iii *= size / len;
			//ss << iii << endl;
			if (iii < 1)
			{
				ss << "胶水量异常" << endl;
				ss << len / size << endl;
				break;
			}

			int flag = GetPrivateProfileInt("Main", "JSM", 0, "./1.ini");
			if (!flag)
			{
				if (num != Ana_LenFilter(test, len))
				{
					ss << "有效胶水段数：" << Ana_LenFilter(test, len) << endl;
					break;
				}
				ss << "有效胶水数:" << num << endl;
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
					ss << "胶水量足够" << endl;
				}
				else
				{
					ss << "胶水量欠缺" << endl;
					ss << iii << "//" << (num - 1)*len / size << endl;
					break;
				}
			}

			test.clear();
			for (int i = 0; i < size; i++)
			{
				if (OutContour[1][i] && OutContour[0][i])
				{
					test.push_back(0);

					float s = sin(i*CV_PI / 180);
					float c = cos(i*CV_PI / 180);

					Point2f start = center + radius4 * Point2f(s, -c);
					Point2f end = center + radius3 * Point2f(s, -c);

					line(output, start, end, Scalar(255, 255, 0), 1);
				}
				else
					test.push_back(1);
			}
			if (Ana_LenFilter(test, 1))
			{
				ss << "外环有溢胶" << endl;
				break;
			}
			else
				ss << "外环点胶正常" << endl;

			test.clear();
			for (int i = 0; i < size; i++)
			{
				if (OutContour[1][i] && OutContour[3][i])
				{
					test.push_back(0);

					float s = sin(i*CV_PI / 180);
					float c = cos(i*CV_PI / 180);

					Point2f start = center + radius9 * Point2f(s, -c);
					Point2f end = center + radius8 * Point2f(s, -c);

					line(output, start, end, Scalar(255, 255, 0), 1);
				}
				else
					test.push_back(1);
			}
			if (Ana_LenFilter(test, 1))
			{
				ss << "内环有溢胶" << endl;
				break;
			}
			else
				ss << "内环点胶正常" << endl;
#pragma endregion

			result = 0;//仅输出OK/NG
		}

	}
	//Scalar color = Scalar(0, 255, 0);
	//if (result == 1)color = Scalar(0, 0, 255);

	rectangle(output, Rect(0, 0, output.cols, output.rows), Scalar(0, 0, 255), 8);
	//putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 0, 255), 40, "黑体", 0, 0);
	if (isGray == 0)
	{
		stringstream ss2;
		ss2 << "镜筒编号：" << Para.JTNum << endl;
		putTextZH(output, ss2.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 80, "黑体", 0, 0);
		if (result == 0)
		{
			stringstream ss3;
			ss3 << "OK" << endl;
			putTextZH(output, ss3.str().c_str(), Point(20, 100), Scalar(0, 255, 0), 200, "黑体", 0, 0);
		}
		else
		{
			stringstream ss3;
			ss3 << "NG" << endl;
			putTextZH(output, ss3.str().c_str(), Point(20, 100), Scalar(0, 0, 255), 200, "黑体", 0, 0);

		}
	}
	else
	{
		putTextZH(output, ss.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 60, "黑体", 0, 0);
	}

	img_decode = output.clone();
	output.release();


	return result;
}

bool MV_PJ01(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	return MV_EntryPoint(3, nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float);
	//Mat img_decode = imread("E:\\MM\\1.bmp");
	Mat img_decode(nHeight, nWidth, CV_8UC3, data.data_Input);
	//MV_Release(data);

	TickMeter tm;
	tm.start();

	/////////////////////////////////////////////
#pragma region 输入参数
	PJ_Para Para;
	Para.isGray = input_Parameter_Int[0];
	Para.B1 = input_Parameter_Int[1];
	Para.G1 = input_Parameter_Int[2];
	Para.R1 = input_Parameter_Int[3];
	Para.Gain1 = input_Parameter_Int[4];
	Para.threshold0 = input_Parameter_Int[5];
	Para.radius1 = input_Parameter_Int[6];
	Para.radius2 = input_Parameter_Int[7];
	Para.B2 = input_Parameter_Int[8];
	Para.G2 = input_Parameter_Int[9];
	Para.R2 = input_Parameter_Int[10];
	Para.Gain2 = input_Parameter_Int[11];
	Para.threshold1 = input_Parameter_Int[12];
	Para.thLen0 = input_Parameter_Int[13];
	Para.thLen1 = input_Parameter_Int[14];
	Para.radius3 = input_Parameter_Int[15];
	Para.radius4 = input_Parameter_Int[16];
	Para.radius5 = input_Parameter_Int[17];
	Para.radius6 = input_Parameter_Int[18];
	Para.radius7 = input_Parameter_Int[19];
	Para.radius8 = input_Parameter_Int[20];
	Para.radius9 = input_Parameter_Int[21];
	Para.num = input_Parameter_Int[22];
	Para.len = input_Parameter_Int[23];
	float JT_Num = input_Parameter_Int[24];
	int LeftUp_x = img_decode.cols *input_Parameter_Int[25];//四点坐标
	int	LeftUp_y = img_decode.rows *input_Parameter_Int[26];
	int RightDown_x = img_decode.cols *input_Parameter_Int[27];
	int RightDown_y = img_decode.rows*input_Parameter_Int[28];
	float isCap = input_Parameter_Int[29];
#pragma endregion
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\PJ\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), img_decode);

	}
#pragma region 切割并识别图片
	int flag = 0;
	Mat output = img_decode;
	int Roi_w = (RightDown_x - LeftUp_x) / 5;
	int Roi_h = (RightDown_y - LeftUp_y) / 5;
	bool Result[25] = { false };
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			flag++;
			int Roi_x = LeftUp_x + j * Roi_w;
			int Roi_y = LeftUp_y + i * Roi_h;
			Mat Roi_output = img_decode(Rect(Roi_x, Roi_y, Roi_w, Roi_h));
			Para.JTNum = i * 5 + j + 1;
			Result[i * 5 + j] = img_JS(Roi_output, Para, j);

			Mat imgRoi = img_decode(Rect(Roi_x, Roi_y, Roi_w, Roi_h));
			Roi_output.copyTo(imgRoi, Roi_output);
			if (flag == JT_Num)
			{
				output.release();
				output = Roi_output;
			}
		}
	}
#pragma endregion

	if (JT_Num == 0)
	{
		resize(img_decode, output, Size(0, 0), 0.5, 0.5);
	}
	/////////////////////////////////////////////
	tm.stop();
	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\PJOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), output);

	}



	data.Height = output.rows;
	data.Width = output.cols;
	//data.size = output.total()*output.elemSize();
	//data.data_Output = (uchar*)calloc(data.size, sizeof(uchar));
	//std::memcpy(data.data_Output, output.data, data.size*sizeof(uchar));

	//输出
	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = static_cast<int>(buf.size() - 54);
	data.data_Output = new unsigned char[data.size];
	std::copy(buf.begin() + 54
		, buf.end(), data.data_Output);

	for (int i = 0; i < 25; i++)
	{
		output_Parameter_Int[i] = Result[i];
	}



	return 0;
}

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

bool MV_TP(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	Mat gray(nHeight, nWidth, CV_8UC1, data.data_Input);

	//Mat gray = imread("C:\\Image_20200907112002782.bmp", 0);

	Mat img_decode, output;
	cvtColor(gray, img_decode, CV_GRAY2BGR);
	//Redate Para;
	TickMeter tm;
	tm.start();


	//cvtColor(img_decode,output, CV_GRAY2BGR);

	CONTOURS contours;
	vector<Vec4i> hierarchy;
	stringstream out;
	int num = 0;

	out << "输出";
#pragma region 参数载入
	int sx = input_Parameter_Int[0];
	int sy = input_Parameter_Int[1];
	int ex = input_Parameter_Int[2];
	int ey = input_Parameter_Int[3];
	int angle = input_Parameter_Int[4];;//画面旋转角度
	int threshold = input_Parameter_Int[5];;//灰度阈值
	int MaxRadius = input_Parameter_Int[6];;//半径上限
	int MinRadius = input_Parameter_Int[7];;//半径下限
	int isGray = input_Parameter_Int[8];;//灰度显示
	int isCap = input_Parameter_Int[9];;//是否截图
#pragma endregion

	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\TP\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), img_decode);

	}

#pragma region 轮廓获取
	//cvtColor(img_decode, gray, CV_BGR2GRAY);
	//gray = img_decode.clone();
	Mat gray_tmp;
	blur(gray, gray_tmp, Size(3, 3));
	cv::threshold(gray_tmp, gray, threshold, 255, THRESH_BINARY);//二值化阈值

	Mat ROI_g = gray(Rect(sx, sy, ex - sx, ey - sy));
	Mat ROI_c = img_decode(Rect(sx, sy, ex - sx, ey - sy));

	findContours(ROI_g, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
#pragma endregion

#pragma region 获取轮廓
	int MaxL = -1;
	long area = -1;
	for (size_t i = 0; i < contours.size(); i++)
	{
#if 1
		vector<Point2i> tmpCont;
		convexHull(contours[i], tmpCont);

		double tmpArea = contourArea(tmpCont, 0);
		if (tmpArea > CV_PI * MaxRadius*MaxRadius || tmpArea < CV_PI * MinRadius*MinRadius)continue;
#else
		int tmpArea = arcLength(contours[i], 0);
#endif

		if (tmpArea > area)
		{
			area = (long)tmpArea;
			MaxL = i;
		}
	}
#pragma endregion

#pragma region 九宫格识别
#if 1
	if (MaxL >= 0)
	{
		drawContours(ROI_c, contours, MaxL, Scalar(255, 0, 0), 2);
		RotatedRect rect = minAreaRect(contours[MaxL]);

		Point2f rangle[4];
		rect.points(rangle);

		int lu = -1;
		int tmp = 9999999;
		for (int i = 0; i < 4; i++)
		{
			if (rangle[i].x + rangle[i].y < tmp)
			{
				tmp = rangle[i].x + rangle[i].y;
				lu = i;
			}
		}

		int x_delta = rangle[(lu + 1) % 4].x - rangle[lu].x;
		int y_delta = rangle[(lu + 3) % 4].y - rangle[lu].y;

		int x_delta_y = rangle[(lu + 3) % 4].x - rangle[lu].x;
		int y_delta_x = rangle[(lu + 1) % 4].y - rangle[lu].y;


		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{

				int x = rangle[lu].x + i * (x_delta + x_delta_y) / 3 + 0.1*x_delta / 3;
				int y = rangle[lu].y + j * (y_delta + y_delta_x) / 3 + 0.1*y_delta / 3;

				stringstream ss;
				ss << j + 3 * i << endl;

				//putTextZH(ROI_c, ss, Point(x, y), Scalar(255, 0, 0), 15);
				putTextZH(ROI_c, ss.str().c_str(), Point(x, y), Scalar(0, 255, 0), 80, "黑体", 0, 0);
				rectangle(ROI_c, Rect(x, y, x_delta / 3 * 0.8, y_delta / 3 * 0.8), Scalar(0, 255, 0), 4);

				Mat tmp_m, tmp_sd;
				Mat mask = Mat(ROI_g.rows, ROI_g.cols, CV_8UC1, Scalar(0));
				rectangle(mask, Rect(x, y, x_delta / 3 * 0.8, y_delta / 3 * 0.8), Scalar(255), -1);

				meanStdDev(ROI_g, tmp_m, tmp_sd, mask);

				if (tmp_m.at<double>(0, 0) < 128)
				{
					out << 1;
					num += 1;
				}
				else
				{
					out << 0;
					num += 0;
				}
				num *= 2;
			}
		}
		num /= 2;

		//circle(ROI_c, rangle[lu], 15, Scalar(255, 0, 0), -1);//B
		//lu = (lu + 1) % 4;
		//circle(ROI_c, rangle[lu], 15, Scalar(0, 255, 0), -1);//G
		//lu = (lu + 1) % 4;
		//circle(ROI_c, rangle[lu], 15, Scalar(0, 0, 255), -1);//R
		//lu = (lu + 1) % 4;
		//circle(ROI_c, rangle[lu], 15, Scalar(25, 25, 0), -1);
	}
#endif // 1
#pragma endregion

#pragma region 输出图像选择
	if (isGray)
	{
		cvtColor(gray, output, CV_GRAY2BGR);
	}
	else
		output = img_decode.clone();
#pragma endregion

	out << endl << "托盘号：" << num;
	putTextZH(output, out.str().c_str(), Point(0, 0), Scalar(0, 255, 0), 80, "黑体", 0, 0);
	rectangle(output, Rect(sx, sy, ex - sx, ey - sy), Scalar(0, 255, 0), 4);

	if (isCap == 1)
	{
		stringstream picfile;
		SYSTEMTIME st;
		GetLocalTime(&st);
		picfile << ".\\picture\\TPOutput\\" << st.wYear << st.wMonth << st.wDay << st.wHour << st.wMinute << st.wSecond << st.wMilliseconds << ".bmp";
		imwrite(picfile.str(), output);

	}



	tm.stop();

#pragma region 图像输出
	data.Height = output.rows;
	data.Width = output.cols;
	//输出
	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = static_cast<int>(buf.size() - 54);
	data.data_Output = new unsigned char[data.size];
	std::copy(buf.begin() + 54
		, buf.end(), data.data_Output);

	//Output.output_Parameter_Float[1] = distance;
	//*output_Parameter_Int = tm.getTimeMilli();
	//output_Parameter_Int++;
	*output_Parameter_Int = 0;
	output_Parameter_Int++;

	*output_Parameter_Float = num;
	output_Parameter_Float++;

	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//镜筒结果
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
#pragma endregion
}
/*******************************************************************************************************************************************************

以下为2020-11重新整理的算法

*******************************************************************************************************************************************************/
bool MV_JT(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
#pragma region 参数定义
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//LPSTR path = (LPSTR)malloc(sizeof(char)*1024);
	//GetPrivateProfileString("Main", "Path","",path,1024,"./2.ini");
	//Mat img = imread(path, 0);

	bool isGray = (bool)GetPrivateProfileInt("Main", "isGray", 0, "./2.ini");	//灰度显示       0表示显示原图 1表示显示灰度图

	int r1		= GetPrivateProfileInt("Main", "r0", 0, "./2.ini");		//定位-内环半径
	int r0		= GetPrivateProfileInt("Main", "r1", 0, "./2.ini");		//定位-外环半径
	int th0		= GetPrivateProfileInt("Main", "th0", 0, "./2.ini");	//定位-灰度阈值

	int isCanny	= GetPrivateProfileInt("Main", "isCanny", 0, "./2.ini");//角度-边缘显示 //0表示不启用 3表示启用
	int para0	= GetPrivateProfileInt("Main", "para0", 0, "./2.ini");	//角度-边缘参数0
	int para1	= GetPrivateProfileInt("Main", "para1", 0, "./2.ini");	//角度-边缘参数1

	int r2		= GetPrivateProfileInt("Main", "r2", 0, "./2.ini");		//角度-外环半径
	int r3		= GetPrivateProfileInt("Main", "r3", 0, "./2.ini");		//角度-内环半径
	float GPR0	= GetPrivateProfileInt("Main", "GPR0", 0, "./2.ini");	//径向有效点比例
	//GPR0 /= 100;
	int th1		= GetPrivateProfileInt("Main", "th1", 0, "./2.ini");	//角度-灰度阈值
	int angL	= GetPrivateProfileInt("Main", "angL", 0, "./2.ini");	//角度-缺口长度

	int CN		= GetPrivateProfileInt("Main", "CN", 0, "./2.ini");	//匹配-圆环数量
	int *r4		= (int*)malloc(sizeof(int)*CN);		//匹配-半径下限
	int *r5		= (int*)malloc(sizeof(int)*CN);		//匹配-半径上限
	float *GPR1	= (float*)malloc(sizeof(float)*CN);	//匹配-径向有效点比例
	float *GPR2	= (float*)malloc(sizeof(float)*CN);	//匹配-切向有效点比例
	int *th2	= (int*)malloc(sizeof(int)*CN);		//匹配-灰度阈值

	//for (size_t i = 0; i < CN; i++)
	//{
	//	stringstream ss;
	//	ss << "CN" << i;
	//	r4[i]	= GetPrivateProfileInt(ss.str().c_str(), "r4", 0, "./2.ini");
	//	r5[i]	= GetPrivateProfileInt(ss.str().c_str(), "r5", 0, "./2.ini");
	//	GPR1[i] = GetPrivateProfileInt(ss.str().c_str(), "GPR1", 0, "./2.ini");
	//	GPR2[i] = GetPrivateProfileInt(ss.str().c_str(), "GPR2", 0, "./2.ini");
	//	th2[i]	= GetPrivateProfileInt(ss.str().c_str(), "th2", 0, "./2.ini");
	//	GPR1[i] /= 100;//浮点型转换
	//	GPR2[i] /= 100;//浮点型转换
	//}

	int BzCenterX = 0;	//补偿基准点X
	int BzCenterY = 0;	//补偿基准点Y

	float rule_X = 1;//比例尺
	float rule_Y = 1;//比例尺

	float HzAngle = 0;
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = -1;
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
	OK_ss << "测试1" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "测试1" << endl;

	gray = img.clone();

	tm.start();
#pragma endregion

#pragma region 输出参数
	int deltaX = 0;
	int deltaY = 0;
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
	float radius;

	convexHull(contours[MaxL], tmpCont);
	ell = fitEllipse(tmpCont);
	ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
	center = ell.center;
	deltaX = center.x - BzCenterX;
	deltaY = center.y - BzCenterY;
	radius = ell.size.width / 2;

	OK_ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
	OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

	ERR_ss << " " << endl;
	ERR_ss << " " << endl;
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
			circle(output, end, 1, Scalar(255, 0, 0), 2);
			circle(output, sta, 1, Scalar(255, 0, 0), 2);
		}
		else
		{
			circle(output, end, 1, Scalar(0, 0, 255), 2);
			circle(output, sta, 1, Scalar(0, 0, 255), 2);
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

	OK_ss << "角度：" << angle << endl;
	ERR_ss << " " << endl;

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

		for (size_t j = 0; j < size; j++)
		{
			float s = sin(j * 360 / size * CV_PI / 180);
			float c = cos(j * 360 / size * CV_PI / 180);
			Point2f end = center + r5[i] * Point2f(s, -c);
			Point2f sta = center + r4[i] * Point2f(s, -c);
			if (GoodPoint[j])//BGR
			{
				circle(output, end, 1, Scalar(0, 255, 0), 2);
				circle(output, sta, 1, Scalar(0, 255, 0), 2);
			}
			else
			{
				circle(output, end, 1, Scalar(0, 0, 255), 2);
				circle(output, sta, 1, Scalar(0, 0, 255), 2);
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
			ERR_ss << " " << endl;
		}
		else
		{
			ERR_ss << "圆环" << i << "匹配度:" << sum << endl;
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

			std::string test = ERR_ss.str();
			putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
			test = OK_ss.str();
			putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);

			
#pragma endregion

#pragma region 图像转换
			data.Height = output.rows;
			data.Width = output.cols;

			std::vector<uchar>buf;
			cv::imencode(".bmp", output, buf);
			data.size = buf.size() - 54;
			data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
			std::copy(buf.begin() + 54, buf.end(), data.data_Output);
#pragma endregion		

#pragma region 参数输出
			*output_Parameter_Int = tm.getTimeMilli();
			output_Parameter_Int++;
			//OutputX = 5;
			//OutputY = 5;
			//角度
			*output_Parameter_Float = 180;
			output_Parameter_Float++;
			//正反
			*output_Parameter_Float = 0;
			output_Parameter_Float++;
			//中心差值
			*output_Parameter_Float = 20;
			output_Parameter_Float++;
			*output_Parameter_Float = 20;
			output_Parameter_Float++;
			//镜片结果
			*output_Parameter_Float = 0;
			output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
			free(r4);
			free(r5);
			free(GPR1);
			free(GPR2);
			free(th2);
#pragma endregion
			return 0;
}
//暂时不需重写
bool MV_JP(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
#pragma region 参数定义
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//LPSTR path = (LPSTR)malloc(sizeof(char)*1024);
	//GetPrivateProfileString("Main", "Path","",path,1024,"./2.ini");
	//Mat img = imread(path, 0);

	bool isGray = (bool)GetPrivateProfileInt("Main", "isGray", 0, "./2.ini");	//灰度显示       0表示显示原图 1表示显示灰度图

	int r1 = GetPrivateProfileInt("Main", "r0", 0, "./2.ini");		//定位-内环半径
	int r0 = GetPrivateProfileInt("Main", "r1", 0, "./2.ini");		//定位-外环半径
	int th0 = GetPrivateProfileInt("Main", "th0", 0, "./2.ini");	//定位-灰度阈值

	int isCanny = GetPrivateProfileInt("Main", "isCanny", 0, "./2.ini");//角度-边缘显示 //0表示不启用 3表示启用
	int para0 = GetPrivateProfileInt("Main", "para0", 0, "./2.ini");	//角度-边缘参数0
	int para1 = GetPrivateProfileInt("Main", "para1", 0, "./2.ini");	//角度-边缘参数1

	int r2 = GetPrivateProfileInt("Main", "r2", 0, "./2.ini");		//角度-外环半径
	int r3 = GetPrivateProfileInt("Main", "r3", 0, "./2.ini");		//角度-内环半径
	float GPR0 = GetPrivateProfileInt("Main", "GPR0", 0, "./2.ini");	//径向有效点比例
	//GPR0 /= 100;
	int th1 = GetPrivateProfileInt("Main", "th1", 0, "./2.ini");	//角度-灰度阈值
	int angL = GetPrivateProfileInt("Main", "angL", 0, "./2.ini");	//角度-缺口长度

	int CN = GetPrivateProfileInt("Main", "CN", 0, "./2.ini");	//匹配-圆环数量
	int *r4 = (int*)malloc(sizeof(int)*CN);		//匹配-半径下限
	int *r5 = (int*)malloc(sizeof(int)*CN);		//匹配-半径上限
	float *GPR1 = (float*)malloc(sizeof(float)*CN);	//匹配-径向有效点比例
	float *GPR2 = (float*)malloc(sizeof(float)*CN);	//匹配-切向有效点比例
	int *th2 = (int*)malloc(sizeof(int)*CN);		//匹配-灰度阈值

	//for (size_t i = 0; i < CN; i++)
	//{
	//	stringstream ss;
	//	ss << "CN" << i;
	//	r4[i]	= GetPrivateProfileInt(ss.str().c_str(), "r4", 0, "./2.ini");
	//	r5[i]	= GetPrivateProfileInt(ss.str().c_str(), "r5", 0, "./2.ini");
	//	GPR1[i] = GetPrivateProfileInt(ss.str().c_str(), "GPR1", 0, "./2.ini");
	//	GPR2[i] = GetPrivateProfileInt(ss.str().c_str(), "GPR2", 0, "./2.ini");
	//	th2[i]	= GetPrivateProfileInt(ss.str().c_str(), "th2", 0, "./2.ini");
	//	GPR1[i] /= 100;//浮点型转换
	//	GPR2[i] /= 100;//浮点型转换
	//}

	int BzCenterX = 0;	//补偿基准点X
	int BzCenterY = 0;	//补偿基准点Y

	float rule_X = 1;//比例尺
	float rule_Y = 1;//比例尺

	float HzAngle = 0;
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = -1;
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
	OK_ss << "测试1" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "测试1" << endl;

	gray = img.clone();

	tm.start();
#pragma endregion

#pragma region 输出参数
	int deltaX = 0;
	int deltaY = 0;
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
	float radius;

	convexHull(contours[MaxL], tmpCont);
	ell = fitEllipse(tmpCont);
	ellipse(output, ell, Scalar(255, 0, 0), 2);	//有效轮廓以蓝色
	center = ell.center;
	deltaX = center.x - BzCenterX;
	deltaY = center.y - BzCenterY;
	radius = ell.size.width / 2;

	OK_ss << "圆度" << Ana_ComputeRoundness(contours[MaxL]) << endl;
	OK_ss << "圆心坐标:(" << center.x << "," << center.y << ")" << endl;

	ERR_ss << " " << endl;
	ERR_ss << " " << endl;
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
			circle(output, end, 1, Scalar(255, 0, 0), 2);
			circle(output, sta, 1, Scalar(255, 0, 0), 2);
		}
		else
		{
			circle(output, end, 1, Scalar(0, 0, 255), 2);
			circle(output, sta, 1, Scalar(0, 0, 255), 2);
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

	OK_ss << "角度：" << angle << endl;
	ERR_ss << " " << endl;

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

		for (size_t j = 0; j < size; j++)
		{
			float s = sin(j * 360 / size * CV_PI / 180);
			float c = cos(j * 360 / size * CV_PI / 180);
			Point2f end = center + r5[i] * Point2f(s, -c);
			Point2f sta = center + r4[i] * Point2f(s, -c);
			if (GoodPoint[j])//BGR
			{
				circle(output, end, 1, Scalar(0, 255, 0), 2);
				circle(output, sta, 1, Scalar(0, 255, 0), 2);
			}
			else
			{
				circle(output, end, 1, Scalar(0, 0, 255), 2);
				circle(output, sta, 1, Scalar(0, 0, 255), 2);
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
			ERR_ss << " " << endl;
		}
		else
		{
			ERR_ss << "圆环" << i << "匹配度:" << sum << endl;
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

	std::string test = ERR_ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	test = OK_ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);


#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);
#pragma endregion		

#pragma region 参数输出
	*output_Parameter_Int = tm.getTimeMilli();
	output_Parameter_Int++;
	//OutputX = 5;
	//OutputY = 5;
	//角度
	*output_Parameter_Float = 180;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 20;
	output_Parameter_Float++;
	*output_Parameter_Float = 20;
	output_Parameter_Float++;
	//镜片结果
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
#pragma endregion

#pragma region 释放内存
	free(r4);
	free(r5);
	free(GPR1);
	free(GPR2);
	free(th2);
#pragma endregion
	return 0;
}

bool MV_DGP(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
#pragma region 参数定义
	Mat img(nHeight, nWidth, CV_8UC1, data.data_Input);//默认非点胶后相机提供的原始图像为黑白图像

	//LPSTR path = (LPSTR)malloc(sizeof(char) * 1024);
	//GetPrivateProfileString("Main", "Path", "", path, 1024, "./2.ini");
	//Mat img = imread(path, 0);

	bool isGray = (bool)GetPrivateProfileInt("Main", "isGray", 0, "./2.ini");	//灰度显示 0表示显示原图 1表示显示灰度图

	int r0 = GetPrivateProfileInt("Main", "r1", 0, "./2.ini");		//定位-内环半径
	int r1 = GetPrivateProfileInt("Main", "r0", 0, "./2.ini");		//定位-外环半径
	int th0 = GetPrivateProfileInt("Main", "th0", 0, "./2.ini");	//定位-灰度阈值

	int NoUse0		= 0;//占位但是不用，界面上也不需要显示
	int QKLength	= GetPrivateProfileInt("Main", "QKLength", 0, "./2.ini");	//缺口-长度下限
	int resultNum	= GetPrivateProfileInt("Main", "resultNum", 0, "./2.ini");	//缺口-缺口数量
	int NoUse1		= 0;//占位但是不用，界面上也不需要显示

	int r2 = GetPrivateProfileInt("Main", "r2", 0, "./2.ini");			//缺口-外环半径
	int r3 = GetPrivateProfileInt("Main", "r3", 0, "./2.ini");			//缺口-内环半径
	float GPR0 = GetPrivateProfileInt("Main", "GPR0", 0, "./2.ini");	//缺口-径向有效点比例
	//GPR0 /= 100;
	int th1 = GetPrivateProfileInt("Main", "th1", 0, "./2.ini");		//缺口-灰度阈值
#pragma endregion

#pragma region 本地参数
	int MaxL = -1;
	long area = -1;
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
	OK_ss << "挡光片检测" << endl;

	ERR_ss.setf(std::ios::fixed); //用定点格式显示浮点数,不会用科学计数法表示
	ERR_ss.precision(3);  //由于用了定点格式，设置变为了保留4位小数
	ERR_ss << "挡光片检测" << endl;

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

	ERR_ss << " " << endl;
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
	Ana_DefectDetection_2(img, GoodPoint, center, r3, r2, th1, size, (r3 - r2) * GPR0);

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

	std::string test = ERR_ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	test = OK_ss.str();
	putTextZH(output, test.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);


#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);
#pragma endregion		

#pragma region 参数输出
	*output_Parameter_Int = tm.getTimeMilli();
	output_Parameter_Int++;
	//OutputX = 5;
	//OutputY = 5;
	//角度
	*output_Parameter_Float = 180;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 20;
	output_Parameter_Float++;
	*output_Parameter_Float = 20;
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
#pragma endregion

	return 0;
}

bool MV_JS(int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
#pragma region 参数定义
	Mat img_decode(nHeight, nWidth, CV_8UC3, data.data_Input);//默认判胶相机使用的是彩色相机

	//LPSTR path = (LPSTR)malloc(sizeof(char)*1024);
	//GetPrivateProfileString("Main", "Path","",path,1024,"./3.ini");
	//Mat img_decode = imread(path, 1);

	bool isGray = GetPrivateProfileInt("Main", "isGray", 0, "./3.ini");//灰度显示

	int r1 = GetPrivateProfileInt("Main", "r0", 0, "./3.ini");//定位-轮廓下限
	int r0 = GetPrivateProfileInt("Main", "r1", 0, "./3.ini");//定位-轮廓上限
	int th0 = GetPrivateProfileInt("Main", "th0", 0, "./3.ini");//定位-灰度阈值

	int th1 = GetPrivateProfileInt("Main", "th1", 0, "./3.ini");//判胶-灰度阈值
	float R11 = GetPrivateProfileInt("Main", "R11", 0, "./3.ini");//判胶-R通道增益
	float B11 = GetPrivateProfileInt("Main", "B11", 0, "./3.ini");//判胶-B通道增益
	float G11 = GetPrivateProfileInt("Main", "G11", 0, "./3.ini");//判胶-G通道增益
	float Gain = GetPrivateProfileInt("Main", "Gain", 0, "./3.ini");//判胶-总增益

	int r2 = GetPrivateProfileInt("Main", "r2", 0, "./3.ini");//判胶-胶水检测内环
	int r3 = GetPrivateProfileInt("Main", "r3", 0, "./3.ini");//判胶-胶水检测外环

	int r4 = GetPrivateProfileInt("Main", "r4", 0, "./3.ini");//判胶-欠胶检测外径

	int r5 = GetPrivateProfileInt("Main", "r5", 0, "./3.ini");//判胶-向外溢胶内环
	int r6 = GetPrivateProfileInt("Main", "r6", 0, "./3.ini");//判胶-向外溢胶外环

	int r7 = GetPrivateProfileInt("Main", "r7", 0, "./3.ini");//判胶-向内溢胶内环
	int r8 = GetPrivateProfileInt("Main", "r8", 0, "./3.ini");//判胶-向内溢胶外环

	int th2 = GetPrivateProfileInt("Main", "th2", 0, "./3.ini");//判胶-欠胶环最小胶量
	int th3 = GetPrivateProfileInt("Main", "th3", 0, "./3.ini");//判胶-胶水环最小胶水量

	int th4 = GetPrivateProfileInt("Main", "th4", 0, "./3.ini");//判胶-溢胶限度

	int num = GetPrivateProfileInt("Main", "num", 0, "./3.ini");//判胶-胶水段数
	int len = GetPrivateProfileInt("Main", "len", 0, "./3.ini");//判胶-胶水长度 默认值0
#pragma endregion

#pragma region 本地参数
	float iii = 0;
	int flag = 0;
	int MaxL = -1;
	Mat gray, output, green;
	Mat RGB[3];
	long area = -1;
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
		if (tmpLen > 2 * CV_PI*r1 || tmpLen < 2 * CV_PI*r0) continue;
		if (tmpArea > CV_PI*r1*r1 || tmpArea < CV_PI*r0*r0) continue;
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
	ERR_ss << " " << endl;

	circle(output, center, r0, Scalar(0, 255, 0), 1);
	circle(output, center, r1, Scalar(0, 255, 0), 1);
	//circle(output, center, Radius7, Scalar(0, 255, 0), 1);
#pragma endregion

#pragma region 第二次合成
	//调试使用需求，th1<0表示不做胶水量判别
	if (0 > th1)goto OutPoint;

	cv::split(img_decode, RGB);

	green = G11 * RGB[1].clone();
	//green = G22 * RGB[1].clone();
	grayTemp = green.clone();
	blur(grayTemp, green, Size(3, 3));
	threshold(green, green, th1, 255, THRESH_BINARY);//二值化阈值


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

	//胶环检测
	Ana_DefectDetection_2(green, OutContour[0], center, r3, r2, 0, size, th3);

	//欠胶检测
	Ana_DefectDetection_2(green, OutContour[1], center, r4, r2, 0, size, th2);
	
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
		if (OutContour[1][i])iii++;
	}
	iii /= test.size();
	iii *= size / len;
	if (iii < 1)
	{
		ERR_ss << "胶水量异常" << endl;
		ERR_ss << len / size << endl;
		goto OutPoint;
	}

	flag = GetPrivateProfileInt("Main", "JSM", 0, "./1.ini");
	if (!flag)
	{
		if (num != Ana_LenFilter(test, len))
		{
			ERR_ss << "有效胶水段数：" << Ana_LenFilter(test, len) << endl;
			goto OutPoint;
		}
		OK_ss << "有效胶水数:" << num << endl;
		ERR_ss << " " << endl;
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
			ERR_ss << " " << endl;
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
		ERR_ss << " " << endl;
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
		ERR_ss << " " << endl;
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

	std::string text = ERR_ss.str();
	putTextZH(output, text.c_str(), text_Localtion01, Scalar(0, 0, 255), text_Size, "黑体", 0);
	text = OK_ss.str();
	putTextZH(output, text.c_str(), text_Localtion01, Scalar(0, 255, 0), text_Size, "黑体", 0);
#pragma endregion

#pragma region 图像转换
	data.Height = output.rows;
	data.Width = output.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", output, buf);
	data.size = buf.size() - 54;
	data.data_Output = (unsigned char *)calloc(buf.size() - 54, sizeof(unsigned char));
	std::copy(buf.begin() + 54, buf.end(), data.data_Output);
#pragma endregion		

#pragma region 参数输出
	*output_Parameter_Int = 0;//tm.getTimeMilli();
	output_Parameter_Int++;
	//OutputX = 5;
	//OutputY = 5;
	//角度
	*output_Parameter_Float = 180;
	output_Parameter_Float++;
	//正反
	*output_Parameter_Float = 0;
	output_Parameter_Float++;
	//中心差值
	*output_Parameter_Float = 20;
	output_Parameter_Float++;
	*output_Parameter_Float = 20;
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

bool MV_EntryPoint(int type, int nHeight, int nWidth, BmpBuf &data, float* input_Parameter_Int, int* output_Parameter_Int, float* output_Parameter_Float)
{
	switch (type)
	{
	case 0:MV_JT(nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float); break;
	case 1:MV_JT(nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float); break;
	case 2:MV_DGP(nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float); break;
	case 3:MV_JS(nHeight, nWidth, data, input_Parameter_Int, output_Parameter_Int, output_Parameter_Float); break;
	default:
		break;
	}
	return 0;
}