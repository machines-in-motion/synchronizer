#include "synchronizer/master.hpp"

namespace synchronizer {


  Master::Master(std::string memory_segment)
    : memory_segment_(memory_segment)
  {
    
    sync_.reset(new Master_sync(memory_segment));

    // making sure two masters do not coexist on the same memory segment
    if (shared_memory::segment_exists(memory_segment+"_master"))
      {
	throw std::runtime_error(std::string("A synchronizer::master is contructed under memory segment ")+
				 memory_segment+
				 std::string(", which has already another master active"));
      } else {
      shared_memory::set<bool>(memory_segment+"_master",
			       "master",true);
    }
    
  }

  
  Master::~Master()
  {
    if(sync_!=nullptr){
      try {
	stop_sync();
      } catch(...){}
    }
    shared_memory::clear_shared_memory(memory_segment_+"_master");
    shared_memory::delete_segment(memory_segment_+"_master");
  }
  
  
  void Master::start_sync(long int timeout_us)
  {
    
    if(sync_ == nullptr)
      {
	sync_.reset(new Master_sync(memory_segment_));
      }
    
    sync_->start_sync(timeout_us);
    
  }

  
  void Master::stop_sync()
  {
    
    sync_->stop_sync();
    sync_.reset(nullptr);
    
  }

  
  void Master::pulse()
  {
    
    sync_->pulse();
    
  }


}
