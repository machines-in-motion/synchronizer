#include "synchronizer/slave.hpp"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>


#define SYNC_MEM "sync_mem"
#define COUNT_MEM "count_mem"

#define ASYNC_FREQUENCY 3

void execute(){

  shared_memory::clear_shared_memory(COUNT_MEM);
  shared_memory::set<bool>(COUNT_MEM,"running",true);
  
  synchronizer::Slave s(SYNC_MEM,ASYNC_FREQUENCY);

  int count = 0;
  shared_memory::set<int>(COUNT_MEM,"count",count);

  bool running = true;

  while (running) {

    shared_memory::get<bool>(COUNT_MEM,"running",running);
    if(!running){
      break;
    }

    shared_memory::get<int>(COUNT_MEM,"count",count);
    count++;
    shared_memory::set<int>(COUNT_MEM,"count",count);
    
    std::cout << "\t\tslave: " << count << "\n";

    s.pulse();
    
  }


}


int main(){

  execute();
  
}
