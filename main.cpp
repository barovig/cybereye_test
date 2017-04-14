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
	
	// write images to files
//	int i = 1;
//	for(ce::ImgObj img : eye.getImages())
//	{
//		string path("/tmp/"+std::to_string(i++)+".jpg");
//		imwrite(path, img.getImgData());
//	}
	
//	// prepare boost stuff to send data to client
//	io_service ios;
	
//	// make endpoint directly - don't use resolver and iterator
//	string addr = "192.168.1.15";
//	int port = 13491;
//	tcp::endpoint endpt(address::from_string(addr), port);
	
//	// create socket and connect
//	tcp::socket sock(ios);
//	sock.connect(endpt);
	
//	boost::system::error_code err;
	
//	const int header_length = 8;
	
//	// create archive and stream
//	// NOTE: because binary_oarchive is non-portable, use text_archive
//    std::ostringstream archive_stream;
//    boost::archive::text_oarchive archive(archive_stream);	
//	Mat img;
//	ImgObj imObj = eye.getImages()[0];
//	imObj.getImgData().copyTo(img);
//    archive << img;
//    string outbound_data = archive_stream.str();	// string data to send
	
//	// create stringstream for header data
//	ostringstream header_stream;
//	// send data size as hex num string, note fixed length
//	header_stream << setw(header_length) << std::hex << outbound_data.size();
	
//	// sanity check
//	if(!header_stream || header_stream.str().size() != header_length)
//	{
//		cerr << "Sent header length and declared length mismatch - dayum!" << endl;
//		return 0;
//	}
	
//	string outbound_header = header_stream.str();	// create string for header 
	
//	// write all data to socket using vector of buffers
	
//	vector<const_buffer> buffs;
//	buffs.push_back(buffer(outbound_header));
//	buffs.push_back(buffer(outbound_data));
//	sock.write_some(buffs, err);
	
	
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

