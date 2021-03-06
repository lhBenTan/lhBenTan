#pragma once

/*
开发者：谭力豪
建立日期：2019-12-10
开发环境：Windows7+VS17
opencv版本：3.4.6-win32-release-vc15&3.4.6-win32-debug-vc15
备注：加载本动态链接库前请在项目中加入opencv(具体版本见上文)
	  请对应调试器版本添加对应lib及dll
	  请按头文件调用函数

修改说明：
_序号_|_______日期_______|_____________________说明______________________|__修改人__|
__01__|____2019-09-05____|___________________新规发布____________________|__谭力豪__|
__02__|____2019-09-17____|_______FindSomeContours=>ContoursFilter________|__谭力豪__|
__03__|____2019-10-31____|_____________添加ComputeRoundness______________|__谭力豪__|
	  |____2019-10-31____|______________添加ContoursMatch________________|__谭力豪__|
	  |____2019-10-31____|________________添加FindMaxLen_________________|__谭力豪__|
	  |____2019-10-31____|______________修改ContoursFilter_______________|__谭力豪__|
	  |____2019-10-31____|____________添加DefectDetection重载____________|__谭力豪__|
__04__|____2019-12-10____|_________________添加调试版本__________________|__谭力豪__|
	  |____2019-12-10____|_________________迁移项目文件__________________|__谭力豪__|
	  |____2019-12-10____|__________________添加DEF文件__________________|__谭力豪__|
	  |____2019-12-10____|_________________添加函数前缀__________________|__谭力豪__|
	  |____2019-12-10____|_____________修改部分函数形参格式______________|__谭力豪__|

*/

#pragma region 预定义及引用
#include<opencv.hpp>

using namespace cv;
using namespace std;

#define CONTOURS vector<vector<Point2i>>
#pragma endregion

#if 1
#pragma region 有损处理
/*************************************
函数名：Pro_Pretreatment
功能：对图片进行预处理
输入参数：
	str-原始图片
	dst-输出图片
	Para-配置参数-【0】模糊核大小（小于0是不进行模糊操作）
				  【1】阈值参数(小于0时不进行二值化操作)
				  【2】二值化类型
				  【3】形态学变化的核形状（小于0时不进行形态学变化）
				  【4】形态学变化的核大小
				  【5】形态学变化操作
				  【6】轮廓查找操作
	hierarchy-轮廓树
	contours-输出轮廓表
返回值
*************************************/
_declspec(dllimport)void Pro_Pretreatment(Mat str, Mat &dst, int* Para, vector<Vec4i> &hierarchy, CONTOURS &contours);
#pragma endregion

