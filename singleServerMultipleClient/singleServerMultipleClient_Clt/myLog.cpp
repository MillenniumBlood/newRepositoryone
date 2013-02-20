#include "myLog.h"

//默认、带日志文件路径、带日志级别日志构造函数
MyLog::MyLog(const string &fileName, enum logLevels levelIn)
{
	time(&startTime);
    logLevel = levelIn;
    init(fileName);
	//init(fileName,LOG_APPEND);
}

MyLog::~MyLog()
{
   if ( logLevel<QUIET_MODE )
   {
      clear(ios::goodbit);
      *this << endl;
      printHeader(1);    // add ending time to log file
   }
   close();
}

void MyLog::init(const string& fileName, int mode)
{
   /*if ( (fileName.c_str())[0] )
      openLog(fileName,mode);
   else*/
      openLog("syslog.log", mode);
}

void MyLog::openLog(const string& fileName, int mode)
{
   if (logLevel < QUIET_MODE)
   {
      open(fileName.c_str(),mode);
      
      // fail() returns a null zero when operation fails   
      // rc = (*this).fail();
    
      if ( fail() == 0 )
      {
         logName = fileName;
         printHeader(0);         // insert start time into top of log file
      }
      else
      {
         cout << "ERROR: Log file " << fileName.c_str() 
              << " could not be opened for write access." << endl;
         logLevel = QUIET_MODE;
      }
   }
   else
   {
      cout << "Logging disabled (QUIET_MODE set)" << endl;
   }
}

void MyLog::printHeader(int theEnd)
{
   if ( logLevel < QUIET_MODE )
   {
      clear(ios::goodbit);

      // setup time
      time_t sttime;
      time(&sttime);
	  struct tm tim={0};

      // convert to gm time
      gmtime_s(&tim, &sttime);

      // set data items
      int sec  = tim.tm_sec;           // second (0-61, allows for leap seconds)
      int min  = tim.tm_min;           // minute (0-59)
      int hour = tim.tm_hour;          // hour (0-23)

      int mon  = tim.tm_mon + 1;       // month (0-11)
      int mday = tim.tm_mday;          // day of the month (1-31)
      int year = tim.tm_year % 100;    // years since 1900

      char cur_time[10];
      char cur_date[10];

      sprintf_s(cur_time,10,"%02d:%02d:%02d",hour,min,sec);
      sprintf_s(cur_date,10,"%02d/%02d/%02d",mon,mday,year);

      char line_2[61];
      sprintf_s(line_2,61,"DATE: %s - %s%30s",cur_date,cur_time,logName.c_str());
      *this << line_2 << endl << endl;

      if (theEnd) *this << getExecTime() << endl;
   }
        
} 

void MyLog::getExecTime(int* min, int* sec)
{
   time_t endTime;
   time(&endTime);
   *min = (int)((endTime - startTime)/60);
   *sec = (int)((endTime - startTime)%60);
} 

char* MyLog::getExecTime()
{
    int min = 0;
    int sec = 0;
    getExecTime(&min, &sec);
    static char execTime[128];
    sprintf_s(execTime, 128, "Execution time: %d minutes %d seconds", min, sec);
    return execTime;
}
