#include <iostream>
#include "unistd.h"
#include "ce.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include <functional>

using namespace std;
using namespace ce;
using namespace cv;
using namespace std::placeholders;

char key = -1;

Point prevPt(-1, -1);

static void onMouse( int event, int x, int y, int flags, void* userdata);
int main()
{		
	Mat frame, mask;
	
	// cybereye setup
	CyberEye eye(0);
	eye.initialise();
	eye.start();
	eye.getFrame(frame);
	
	namedWindow("frame");
	namedWindow("mask");
	setMouseCallback( "frame", onMouse, (void*)&mask);
	
	try{
		while((key = waitKey(10)) != 27)
		{
			
			eye.getFrame(frame);
			
			if(frame.size != mask.size){
				mask = Mat(frame.size(), CV_8UC1);
				mask = cv::Scalar::all(0);
			}
			
			if(key == ' ')
			{
				eye.segment(mask);
				mask = cv::Scalar::all(0);
			}
			
			if(!frame.empty())
				imshow("frame", frame);
			if(!mask.empty())
				imshow("mask", mask);
	
		}
	}
	catch(cv::Exception e){
		cout << e.what()<< " " << e.line << " " << e.file << "FUNCTION: " << e.func << endl;
	}

	eye.stop();
	int i = 1;
	for(ce::ImgObj img : eye.getImages())
	{
		string path("/tmp/"+std::to_string(i++)+".jpg");
		imwrite(path, img.getImgData());
	}
	cout << "BEfore exit" << endl;
	destroyAllWindows();
	return 0;
}

static void onMouse( int event, int x, int y, int flags, void* userdata)
{
	Mat mask = *static_cast<Mat*>(userdata);
    if( x < 0  || y < 0 )
        return;
    if( event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON) )
        prevPt = Point(-1,-1);
    else if( event == EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) )
    {
        Point pt(x, y);
        if( prevPt.x < 0 )
            prevPt = pt;
        line( mask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
    }
}