#pragma region 图像分析
/*************************************
函数名：Ana_FindBigestContours
功能：获取最大轮廓序号
输入参数：
	contours-输入轮廓表
返回值：周长最大轮廓对应序号
*************************************/
_declspec(dllimport)int Ana_FindBigestContours(CONTOURS contours);
/*************************************
函数名：Ana_ContoursFilter
功能：获取特定特征的轮廓
输入参数：
	Para-配置参数-【0】是否为最内层轮廓 1-yes 0—no
				  【1】是否为最外层轮廓 1-yes 0—no
				  【2】轮廓周长下限（不设下限时，此值请设为0）
				  【3】轮廓周长上限（不设上限时，此值请设为INT_MAX）
				  【4】轮廓到圆心的距离上限（不设上限时，此值请设为INT_MAX）
				  【5】轮廓到圆心的距离下限（不设下限时，此值请设为0）
				  【6】轮廓近似面积下限（不设下限时，此值请设为0）
	Center-圆心坐标
	contours-输入轮廓表
	hierarchy-轮廓树
	*GoodContours-输出轮廓表
返回值：满足要求的轮廓个数
*************************************/
_declspec(dllimport)int Ana_ContoursFilter(int* Para, Point2f Center, CONTOURS contours, vector<Vec4i> hierarchy, CONTOURS &GoodContours);
/*************************************
函数名：Ana_DefectDetection_1
功能：缺口检测---计算轮廓到中心的距离
输入参数1：
	Contour-输入轮廓
	Para-【0】（radius - Para1）圆心距上限（不设上限时，此值请设为INT_MIN）
		 【1】（radius - Para2）圆心距下限（不设下限时，此值请设为INT_MAX）
	OutContour-输出轮廓表（用于绘图）
	Center-圆心坐标
返回值：
*************************************/
_declspec(dllimport)void Ana_DefectDetection_1(vector<Point> Contour, float *Para, vector<bool> &OutContour, Point2f &Center);
/*************************************
函数名：Ana_DefectDetection_2
功能：缺口检测---检测圆环灰度
输入参数1：
	str	- 被测图像
	outContour	- 对应方向是否满足要求
	Center	-	画面中心
	Maxradius	-	外环半径
	Minradius	-	内环半径
	th	- 阈值
	size	-	总间隔数
	tlh	-	对应方向满足要求的像素的个数
返回值：
*************************************/
_declspec(dllimport)void Ana_DefectDetection_2(Mat str, vector<bool> &OutContour, Point2f Center, float Maxradius, float Minradius, int th, float size, int thl);//待修改
/*************************************
函数名：Ana_DefectDetection_3
功能：缺口检测---检测圆周灰度
输入参数1：
	str	- 被测图像
	outContour	-	对应方向有效长度
	Center	-	画面中心
	radius	-	半径
	th	- 阈值
	size	-	总间隔数
返回值：
*************************************/
_declspec(dllimport)void Ana_DefectDetection_3(Mat str, vector<bool> &OutContour, Point2f Center, float radius, int th, float size);
/*************************************
函数名：Ana_DefectDetection_4
功能：缺口检测---外接圆+外接矩形计算缺口方向
输入参数1：
	contour	- 被测轮廓
返回值：缺口角度
*************************************/
_declspec(dllimport)float Ana_DefectDetection_4(vector<Point> contour);
/*************************************
函数名：ComputeAngle
功能：计算旋转角度
输入参数：
	sPoint-起点坐标
	ePoint-终点坐标
返回值：旋转角度
*************************************/
_declspec(dllimport)double Ana_ComputeAngle(Point2f sPoint, Point2f ePoint);
/*************************************
函数名：ComputeRoundness
功能：计算轮廓圆度
输入参数：
	contour-被测轮廓
返回值：轮廓圆度
*************************************/
_declspec(dllimport)double Ana_ComputeRoundness(vector<Point> contour);
/*************************************
函数名：ContoursMatch
功能：轮廓比对
输入参数：
	contour-被测轮廓
	Para-【0】相似度阈值
		 【1】不相似块数量上限
返回值：异常轮廓序号（返回-1表示左右块均正常）
*************************************/
_declspec(dllimport)int Ana_ContoursMatch(CONTOURS contours, float *Para);
/*************************************
函数名：FindMaxLen
功能：查询最大缺陷
输入参数：
	inContour-被测列表
返回值：[0]起始(注:有一定概率第返回值为负数,请利用输入长度校正)
		[1]终止
*************************************/
_declspec(dllimport)Vec2i Ana_FindMaxLen(vector<bool> inContour);
/*************************************
函数名：ContoursMatch
功能：轮廓比对
输入参数：
	img		-模板图像
	imgEx	-目标图像
返回值：相似度
*************************************/
_declspec(dllimport)float Ana_HistCompare(Mat img, Mat imgEx);
/*************************************
函数名：Ana_LenFilter
功能：获取特定高电平信号长度数量
输入参数：
	in	- 信号队列
	th	- 长度
返回值：高电平个数
*************************************/
_declspec(dllimport)int Ana_LenFilter(vector<bool> in, int th);

/*************************************
函数名：Ana_FindLen
功能：查找特定长度的中心
输入参数：
	inContour	-被测列表
	max			-长度上限
	min			-长度下线
返回值：满足条件的脉冲的几何中心
*************************************/
_declspec(dllexport)float Ana_FindLen(vector<bool> inContour, int max, int min);
#pragma endregion

