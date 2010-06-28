#ifndef PRICEWRITER_HPP
#define PRICEWRITER_HPP

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/regex.hpp>
#include <fstream>
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::gregorian;
using namespace std;
using namespace boost;
using namespace boost::posix_time;

class PriceWriter
{

private:
	time_duration returnInterval, openTime, closeTime, currentSchedTime,newTime;
	ptime lastTimeDateHolder;
	date currentDate,newDate;
	double lastPriceHolder, newPrice;
	bool isDayComplete;
	
	double AveragePrice(const time_duration, const double, const time_duration, const time_duration, const double);
	void StartNewDay();



public:
	void Push(const ptime& fileTimeDate,const double& filePrice);
	PriceWriter( const time_duration& ,const time_duration& , const time_duration& );
	void CompleteCurrentDay(); 
//	void Close();
        virtual ~PriceWriter();  
};


#endif
