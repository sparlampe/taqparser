#ifndef FILEADAPTER_HPP
#define FILEADAPTER_HPP

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/regex.hpp>
#include <fstream>
//#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::posix_time;

class FileAdapter
{

private:
	regex expression;
	ifstream InputFile;
	cmatch what;
	char line[1000]; 
	double currentPrice,lastPrice;
	ptime  currentDateTime, lastDateTime;
	long linenumber;
	long repeatCounter;

public:
	FileAdapter(const char* filename);
        virtual ~FileAdapter();  
	int NextTimePoint(double& price, ptime& datetime);
};


#endif