#endif

#pragma region 示例

#pragma region 胶水检测
/* 示例1 胶水检测

bool img_JStest(LPSTR filename, BmpBuf &data)
{
	stringstream ss;
	Mat Color = imread(filename, 1);
	Mat Gray,img;

	Color -= Scalar(0,20,0);
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

	//Gray -= 15;
	//Gray *= 8;

	img = Gray.clone();

	//tm.stop();
	//ss << "时间 = " << tm.getTimeMilli() << endl;//rows
	//tm.start();

	//计算画面

	blur(Gray, Gray, Size(5, 5));
	threshold(Gray, Gray, 150, 255, CV_THRESH_BINARY);
	cv::findContours(Gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	//tm.stop();
	//ss << "时间 = " << tm.getTimeMilli() << endl;//rows
	//tm.start();

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

		ss << "找不到胶圈"<<endl;//rows

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
	if(Ana_LenFilter(test, 1))
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
	circle(Color, center, radius, Scalar(0, 0, 255), 1);

	radius = 245;
	circle(Color, center, radius, Scalar(0, 0, 255), 1);

	radius = 240;
	circle(Color, center, radius, Scalar(0, 255, 0), 1);

	radius = 210;
	circle(Color, center, radius, Scalar(255, 255, 0), 1);

	radius = 200;
	circle(Color, center, radius, Scalar(0, 255, 0), 1);

	radius = 195;
	circle(Color, center, radius, Scalar(0, 0, 255), 1);

	radius = 190;
	circle(Color, center, radius, Scalar(0, 0, 255), 1);
#pragma endregion


	data.h = Color.rows;
	data.w = Color.cols;

	tm.stop();
	ss << "时间 = " << tm.getTimeMilli() << endl;//rows
	putTextZH(Color, ss.str().c_str(), Point(0, 0 + 20), Scalar(0, 255, 0), 50, "黑体", 0, 0);
	//
	flip(Color, Color, 1);  //rotate 180
	flip(Color, Color, -1);  //rotate 180

	std::vector<uchar>buf;
	cv::imencode(".bmp", Color, buf);
	data.size = buf.size();
	data.data_Output = (unsigned char *)calloc(buf.size(), sizeof(unsigned char));
	std::copy(buf.begin(), buf.end(), data.data_Output);

	return 0;

}

*/
#pragma endregion

