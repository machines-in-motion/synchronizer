#include "synchronizer/private/slave_sync.hpp"


namespace synchronizer {


  Slave_sync::Slave_sync(std::string memory_segment,
			 double async_frequency)
    : memory_segment_(memory_segment),
      async_frequency_(async_frequency)
  {
    reinit();
  }

  Slave_sync::~Slave_sync(){
    clean_memory();
  }

  void Slave_sync::clean_memory(){
    shared_memory::LockedConditionVariable::clean(memory_segment_, "cond_var");
    shared_memory::clear_shared_memory(memory_segment_);
  }

  // restarting fresh, i.e recreating the condition variable and the
  // shared memory segment
  void Slave_sync::reinit(){
    clean_memory();
    cv_.reset(new shared_memory::LockedConditionVariable(memory_segment_,"cond_var",false));
    // will be used to get info from the master that sync mode changed
    shared_memory::set(memory_segment_,"sync",false);
    // will be used to inform the master request to change mode
    // has been received
    shared_memory::set(memory_segment_,"sync_shake",false);
    is_sync_mode_ = false;
    previous_mode_ = false;
    // for running at desired frequency when in async mode
    spinner_.initialize();
    spinner_.set_frequency(async_frequency_);
  }
  
  
  void Slave_sync::wait(){
    cv_->wait();
  }

  void Slave_sync::notify(){
    cv_->notify_one();
  }

  // returning true if this instance should be destroyed
  // and replaced by a new one
  bool Slave_sync::pulse(){
    // reading from shared memory which mode
    // we are in
    bool reset = read_sync_mode();
    if(reset){
      // reset is true when switching from
      // sync to async. This instance should then
      // be destroyed and replaced by a new one
      return true;
    }
    if(is_sync_mode_){
      // in sync mode, we use the condition variable
      notify();
      wait();
    } else {
      // in async mode, we run at desired frequency
      spinner_.spin();
    }
    return false;
  }

  // returns true if reset is requested, i.e.
  // sync mode just been turned off
  bool Slave_sync::read_sync_mode(){
    // reading from the shared memory what mode the master
    // requests
    shared_memory::get(memory_segment_,"sync",is_sync_mode_);
    if(is_sync_mode_ && !previous_mode_){
      // switching from async to sync
      // telling the master we go the request
      shared_memory::set(memory_segment_,"sync_shake",true);
      // init the condition variable 
      cv_->lock_scope();
      wait();
    }
    if(previous_mode_ && !is_sync_mode_){
      // switching from sync to async.
      // this is solved by returning true,
      // i.e info this instance should be destroyed
      // and replaced by a new one (which will be async)
      previous_mode_ = is_sync_mode_;
      return true;
    }
    // to monitor mode switch
    previous_mode_ = is_sync_mode_;
    return false;
  }


}