#include "FileAdapter.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <string>
#include <math.h>
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::gregorian;
using namespace std;
using namespace boost;
using namespace boost::posix_time;

FileAdapter::FileAdapter(const char* filename)
{
	 InputFile.open(filename,ifstream::in);
	 expression=regex("(SPY)[[:space:]]+([[:digit:]]{8,8})[[:space:]]+([[:digit:]]{1,2}):([[:digit:]]{2,2}):([[:digit:]]{2,2})[[:space:]]+([[:digit:]]*.[[:digit:]]*)[[:space:]]*");

	if(!InputFile.eof())
	{
		InputFile.getline(line,1000);
		linenumber=1;

		//cout<<line<<endl;
		
		if(regex_match(line, what, expression))
		   {
			lastPrice=atof(what[6].first);
			//cout<<lastPrice<<endl;
			//cout<<string(what[2].first,what[2].second)<<endl;
		//	cout<<what[3]<<endl;
			lastDateTime=ptime(from_undelimited_string(string(what[2].first,what[2].second)),
						time_duration(atoi(what[3].first),
							      atoi(what[4].first),
							      atoi(what[5].first)
							      )
					   );
			repeatCounter=1;
		    }
		else
		{
		throw "the first line in the file could not match";
		}

	}
	else{
	throw "the file read faild on initialisation";
	}
}


FileAdapter::~FileAdapter()
{
	InputFile.close();
}
 

int FileAdapter::NextTimePoint(double & price, ptime & datetime)
{
	while(!InputFile.eof())	
	{


		InputFile.getline(line,1000);
		linenumber++;

		if(InputFile.eof())
			break;
		
		if(regex_match(line, what, expression))
			{
				currentPrice=atof(what[6].first);
				//cout<<currentPrice<<endl;
				//cout<<string(what[2].first,what[2].second)<<endl;
				//cout<<what[3]<<endl;
				currentDateTime=ptime(from_undelimited_string(string(what[2].first,what[2].second)),
							time_duration(atoi(what[3].first),
								      atoi(what[4].first),
								      atoi(what[5].first)
								      )
						   );

			}
		else
			{
				cout<< "regex hasnot matched at line "<<linenumber<<endl;
				return 1;
			}
		if(currentDateTime!=lastDateTime)
		{
			price=lastPrice;
			datetime=lastDateTime;
			repeatCounter=1;
			lastPrice=currentPrice;
			lastDateTime=currentDateTime;
			return 0;
		}
		else
		{       repeatCounter++;
			//cout<<currentPrice<<endl;
			//cout<<repeatCounter<<endl;
			//cout<<"Curr/ReC-1 "<<(currentPrice/(repeatCounter-1))<<endl;
			//cout<<"LastP"<<lastPrice<<endl;
			lastPrice=lastPrice+(currentPrice/((double)repeatCounter-1.0));
			//cout<<"LastP"<<lastPrice<<endl;
			//cout<<"Dev "<<(((double)repeatCounter-1.0)/(double)repeatCounter)<<endl;
			lastPrice=lastPrice*(((double)repeatCounter-1.0)/(double)repeatCounter);
					
		}
	}	

	price=lastPrice;
	datetime=lastDateTime;
	return 2;
}
