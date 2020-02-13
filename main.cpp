#include <iostream>
#include <vector>
//#include <iterator>
//#include <algorithm>
//#include <cassert>
#include <fstream>

#include "common.h"

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

	cout << "Done" << endl;

    return 0;
}
//--------------------------------------------------------------------




