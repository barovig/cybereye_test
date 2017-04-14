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



int _main()
{
	Mat img;
	
	// create service and socket
	io_service ios;
	int port = 13491;
	tcp::acceptor acceptor(ios, tcp::endpoint(tcp::v4(), port));
	// create inbound header buffer 
	const int header_length = 8;
	char buff_header[header_length];
	
	// create inbound data vector
	vector<char> buff_data;
	for(;;){
		
		tcp::socket sock(ios);
		acceptor.accept(sock);
		
		boost::system::error_code err;
		
		// read data into header buffer and construct stream
		size_t data_sz = 0;		
		{ // scope closes stream
			sock.read_some(buffer(buff_header), err);
			istringstream is(string(buff_header, header_length));
			if( !(is >> std::hex >> data_sz) )
			{
				err = boost::asio::error::invalid_argument;
				cerr << err.message() << endl;
				return 0;
			}
		}
		
		buff_data.resize(data_sz);
		// read actual data
		{
			sock.read_some(buffer(buff_data), err);
			string archive_data(&buff_data[0], buff_data.size());
			istringstream archive_is(archive_data);
			boost::archive::text_iarchive arch(archive_is);
			
			arch >> img;
		}
		
	}
	
	imwrite("/tmp/img.jpg", img);
	return 0;
}
