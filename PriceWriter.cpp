#include "PriceWriter.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <string>
#include <math.h>
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::gregorian;
using namespace std;
using namespace boost;
using namespace boost::posix_time;

PriceWriter::PriceWriter(const time_duration& _returnInterval,const time_duration& _openTime, const time_duration& _closeTime)
{
	returnInterval=_returnInterval;
	openTime=_openTime;
	closeTime=_closeTime;
	currentDate=date(1900,1,1);
	isDayComplete=true;
}


void PriceWriter::Push(const ptime & newTimeDate,const double& _newPrice)
{
	newTime=newTimeDate.time_of_day();
	newDate=newTimeDate.date();
	newPrice=_newPrice;

//assumption is made that the first tick of the day is withing the first scheduled interval

	if(currentDate!=newDate)
	{
		CompleteCurrentDay();
		StartNewDay();
	}
	else if(newTime<=closeTime && newTime>openTime )
	{



					if(newTime==currentSchedTime)
					{
						cout<<to_simple_string(ptime(currentDate,currentSchedTime))<<" price "<<newPrice<<endl;
						lastTimeDateHolder=newTimeDate;
						lastPriceHolder=newPrice;
						
						currentSchedTime +=returnInterval;
					

					}		
					else if(newTime<currentSchedTime && newTime>lastTimeDateHolder.time_of_day())
					{
						lastTimeDateHolder=newTimeDate;
						lastPriceHolder=newPrice;
					}	
					else if(newTime>currentSchedTime)	
					{
						cout<<to_simple_string(ptime(currentDate,currentSchedTime))<<" price "
						<<AveragePrice(lastTimeDateHolder.time_of_day(),lastPriceHolder,currentSchedTime,newTime,newPrice)<<endl;

						lastTimeDateHolder=newTimeDate;
						lastPriceHolder=newPrice;
						currentSchedTime +=returnInterval;
					}		

	}
	else if(newTime>closeTime && !isDayComplete)
	{
		CompleteCurrentDay();
	}

	

}

void PriceWriter::CompleteCurrentDay()
{
	if(!isDayComplete)
	{	
		if(currentSchedTime<=closeTime)
		{
		cout<<to_simple_string(ptime(currentDate,currentSchedTime))<<" price "<<lastPriceHolder<<endl;
		currentSchedTime +=returnInterval;
		}
		
		while(currentSchedTime<=closeTime)
		{
			cout<<to_simple_string(ptime(currentDate,currentSchedTime))<<" price "<<" NA "<<endl;
			currentSchedTime +=returnInterval;

		}
		isDayComplete=true;
	}
}

void PriceWriter::StartNewDay()
{
	currentDate=newDate; 
	currentSchedTime=openTime;
	if (newTime>=currentSchedTime)
	{
		cout<<to_simple_string(ptime(currentDate,currentSchedTime))<<" price "<<newPrice<<endl;
		lastTimeDateHolder=ptime(newDate,newTime);
		lastPriceHolder=newPrice;
		currentSchedTime +=returnInterval;
		isDayComplete=false;
	}
}
	
double PriceWriter::AveragePrice(const time_duration lastTime, const double lastPrice, const time_duration schedTime, const time_duration nextTime, const double nextPrice)
{
	double averagePrice;
	long secondsLastToSchedule=(schedTime-lastTime).total_seconds();
	long secondsScheduleToNext=(nextTime-schedTime).total_seconds();
	averagePrice=(lastPrice*secondsLastToSchedule+nextPrice*secondsScheduleToNext)/(secondsLastToSchedule+secondsScheduleToNext);
	return averagePrice;
}


PriceWriter::~PriceWriter()
{}

