#include "opencv2/opencv.hpp"
#include <pthread.h>
#include "iostream"
#include "algorithm"
#include "vector"

using namespace std;

const int MAX_THREADS = 64;

uchar adaptiveProcess(const cv::Mat &, int, int, int, int);

struct p_info
{
	cv::Mat *src;
	cv::Mat *dst;
	int start;
	int end;
	int max;
	int min;
};

static void* th_work(void *arguments)
{
	struct p_info *part_info = (struct p_info *) arguments;
	cv::Mat &dst = *(part_info->dst);
	int cols = dst.cols;
	int offset = part_info->max / 2;

	for (int j = part_info->start; j < part_info->end; j++)
	{
		for (int i = offset; i < cols * dst.channels() - offset; i++)
		{
			dst.at<uchar>(j, i) = adaptiveProcess(dst, j, i, part_info->min, part_info->max);
		}
	}
	return NULL;
}

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
	thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
	struct p_info partitions[thread_count];

	/* Read Images */
	cv::Mat src = cv::imread(argv[1]);
	cv::Mat dst;

	/* Prepare DST */
	int min_size = 3;
	int max_size = 15;
	int offset = max_size / 2;
	cv::copyMakeBorder(src, dst, offset, offset, offset, offset, cv::BORDER_REFLECT);

	int div_size;
	for (thread = 1; thread < thread_count; thread++)
	{
#ifdef DEBUG
		printf("Creating thread %ld.\n", thread);
#endif
		div_size = src.cols / thread_count;
		partitions[thread].src = &src;
		partitions[thread].dst = &dst;
		partitions[thread].start = div_size * thread + offset;
		partitions[thread].end = (thread != thread_count - 1) ? div_size * (thread + 1) + offset : src.cols + offset;
		partitions[thread].max = max_size;
		partitions[thread].min = min_size;

		/* Create threads */
		pthread_create(&thread_handles[thread], NULL,
					   th_work, (void *) &partitions[thread]);
	}

	for (thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	free(thread_handles);

	return 0;
}


uchar adaptiveProcess(const cv::Mat &im, int row, int col, int kernelSize, int maxSize)
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