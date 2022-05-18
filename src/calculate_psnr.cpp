#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "opencv2/opencv.hpp"
#include <limits>

#define intensity_range 255 

double getPSNR(const cv::Mat& I1, const cv::Mat& I2)
{
    cv::Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2
    cv::Scalar s = sum(s1);        // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
	 double inf = std::numeric_limits<double>::infinity();
    if( sse <= 1e-10) // for small values return inf
		 
        return inf;
    else
    {
        double mse  = sse / (double)(I1.channels() * I1.total());
		 
        double psnr = 10.0 * log10((intensity_range * intensity_range) / mse);
        return psnr;
    }
}
int main(int argc, char* argv[]) {

	//原画像と処理画像を読み込む．
	if(argc <= 1)
		  std:: cout << "<source_image.format> <noisy_image.format>" <<std::endl;	
   else{

		cv::Mat src = cv::imread(argv[1]);
		cv::Mat dst = cv::imread(argv[2]);
		cvtColor(src,src, cv::COLOR_RGB2GRAY,1);
		cvtColor(dst,dst, cv::COLOR_RGB2GRAY,1);
		std::cout << "The PSNR between two images ("<< src.cols << "x" << src.rows << ") = " << getPSNR(src, dst)<< " dBs " << std::endl;
	}
	return 0;
}
