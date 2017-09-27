
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void PrintMs(const char *text = "")
{
    static long long last = 0;
    long long cur  = getTickCount();

    if (last == 0) {
        last  = cur;
		 return;
	}	

	long long ms;
	ms  = (double)(cur -last) /getTickFrequency() * 1000; //ms

	if (*text != 0)
	{
        printf("%s = %d ms.\n", text, ms);
	}

	last = getTickCount();
}


int main(int argc, char *argv[])
{
     VideoCapture video;
	 //video.open("../../bin/test.mp4");
	 video.open("../../bin/lol.mp4");

	 if (!video.isOpened()) {
	    cout<<"open video failed. "<< endl;
		getchar();
		return -1;		 
	 }

	 cout<<"open video success"<<endl;

	 namedWindow("video");
	 Mat frame;
     int fps = video.get(CAP_PROP_FPS);
	 int s = 30;
	 if (fps != 0)
         s  = 1000 / fps;

	 cout<<" fps is "<< fps <<endl;
     
	 int fcount  = video.get(CAP_PROP_FRAME_COUNT);
	 cout<<" total frame is "<< fcount << endl;
	 cout << " total sec is "<< fcount / fps << endl;

	 s = s/ 2;
	 for (;;) {
	     video.read(frame);

         int cur = video.get(CAP_PROP_POS_FRAMES);
         if (cur > 120) {
		     video.set(CAP_PROP_POS_FRAMES, 0);
			 continue;
		 }		 
		 if (frame.empty()) break;
		 imshow("video", frame);
		 waitKey(s);
	 }
}


