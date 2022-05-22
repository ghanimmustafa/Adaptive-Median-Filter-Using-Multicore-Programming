//Compile:
// g++ local_median_filter.cpp -o local_median_filter `pkg-config --cflags --libs opencv4`
#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"
#include <chrono>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	int kernel_size;
	string image_name;
	Mat src;
	Mat dst;
	ofstream myfile;
	myfile.open ("local_median_filter_runtimes.txt",ios::app);
	if (argc<=2)
	{ cout<< "<image_name.png> <kernel_size> \n";
	  exit(0);
	}

	else if (argc>=3)
	{ 
		src = imread(argv[1]);
		image_name = argv[1];
		kernel_size = strtol(argv[2], NULL, 10);
	}
    auto start = std::chrono::system_clock::now();
	 medianBlur(src,dst,kernel_size);
  	 auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << image_name << " after " << kernel_size << "x" << kernel_size << " local median filter takes " <<  elapsed_seconds.count()  << " seconds." << std::endl; 
	 myfile << image_name << " after " << kernel_size << "x" << kernel_size << " local median filter takes " <<  elapsed_seconds.count()  << " seconds\n"; 
	 myfile.close();
	 imwrite("local_filtered_" + image_name, dst);
	/*
	 cv::resize(dst, dst, cv::Size(), 0.35, 0.35);
	 cv::imshow("result", dst);
	 waitKey(0);
	 destroyAllWindows(); */
	 return 0;
}
