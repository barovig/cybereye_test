#include <iostream>
#include "unistd.h"
#include "ce.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

using namespace std;
using namespace ce;
using namespace cv;

char key = -1;

int main()
{		
	Mat frame;
	Mat mask;
	// cybereye setup
	CyberEye eye(0);
	eye.initialise();
	eye.start();
	
	namedWindow("frame");
	namedWindow("mask");
	
	while((key = waitKey(10)) != 27)
	{
		
		eye.getFrame(frame);
		mask = eye.getMask();
		if(!frame.empty())
			imshow("frame", frame);
		if(!mask.empty())
			imshow("mask",mask);
	}
	eye.stop();
	int i = 1;
	for(ce::ImgObj img : eye.getImages())
	{
		string path("/tmp/imgs/"+std::to_string(i++)+".jpg");
		imwrite(path, img.getImgData());
	}
	cout << "BEfore exit" << endl;
	destroyAllWindows();
	return 0;
}
