#include <ctime>
#include <iostream>
struct timer {
    timer(std::ostream& report) : report(report)
       {start = std::clock();}
    ~timer()
       {
          using namespace std;
          end = clock();
          unsigned milliseconds = (end - start)*1000 / CLOCKS_PER_SEC;
          report << "Spent " << milliseconds << "ms here\n";  
       } 

    std::clock_t start;
    std::clock_t end;
    std::ostream& report;

private:
    // just to surpress a VC++ 8.0 warning
    timer& operator = (const timer&) {};
};
