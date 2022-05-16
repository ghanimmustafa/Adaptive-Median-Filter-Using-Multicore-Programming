/* Source:https://github.com/BBuf/Image-processing-algorithm/blob/master/Image%20Filtering/Adaptive%20Median%20Filter.cpp*/
#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"
using namespace std;
using namespace cv;

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
        kernelSize += 2;
        if(kernelSize <= maxSize)
            return adaptiveProcess(im, row, col, kernelSize, maxSize);
        else
            return med;
    }
}

Mat work(Mat src){
    Mat dst;
    int minSize = 3; //滤波器窗口的起始大小
    int maxSize = 15; //滤波器窗口的最大尺寸
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
    Mat src = cv::imread(argv[1]);
    Mat dst = work(src);
    imshow("origin", src);
    imshow("result", dst);
    imwrite("lena_filtered.png", dst);
    waitKey(0);
}
