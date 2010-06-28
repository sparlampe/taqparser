#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include "FileAdapter.hpp"
#include "PriceWriter.hpp"

using namespace std;
using namespace boost;
using namespace boost::posix_time;


int main(int argc, char **argv)
{

try
{
	ptime  fileTimeDate; //(date(2002,Jan,10),time_duration(1,2,3));
	time_duration returnInterval(0,5,0),openTime(9,30,0),closeTime(16,0,0);
	double filePrice;

	FileAdapter fileAccess("spyday.txt");
	PriceWriter priceWriter(returnInterval,openTime,closeTime); 
	long readResult;

		do{
			readResult=fileAccess.NextTimePoint(filePrice,fileTimeDate);

			if (readResult==1)
				throw "the regex did not match, aborting";
			else if(readResult==2)
			{
				priceWriter.CompleteCurrentDay();
			//	priceWriter.Close();
			}
			else
			{
				priceWriter.Push(fileTimeDate,filePrice);
			}
		}while(readResult==0);

}
catch (char const * str){
cout<<"exception: "<<str<<endl;
}

return 0;

}

//g++ taqparser.cpp -lboost_regex-mt -lboost_date_time-mt -lboost_program_options-mt -o taqparser 




