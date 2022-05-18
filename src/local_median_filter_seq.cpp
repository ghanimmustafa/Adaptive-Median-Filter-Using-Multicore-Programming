
#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"
#include <chrono>
using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	 const int kernel_size = 15;
    Mat src = cv::imread(argv[1]);
    auto start = std::chrono::system_clock::now();
    Mat dst;
	 cv::medianBlur(src,dst,kernel_size);
  	 auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time = "<< elapsed_seconds.count()  << " seconds" << std::endl;  
    //imshow("origin", src);
	 cv::imwrite("seq_local_filtered.png", dst);
	 cv::resize(dst, dst, cv::Size(), 0.35, 0.35);
	 cv::imshow("result", dst);
    waitKey(0);
    destroyAllWindows(); 
}
