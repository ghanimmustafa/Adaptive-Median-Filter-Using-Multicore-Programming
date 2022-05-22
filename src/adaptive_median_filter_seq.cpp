/* Source:https://github.com/BBuf/Image-processing-algorithm/blob/master/Image%20Filtering/Adaptive%20Median%20Filter.cpp*/
// Compile:
// g++ adaptive_median_filter_seq.cpp -o adaptive_median_filter_seq `pkg-config --cflags --libs opencv4` -DDEBUG
#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"
#include <chrono>
#include <string> 
#include <fstream>
using namespace std;
using namespace cv;
#ifdef DEBUG
	double cnt = 0.0;
#endif
uchar adaptiveProcess(const Mat &im, int row, int col, int kernelSize, int maxSize)
{
    vector <uchar> pixels;
    for(int a = -kernelSize / 2; a <= kernelSize / 2; a++){
        for(int b = -kernelSize / 2; b <= kernelSize / 2; b++){
            pixels.push_back(im.at<uchar>(row + a, col + b));
        }
    }
    sort(pixels.begin(), pixels.end());
    auto min = pixels[0];
    auto max = pixels[kernelSize * kernelSize - 1];
    auto med = pixels[kernelSize * kernelSize / 2];
    auto zxy = im.at<uchar>(row, col);
    if(med > min && med < max){
        if(zxy > min && zxy < max){
            return zxy;
        }else{
            return med;
        }
    }
    else{
		  #ifdef DEBUG
		  	cnt+= 1;
        #endif	
        kernelSize += 2;
        if(kernelSize <= maxSize)
            return adaptiveProcess(im, row, col, kernelSize, maxSize);
        else
            return med;
    }
}

Mat work(Mat src){
    Mat dst;
    int minSize = 3;
    int maxSize = 15;
    copyMakeBorder(src, dst, maxSize / 2, maxSize / 2, maxSize / 2, maxSize / 2, BORDER_REFLECT);
    int rows = dst.rows;
    int cols = dst.cols;
    for(int j = maxSize / 2; j < rows - maxSize / 2; j++){
        for(int i = maxSize / 2; i < cols * dst.channels() - maxSize / 2; i++){
            dst.at<uchar>(j, i) = adaptiveProcess(dst, j, i, minSize, maxSize);
        }
    }
    dst = dst(Range(maxSize/2, src.rows + maxSize/2), Range(maxSize/2, src.cols + maxSize/2));
    return dst;
}

int main(int argc, char* argv[]){
	int kernel_size;
	string image_name;
	Mat src;
	Mat dst;
	if (argc<=1)
	{ cout<< "<image_name.png> \n";
	  exit(0);
	}

	else if (argc>=2)
	{ 
		src = imread(argv[1]);
		image_name = argv[1];
	}
    auto start = std::chrono::system_clock::now();
    dst = work(src);
  	 auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << image_name << " after " << " adaptive median filter (sequential) takes " <<  elapsed_seconds.count()  << " seconds." << std::endl; 
	 
	 #ifdef DEBUG
		ofstream myfile2;
		myfile2.open ("adaptive_window_usage_ratio.txt",ios::app);
    	std::cout << "Adaptive window usage ratio = "<< (cnt/(src.rows * src.cols)) * 100  << " %" << std::endl;
		myfile2 << image_name << ": adaptive window usage ratio = " << (cnt/(src.rows * src.cols)) * 100  << "% \n";
		myfile2.close();
		cv::imwrite("seq_adaptive_filtered_"+image_name, dst);
	 #else
		ofstream myfile1;
	   myfile1.open ("adaptive_median_filter_seq_runtimes.txt",ios::app);
		myfile1 << image_name << " after " << " adaptive median filter (sequential) takes " <<  elapsed_seconds.count()  << " seconds\n";
		myfile1.close();
    #endif  
	
	
	 /*	
	 cv::resize(dst, dst, cv::Size(), 0.35, 0.35);
	 cv::imshow("result", dst);
    waitKey(0);
    destroyAllWindows(); */
	 return 0;
}
