
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    Mat  image   = imread("1.png", 0);
	namedWindow("img");

	imshow("img", image);

	waitKey(0);
	return 0;
}


