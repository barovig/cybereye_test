#include <iostream>
#include "unistd.h"
#include "ce.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

using namespace std;
using namespace ce;
using namespace cv;

// key callbacks
void switchUpdate(Engine*);


bool stop = false;
bool update = true;

int main()
{
	ce::Model *model = new ce::Model();
    BkgSegmentationEngine *e = new BkgSegmentationEngine(model);
    Collector *c = new Collector();
    Tracker *t = new Tracker();
    CoreManager cm(e, c, t, 0);
	
	// run frame capture thread
	cm.startCapture();
	
	// run segmentation engine
	cm.startSegmentation();
	
    cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("mask", cv::WINDOW_AUTOSIZE);
	
	char key = 0;
	
    while(!stop) {
		
        cv::Mat m = cm.getFrame();
		
		switch(key){
			case ' ':
				switchUpdate(e);
				break;
			case 27:
				stop = true;
				break;
		}
		
		cv::Mat mask;
		e->getMask().copyTo(mask);
		
		if(!m.empty())
            cv::imshow("frame", m);
		
		if(!mask.empty())
		{
//			cv::filter2D(mask, mask, 0, Mat::ones(5,5, CV_8UC1));
			
//			// EROSION/DILATION approach
//			cv::erode(mask, mask, cv::Mat());
//			cv::erode(mask, mask, cv::Mat());
//			cv::erode(mask, mask, cv::Mat());
//			cv::erode(mask, mask, cv::Mat());
			 
//			cv::dilate(mask, mask, cv::Mat());
			
//			// TRY DENOISING
			
//			cv::filter2D(mask, mask, 0, Mat::ones(5,5, CV_8UC1));
			
		}
		
		if(!mask.empty())
			cv::imshow("mask", mask);
		
		key = (char)cv::waitKey(10);
    }

	//cm.stopAllThreads();
		
    return 0;
}

void switchUpdate(Engine *e)
{
	update = !update;
	std::cout << "Changing update flag to " << update << endl;
	BkgSegmentationEngine* bse = dynamic_cast<BkgSegmentationEngine*>(e);
	bse->setUpdateFlag(update ? -1 : 0);		
}
