#include <iostream>
#include <vector>
//#include <iterator>
//#include <algorithm>
//#include <cassert>
//#include <fstream>
#include <string>

#include "common.h"

#include <boost/asio.hpp>

#include "server.h"

using namespace std;
namespace ba = boost::asio;
using boost::asio::ip::tcp;

using namespace std;

bool IsDebugOutput = false;

int main(int argc, char *argv[])
{

	#if (defined WIN32) || (defined WIN64)
		cout << MY_P_FUNC << endl;              // for debugging

		//MY_DEBUG_ONLY(TestBasic();)           // for debugging
	#else
		//istream &i_stream = cin;
	#endif

	MY_DEBUG_ONLY(cout << "Homework bulk_server (DEBUG detected)" << endl;)

	try
	{
		if (argc < 2)
		{
			cerr << "There must be 2 parameters: <port> <bulksize>" << endl;
			exit(0);
		}

		int port = -1, bulk_size = -1;

		sscanf(argv[1], "%i", &port);
		sscanf(argv[2], "%i", &bulk_size);

		if (port < 0 || bulk_size < 1)
		{
			cerr << "My Error: something's wrong with arguments" << endl;
			exit(0);
		}

		ba::io_service io_context;

		tcp::endpoint endpoint(tcp::v4(), port);
		BulkServer server(io_context, endpoint, bulk_size);

		io_context.run();
	}
	catch (std::exception& e)
	{
		cerr << "Exception from main: " << e.what() << endl;
	}

	//cout << "Done" << endl;

    return 0;
}
//--------------------------------------------------------------------




