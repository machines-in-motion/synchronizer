#include "synchronizer/master.hpp"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>


#define SYNC_MEM "sync_mem"
#define COUNT_MEM "count_mem"


// demo_slave or demo_slave.py should be started first.

// expected results:
// demo_slave runs at 3 Hz.
// when demo_master starts, it switches to run to
// "as fast as possible" (start_sync is called)
// then back to 3Hz ("stop_sync" is called),
// then back to as fast as possible (start_sync)


void looping(synchronizer::Master &s)
{
  for (int iteration=0;iteration<50000;iteration++)
    {
      int count;
      shared_memory::get<int>(COUNT_MEM,"count",count);
      count++;
      shared_memory::set<int>(COUNT_MEM,"count",count);
      std::cout << "\t\tmaster: " << count << "\n";
      s.pulse();
    }
}


void execute()
{

  synchronizer::Master s(SYNC_MEM);

  s.start_sync(1000000);
  looping(s);
  
  std::cout << "stopping sync mode for 2 seconds\n";
  s.stop_sync();
  usleep(2000000);
  std::cout << "restarting sync mode\n";

  s.start_sync(1000000);
  looping(s);
  
  std::cout << "EXIT\n";
  
  // turning off slave
  shared_memory::set<bool>(COUNT_MEM,"running",false);
  
}


int main(){

  execute();
  
}