#pragma region 镜片检测
/*
bool img_JPtest(LPSTR filename, BmpBuf &data)
{
	FILE *file;
	file = fopen("2.csv", "a");
	fprintf(file, "%s,", filename);

	Mat Color, Gray;
	Mat img = imread(filename, 0);

	float radius;
	Point2f center(0, 0);
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	TickMeter tm;
	tm.start();

	cvtColor(img, Color, CV_GRAY2BGR);
	//计算画面
	blur(img, Gray, Size(5, 5));
	threshold(Gray, Gray, 130, 255, CV_THRESH_BINARY);
	cv::findContours(Gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	//imshow("1", Gray);

	int MaxL;
	LONG area = -1;

	for (int i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][3] < 0)continue;
		int tmpArea = contourArea(contours[i]);
		if (tmpArea < 2000)continue;

		if (tmpArea > area)
		{
			area = tmpArea;
			MaxL = i;
		}
	}
	if (area > 0)
	{
		Vec2i len;
		float ang[3];
		stringstream ss;

		minEnclosingCircle(contours[hierarchy[MaxL][3]], center, radius);
#pragma region 倾斜计算
		if (area)
		{
			float radiusEx;
			Point2f centerEx;
			minEnclosingCircle(contours[MaxL], centerEx, radiusEx);
			centerEx -= center;
			ss << "圆心偏移:" << sqrt(centerEx.x*centerEx.x + centerEx.y*centerEx.y) << endl;
			fprintf(file, "圆心偏移:%.3f,", sqrt(centerEx.x*centerEx.x + centerEx.y*centerEx.y));
		}
		MaxL = hierarchy[MaxL][3];
#pragma endregion

#pragma region 灰度值匹配
		Mat theimg = imread("./1.bmp", 0);
		Mat roi = img(Rect(center.x - radius, center.y - radius, radius * 2, radius * 2)).clone();
		float Sim = Ana_HistCompare(roi, theimg);

		ss << "相似度:" << Sim << endl;//rows
		fprintf(file, "相似度:%.3f,", Sim);

#if 0
		Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);

		//因为图像中的某个灰度值的总个数可能会超出所定义的图像的尺寸，所以要对个数进行归一化处理

		//先用 minMaxLoc函数来得到计算直方图中的最大数据,这个函数的使用方法大家一看函数原型便知
		double g_dHistMaxValue;
		minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);


		//遍历直方图数据,对数据进行归一化和绘图处理
		for (int i = 0; i < 256; i++)
		{
			int value = cvRound(dstHist.at<float>(i) * 256 * 0.9 / g_dHistMaxValue);

			//line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - value), Scalar(255, 0, 0));

			line(drawImage, Point(i, 0), Point(i, value), Scalar(255, 0, 0));
		}

		// 输出矩阵定义
		Mat resultImage(drawImage.size(), drawImage.type());
		// X与Y方向矩阵
		Mat xMapImage(drawImage.size(), CV_32FC1);
		Mat yMapImage(drawImage.size(), CV_32FC1);
		int rows = drawImage.rows;
		int cols = drawImage.cols;

		for (int j = 0; j < rows; j++)
		{
			for (int i = 0; i < cols; i++)
			{
				// y方向上翻转
				xMapImage.at<float>(j, i) = i;
				yMapImage.at<float>(j, i) = rows - j;
			}
		}
		// 重映射操作
		remap(drawImage, resultImage, xMapImage, yMapImage,
			CV_INTER_LINEAR, cv::BORDER_CONSTANT,
			cv::Scalar(0, 0, 0));

		imshow("【未翻转前的直方图】", drawImage);
		imshow("【翻转后的直方图】", resultImage);
#endif
#pragma endregion

		if (Sim > 0.5)
		{
#pragma region 外接圆遍历
			vector<bool> GoodPoint;
			float size = 360;
			Ana_DefectDetection_3(img, GoodPoint, center, radius, 70, size);
			len = Ana_FindMaxLen(GoodPoint);

			//for (int i = 0; i < size; i++)
			//{
			//	float xo = center.x + 55 * sin(i * 360 / size * CV_PI / 180);
			//	float yo = center.y - 55 * cos(i * 360 / size * CV_PI / 180);
			//	if (GoodPoint[i])
			//	{
			//		line(Color, center, Point(xo, yo), Scalar(255, 255, 0), 1);
			//	}
			//	else
			//	{
			//		line(Color, center, Point(xo, yo), Scalar(0, 0, 255), 1);
			//	}
			//}
			ang[0] = (len[1] + len[0]) * 360 / size / 2;
			if (ang[0] > 360)ang[0] -= 360;
			line(Color, center, Point(center.x + radius * sin(ang[0] * CV_PI / 180.0), center.y - radius * cos(ang[0] * CV_PI / 180.0)), Scalar(255, 0, 0), 4);

			ss << "外接圆遍历角度:" << ang[0] << endl;

			GoodPoint.clear();
#pragma endregion

#pragma region 外接圆及外接矩形
			RotatedRect rect = minAreaRect(contours[MaxL]);

			ang[1] = Ana_ComputeAngle(rect.center, center);

			while (abs(ang[1] - rect.angle) > 10 && rect.angle < 360)rect.angle += 90;

			ang[1] = rect.angle;
			//Point2f p[4];

			//rect.points(p);
			//for (int i = 0; i < 4; i++)
			//{
			//	line(Color, p[i % 4], p[(i + 1) % 4], Scalar(255, 255, 0), 1);
			//}
			ss << "外接矩形角度:" << ang[1] << endl;

			line(Color, center, Point(center.x + radius * sin(ang[1] * CV_PI / 180.0), center.y - radius * cos(ang[1] * CV_PI / 180.0)), Scalar(0, 0, 255), 2);
#pragma endregion

#pragma region 计算轮廓上的点到中心的距离
			float Para[2] = { 2,20 };
			Ana_DefectDetection_1(contours[MaxL], Para, GoodPoint, center);

			for (int i = 0; i < GoodPoint.size(); i++)
			{
				if (GoodPoint[i])
					circle(Color, contours[MaxL][i], 0.5, Scalar(0, 255, 0), 1);
				else
					circle(Color, contours[MaxL][i], 0.5, Scalar(0, 0, 255), 1);
			}

			len = Ana_FindMaxLen(GoodPoint);
			Point2i end = (contours[MaxL][len[1]] + contours[MaxL][(len[0] + GoodPoint.size()) % GoodPoint.size()]) / 2;
			ang[2] = Ana_ComputeAngle(center, end);
			line(Color, center, end, Scalar(0, 255, 0), 1);

			ss << "轮廓距离角度:" << ang[2] << endl;
#pragma endregion

#pragma region 综合判定
			float CTF = 1;
			int angL;
			for (int i = 0; i < 3; i++)
			{
				float tmpCTF = (abs(ang[i] - ang[(i + 1) % 3])) / (ang[i] + ang[(i + 1) % 3]);
				if (tmpCTF < CTF)
				{
					CTF = tmpCTF;
					angL = i;
				}
			}

			ss << "综合判定角度:" << (ang[angL] + ang[(angL + 1) % 3])/2 << endl;
			fprintf(file, "综合判定角度:%.3f,", (ang[angL] + ang[(angL + 1) % 3]) / 2);

#pragma endregion

#pragma region 查找最大轮廓级数
			int maxTic = -1;
			for (int i = 0; i < contours.size(); i++)
			{
				int num = i;
				int tic = 0;
				if (contourArea(contours[i]) < 2000)continue;
				while (hierarchy[num][3] >= 0)
				{
					tic++;
					num = hierarchy[num][3];
				}

				if (tic > maxTic)
					maxTic = tic;
			}

			ss << "最大轮廓级:" << maxTic + 1 << endl;
			fprintf(file, "最大轮廓级:%d,", maxTic + 1);

#pragma endregion

#pragma region 计算平均灰度值
			Scalar a = mean(roi);

			ss << "平均灰度:" << a[0] << endl;
			fprintf(file, "平均灰度:%d,", (int)a[0]);

#pragma endregion

			ss.precision(3);
			ss << "圆度:" << Ana_ComputeRoundness(contours[MaxL]) << endl;
			fprintf(file, "圆度:%.3f,", Ana_ComputeRoundness(contours[MaxL]));
		}
		else
		{
			ss << "镜片错误" << endl;
			fprintf(file, "镜片错误,");

		}

		tm.stop();
		ss << "时间 = " << tm.getTimeMilli() << endl;
		fprintf(file, "时间 = %.3f,", tm.getTimeMilli());
		putTextZH(Color, ss.str().c_str(), Point(0, img.rows / 40.0), Scalar(0, 255, 0), 60 * img.rows*img.cols / 1280 / 1024, "黑体", 0, 0);
	}

	flip(Color, Color, 1);  //rotate 180
	flip(Color, Color, -1);  //rotate 180

	data.h = Color.rows;
	data.w = Color.cols;

	std::vector<uchar>buf;
	cv::imencode(".bmp", Color, buf);
	data.size = buf.size();
	data.data_Output = (unsigned char *)calloc(buf.size(), sizeof(unsigned char));
	std::copy(buf.begin(), buf.end(), data.data_Output);

	fprintf(file, "\n");
	fclose(file);

	return 0;
}
*/
#pragma endregion

#pragma endregion

