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
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::serialization;

/*
 * to install run something like
 * g++ ceye_server.cpp -I/home/semargl/root/include -L/home/semargl/root/lib -Wl,-rpath=/home/semargl/root/lib \
 -lcybereye -lopencv_world -lboost_serialization -lboost_system -o ceye_server
 * *
 */

int __main()
{		
	// construct Receiver and RecognitionEngine
	Ptr<ObjReceiver> receiver(new ObjReceiver());
	Ptr<RecognitionEngine> engine(new RecognitionEngine());
	receiver->setEngine(engine);	
	ce::CoreManager cm(receiver, engine);
	
	cm.startReceiving();
	cout << "Press enter to stop" << endl;
	string input;
	cin >> input;
	cm.stopAllThreads();
	
	return 0;
}


