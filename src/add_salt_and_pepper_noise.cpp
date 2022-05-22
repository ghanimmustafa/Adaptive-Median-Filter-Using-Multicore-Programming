// Compile: g++ add_salt_and_pepper_noise.cpp -o add_salt_and_pepper_noise `pkg-config --cflags --libs opencv4`

#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

void Add_salt_pepper_Noise(Mat &srcArr, float pa, float pb )

{    RNG rng; // rand number generate
    int amount1=srcArr.rows*srcArr.cols*pa;
    int amount2=srcArr.rows*srcArr.cols*pb;
    for(int counter=0; counter<amount1; ++counter)
    {
      // rng.uniform( 0,srcArr.rows), rng.uniform(0, srcArr.cols)
    	 srcArr.at<uchar>(rng.uniform( 0,srcArr.rows), rng.uniform(0, srcArr.cols)) =0;

    }
     for (int counter=0; counter<amount2; ++counter)
     {
     	srcArr.at<uchar>(rng.uniform(0,srcArr.rows), rng.uniform(0,srcArr.cols)) = 255;
     }
}
void Add_gaussian_Noise(Mat &srcArr,double mean,double sigma)
{
    Mat NoiseArr = srcArr.clone();
    RNG rng;
    rng.fill(NoiseArr, RNG::NORMAL, mean,sigma); 

     //randn(dstArr,mean,sigma); 
    add(srcArr, NoiseArr, srcArr); 
}

int main(int argc, char *argv[])
{
	Mat srcArr;
   float Pa;
	float Pb;
	string image_name,noise_degree;
	if (argc<=3)
	{ cout<< "<image_name.png> <Pa (%)> <Pb (%)> \n";
	  exit(0);

	}

	else if (argc>=4)
	{ 

		srcArr = imread(argv[1]);
		image_name = argv[1];
		 Pa = strtol(argv[2], NULL, 10)/100.0;
		 Pb = strtol(argv[3], NULL, 10)/100.0;


	}


	cvtColor(srcArr,srcArr, cv::COLOR_RGB2GRAY,1);

	Mat srcArr1 = srcArr.clone();
	Mat srcArr2 = srcArr.clone();
	Mat dstArr;
	if(Pa >= 0.5 || Pb >= 0.5)
		noise_degree = "high";
	else
		noise_degree ="low";

	Add_salt_pepper_Noise(srcArr1, Pa,Pb);
	//imshow("Add salt and pepper noise to image ", srcArr1);
	imwrite(noise_degree + "_noisy_"+image_name,srcArr1);
	return 0;
}
