/*
 Title: OpenCV: Apply kernel(Mask) to image.
 Note:
	- Develop with OpenCV in OS X, Xcode.
*/

#include <iostream>
#include <opencv2/opencv.hpp>


void ApplyKernel(const cv::Mat& myImage, cv::Mat& Result, char c);
uchar Sharpen(const cv::Mat& myImage, int index_i, int index_j, int nChannels);
uchar EdgeDetection(const cv::Mat& myImage, int index_i, int index_j, int nChannels);
uchar GaussianBlur(const cv::Mat& myImage, int index_i, int index_j, int nChannels);

int main(){
	// Load a sample image.
	cv::Mat img = cv::imread("./image.jpg");
	
	cv::Mat result;
	if (img.data){
		std::cout << "Image loaded" << std::endl;
	}
	else{
		std::cout << "Image not loaded" << std::endl;
		exit(1);
	}
	
	cv::namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Input Image", img);
	cv::waitKey();
	
	result = img.clone();
	ApplyKernel(img, result,'s');
	cv::namedWindow("Processed Image: Sharpened Image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Processed Image: Sharpened Image", result);
	cv::waitKey();
	
	result = img.clone();
	ApplyKernel(img, result,'e');
	cv::namedWindow("Processed Image: Edge Detection", CV_WINDOW_AUTOSIZE);
	cv::imshow("Processed Image: Edge Detection", result);
	cv::waitKey();
	
	result = img.clone();
	ApplyKernel(img, result,'b');
	cv::namedWindow("Processed Image: Gaussian Blur", CV_WINDOW_AUTOSIZE);
	cv::imshow("Processed Image: Gaussian Blur", result);
	cv::waitKey();
}

void ApplyKernel(const cv::Mat& myImage, cv::Mat& Result, char c){
	CV_Assert(myImage.depth() == CV_8U);
	const int nChannels = myImage.channels();
	Result.create(myImage.size(), myImage.type());
	for(int j = 1; j < myImage.rows - 1; ++j){
		uchar* output = Result.ptr<uchar>(j);
		for(int i = nChannels; i < nChannels * (myImage.cols - 1); ++i){
			if(c == 's')
				*output++ = Sharpen(myImage, i, j, nChannels);
			else if(c == 'e')
				*output++ = EdgeDetection(myImage, i, j, nChannels);
			else if(c == 'b')
				*output++ = GaussianBlur(myImage, i, j, nChannels);
			
		}
	}
	
	Result.row(0).setTo(cv::Scalar(0));
	Result.row(Result.rows - 1).setTo(cv::Scalar(0));
	Result.col(0).setTo(cv::Scalar(0));
	Result.col(Result.cols - 1).setTo(cv::Scalar(0));
}

uchar Sharpen(const cv::Mat& myImage, int index_i, int index_j, int nChannels){
	const uchar* previous = myImage.ptr<uchar>(index_j - 1);
	const uchar* current  = myImage.ptr<uchar>(index_j    );
	const uchar* next     = myImage.ptr<uchar>(index_j + 1);
	return cv::saturate_cast<uchar>(5 * current[index_i]
								- current[index_i - nChannels]
								- current[index_i + nChannels]
								- previous[index_i]
								- next[index_i]);
}

uchar EdgeDetection(const cv::Mat& myImage, int index_i, int index_j, int nChannels){
	const uchar* previous = myImage.ptr<uchar>(index_j - 1);
	const uchar* current  = myImage.ptr<uchar>(index_j    );
	const uchar* next     = myImage.ptr<uchar>(index_j + 1);
	return cv::saturate_cast<uchar>(8 * current[index_i]
								- current[index_i - nChannels]
								- current[index_i + nChannels]
								- previous[index_i]
								- previous[index_i - nChannels]
								- previous[index_i + nChannels]
								- next[index_i]
								- next[index_i - nChannels]
								- next[index_i + nChannels]);
}

uchar GaussianBlur(const cv::Mat& myImage, int index_i, int index_j, int nChannels){
	const uchar* previous = myImage.ptr<uchar>(index_j - 1);
	const uchar* current  = myImage.ptr<uchar>(index_j    );
	const uchar* next     = myImage.ptr<uchar>(index_j + 1);
	return cv::saturate_cast<uchar>(0.25 * current[index_i]
									+ 0.125 * current[index_i - nChannels]
									+ 0.125 * current[index_i + nChannels]
									+ 0.125 * previous[index_i]
									+ 0.0625* previous[index_i - nChannels]
									+ 0.0625* previous[index_i + nChannels]
									+ 0.125 * next[index_i]
									+ 0.0625* next[index_i - nChannels]
									+ 0.0625* next[index_i + nChannels]);
}

