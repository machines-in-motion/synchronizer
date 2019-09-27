#include "synchronizer/slave.hpp"
#include "synchronizer/master.hpp"
#include "real_time_tools/realtime_check.hpp"
#include "real_time_tools/spinner.hpp"
#include "real_time_tools/thread.hpp"
#include "gtest/gtest.h"


using namespace synchronizer;


class Synchronizer_tests: public ::testing::Test {
protected:
  void SetUp(){}
  void TearDown(){}
};


TEST_F(Synchronizer_tests,frequency_aync){

  Slave s("ut_sync",250);

  real_time_tools::Realtime_check rc(100,200);

  for(int i=0;i<100;i++){
    s.pulse();
    rc.tick();
  }

  int t,st;
  double tf,sw,cf;
  double average_frequency,wf;

  rc.get_statistics(t,st,tf,sw,average_frequency,
		    cf,wf);

  ASSERT_LT(average_frequency,350);
  ASSERT_GT(average_frequency,250);

}

void* _run_slave(void *freq){

  shared_memory::clear_shared_memory("ut_sync_exchange");
  
  double frequency = *(static_cast<double*>(freq));

  shared_memory::set("ut_sync_exchange","running",true);

  bool running = true;

  Slave s("ut_sync",frequency);

  real_time_tools::Realtime_check rc(frequency,2*frequency);

  while(running){
    s.pulse();
    rc.tick();
    shared_memory::get("ut_sync_exchange","running",running);
  }

  int t,st;
  double tf,sw,cf;
  double average_frequency,wf;

  rc.get_statistics(t,st,tf,sw,average_frequency,
		    cf,wf);

  shared_memory::set("ut_sync_exchange","frequency",average_frequency);
  
}


TEST_F(Synchronizer_tests,test_sync){


  
  // creating a slave thread with async frequency
  // of 100
  
  real_time_tools::RealTimeThread thread;
  double frequency = 100.0;
  thread.create_realtime_thread(_run_slave,&frequency);
  usleep(1000);

  // creating a master requesting a sync
  // frequency of 300

  real_time_tools::Spinner spinner;
  spinner.set_frequency(300);
  Master master("ut_sync");
  master.start_sync();
  for(int iteration=0;iteration<100;iteration++){
    master.pulse();
    spinner.spin();
  }

  // requesting slave to stop
  
  master.stop_sync();
  shared_memory::set("ut_sync_exchange","running",false);
  thread.join();


  // getting frequency the slave was running at, should be around 300
  
  double slave_frequency;
  shared_memory::get("ut_sync_exchange","frequency",slave_frequency);
  ASSERT_GT(slave_frequency,250);
  ASSERT_LT(slave_frequency,350);


}



TEST_F(Synchronizer_tests,test_throw_stop_before_start){

  
  real_time_tools::RealTimeThread thread;
  double frequency = 100.0;
  thread.create_realtime_thread(_run_slave,&frequency);
  usleep(2000);
  
  real_time_tools::Spinner spinner;
  spinner.set_frequency(300);
  Master master("ut_sync");

  ASSERT_THROW(master.stop_sync(),
	       std::runtime_error);

  shared_memory::set("ut_sync_exchange","running",false);
  thread.join();

  
}


TEST_F(Synchronizer_tests,test_throw_if_no_slave){

  ASSERT_THROW( Master master("ut_sync"),
		std::runtime_error );
  
}


TEST_F(Synchronizer_tests,test_no_two_masters){

  Slave slave("ut_sync",0);
  Master master("ut_sync");
  ASSERT_THROW( Master master2("ut_sync"),
		std::runtime_error );
  
}


TEST_F(Synchronizer_tests,test_no_two_slaves){

  Slave slave("ut_sync",0);
  ASSERT_THROW( Slave slave2("ut_sync",0),
		std::runtime_error );
}
