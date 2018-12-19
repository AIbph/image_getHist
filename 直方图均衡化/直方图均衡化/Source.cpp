#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv){
	// 直方图均衡化

	// 读取图片
	Mat src = imread("D:/OpenCVProject/day1219/直方图均衡化/直方图均衡化/test.jpg");
	if (!src.data)
	{
		printf("could not load image....");
	}
	imshow("origin image", src);

	/*
	// 调用均衡化函数
	cvtColor(src, src, CV_BGR2GRAY);
	Mat hist_src;
	equalizeHist(src, hist_src);
	imshow("final image", hist_src);
	*/

	// 分通道显示
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	// 计算直方图
	int histSize = 256;
	float range[] = { 0, 256 };
	const float *histRanges = {range};
	Mat b_hist, g_hist, r_hist;
	/*
		void cv::calcHist   (   
			const Mat *     images,			// 输入的图像或数组，它们的深度必须为CV_8U, CV_16U或CV_32F中的一类，尺寸必须相同。
			int     nimages,				// 输入数组个数，也就是第一个参数中存放了几张图像，有几个原数组。 
			const int *     channels,		// 需要统计的通道dim
			InputArray      mask,			// 可选的操作掩码。如果此掩码不为空，那么它必须为8位并且尺寸要和输入图像images[i]一致。非零掩码用于标记出统计直方图的数组元素数据。 
			OutputArray     hist,			// 输出的目标直方图，一个二维数组 
			int     dims,					// 需要计算直方图的维度，必须是正数且并不大于CV_MAX_DIMS(在opencv中等于32) 
			const int *     histSize,		// 每个维度的直方图尺寸的数组 
			const float **      ranges,		// 每个维度中bin的取值范围 
			bool    uniform = true,			// 直方图是否均匀的标识符，有默认值true 
			bool    accumulate = false		// 累积标识符，有默认值false,若为true，直方图再分配阶段不会清零。此功能主要是允许从多个阵列中计算单个直方图或者用于再特定的时间更新直方图.
		)  
	*/
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRanges, true, false);

	// 计算直方图
	int hist_h = 500;
	int hist_w = 512;
	int bin_w = hist_w / histSize;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(255, 255, 255));
	/*
		void cv::normalize(InputArry src,InputOutputArray dst,double alpha=1,double beta=0,int norm_type=NORM_L2,int dtype=-1,InputArray mark=noArry())
		src               输入数组；
		dst               输出数组，数组的大小和原数组一致；
		alpha           1,用来规范值，2.规范范围，并且是下限；
		beta             只用来规范范围并且是上限；
		norm_type  		归一化选择的数学公式类型；
		dtype           当为负，输出在大小深度通道数都等于输入，当为正，输出只在深度与输如不同，不同的地方游dtype决定；
		mark            掩码。选择感兴趣区域，选定后只能对该区域进行操作。
接！
	*/
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());

	// 画出直方图
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point((i - 1) * bin_w, hist_w - cvRound(b_hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
		line(histImage, Point((i - 1) * bin_w, hist_w - cvRound(b_hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
		line(histImage, Point((i - 1) * bin_w, hist_w - cvRound(b_hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
	}
	imshow("final image", histImage);


	waitKey(0);
	return 0;
}