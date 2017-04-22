//#define _GLIBCXX_USE_CXX11_ABI 0	//fix errors for GCC 5+
#include <iostream>
#include "unistd.h"
#include "ce.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include <functional>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace std;
using namespace ce;
using namespace cv;
using namespace std::placeholders;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::serialization;

char key = -1;
Point prevPt(-1, -1);

static void onMouse( int event, int x, int y, int flags, void* userdata);

int main()
{		
	Mat frame, mask;
	bool showMask = false;
	
	// cybereye setup
	CyberEye eye(0);
	
	// set conn params
	eye.setIP("192.168.1.15");
	eye.setPort(13491);
	
	eye.initialise();
	eye.start();
	eye.getFrame(frame);
	
	namedWindow("frame");
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
			
			if(key == 'm')
			{
				showMask = !showMask;
				showMask ? namedWindow("mask") : destroyWindow("mask");
			}
			
			if(key == 'c')
				eye.clearModel();
			
			if(!frame.empty())
				imshow("frame", frame);
			if(!mask.empty() && showMask)
				imshow("mask", mask);
	
		}
	}
	catch(cv::Exception e){
		cout << e.what()<< " " << e.line << " " << e.file << "FUNCTION: " << e.func << endl;
	}

	eye.stop();
	
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

