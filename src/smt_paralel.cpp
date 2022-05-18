#include "opencv2/opencv.hpp"
#include <pthread.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

const int MAX_THREADS = 64;
pthread_mutex_t mtx;

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

static void *th_work(void *arguments)
{
	std::queue<struct p_info> *part_queue = (std::queue<struct p_info>*) arguments;
	struct p_info part_info;
	while (true)
	{
		pthread_mutex_lock(&mtx);
		if (part_queue->empty())
			break;
		part_info = (struct p_info) part_queue->front();
		part_queue->pop();
		pthread_mutex_unlock(&mtx);
		cv::Mat &dst = *(part_info.dst);
		int cols = dst.cols;
		int offset = part_info.max / 2;
#ifdef DEBUG
		printf("Thread start:%d end:%d STARTED\n", part_info.start, part_info.end);
#endif
		for (int j = part_info.start; j < part_info.end; j++)
		{
			for (int i = offset; i < cols * dst.channels() - offset; i++)
			{
				dst.at<uchar>(j, i) = adaptiveProcess(dst, j, i, part_info.min, part_info.max);
			}
		}
#ifdef DEBUG
		printf("Thread start:%d end:%d ENDED\n", part_info.start, part_info.end);
#endif
	}
	pthread_mutex_unlock(&mtx);
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
	pthread_mutex_init(&mtx, NULL);

	/* Get number of threads from command line */
	thread_count = strtol(argv[2], NULL, 10);
	std::queue<struct p_info> partitions; // struct p_info partitions[thread_count];
	long div_count = thread_count;
	if (thread_count > 8)
		thread_count = 8;
	thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));


	/* Read Images */
	cv::Mat src = cv::imread(argv[1]);
	cv::Mat dst;
   auto start = std::chrono::system_clock::now();
	/* Prepare DST */
	int min_size = 3;
	int max_size = 15;
	int offset = max_size / 2;
	cv::copyMakeBorder(src, dst, offset, offset, offset, offset, cv::BORDER_REFLECT);

	int div_size;
	struct p_info part;
	for (long div = 0; div < div_count; div++)
	{
		div_size = src.cols / div_count;
		part.src = &src;
		part.dst = &dst;
		part.start = div_size * div + offset;
		part.end = (div != div_count - 1) ? div_size * (div + 1) + offset : src.cols + offset;
		part.max = max_size;
		part.min = min_size;
		
		partitions.push(part);
	}

	for (thread = 1; thread < thread_count; thread++)
	{
#ifdef DEBUG
		printf("Creating thread %ld.\n", thread);
#endif
			/* Create threads */
			pthread_create(&thread_handles[thread], NULL,
						   th_work, (void *)&partitions);
	}
	
	th_work((void *)&partitions);

	for (thread = 1; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	free(thread_handles);
	pthread_mutex_destroy(&mtx);

	dst = dst(cv::Range(offset, src.rows + offset), cv::Range(offset, src.cols + offset));
   auto end = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = end - start;
   std::cout << "Elapsed time = "<< elapsed_seconds.count()  << " seconds" << std::endl;  
	//cv::imshow("origin", src);
	cv::imwrite("smt_paralel_adaptive_filtered.png", dst);
	cv::resize(dst, dst, cv::Size(), 0.35, 0.35);
	cv::imshow("result", dst);
	cv::waitKey(0);
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
