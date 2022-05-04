#include <pthread.h>
#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"

using namespace std;
using namespace cv;

const int MAX_THREADS = 64;

uchar adaptiveProcess(const Mat&, int, int, int, int);
void work(Mat, Mat&);

/* Global variable:  accessible to all threads */
int thread_count;

int main(int argc, char **argv)
{

	/* Check command line usage */
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s image_file\n", argv[0]);
		return -1;
	}

	/*  Thread  Related */
	long thread;			   /* Use long in case of a 64-bit system */
	pthread_t *thread_handles; /* Array to hold threads */

	/* Get number of threads from command line */
	thread_count = strtol(argv[2], NULL, 10);
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++)
	{
#ifdef DEBUG
		printf("Creating thread %ld.\n", thread);
#endif

		/* Create threads */
		pthread_create(&thread_handles[thread], NULL,
					   th_work, (void *)thread);
	}

	for (thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	free(thread_handles);

	return 0;
}

uchar adaptiveProcess(const Mat &im, int row, int col, int kernelSize, int maxSize)
{
	vector<uchar> pixels;
	for (int a = -kernelSize / 2; a <= kernelSize / 2; a++)
	{
		for (int b = -kernelSize / 2; b <= kernelSize / 2; b++)
		{
			pixels.push_back(im.at<uchar>(row + a, col + b));
		}
	}
	sort(pixels.begin(), pixels.end());
	auto min = pixels[0];
	auto max = pixels[kernelSize * kernelSize - 1];
	auto med = pixels[kernelSize * kernelSize / 2];
	auto zxy = im.at<uchar>(row, col);
	if (med > min && med < max)
	{
		if (zxy > min && zxy < max)
		{
			return zxy;
		}
		else
		{
			return med;
		}
	}
	else
	{
		kernelSize += 2;
		if (kernelSize <= maxSize)
			return adaptiveProcess(im, row, col, kernelSize, maxSize);
		else
			return med;
	}
}

void work(Mat src, Mat& dst)
{
	int minSize = 3;  //滤波器窗口的起始大小
	int maxSize = 15; //滤波器窗口的最大尺寸
	copyMakeBorder(src, dst, maxSize / 2, maxSize / 2, maxSize / 2, maxSize / 2, BORDER_REFLECT);
	int rows = dst.rows;
	int cols = dst.cols;
	for (int j = maxSize / 2; j < rows - maxSize / 2; j++)
	{
		for (int i = maxSize / 2; i < cols * dst.channels() - maxSize / 2; i++)
		{
			dst.at<uchar>(j, i) = adaptiveProcess(dst, j, i, minSize, maxSize);
		}
	}
}