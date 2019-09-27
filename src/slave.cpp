#include "synchronizer/slave.hpp"


namespace synchronizer {

  
  Slave::Slave(std::string memory_segment,
	       double async_frequency)
    : memory_segment_(memory_segment),
      sync_(memory_segment,async_frequency)
  {

    // making sure two slaves do not coexist on the same memory segment
    if (shared_memory::segment_exists(memory_segment+"_slave")){
      throw std::runtime_error(std::string("A synchronizer::slave is contructed under memory segment ")+
			       memory_segment+
			       std::string(", which has already another slave active"));
    } else {
      shared_memory::set<bool>(memory_segment+"_slave",
			       "slave",true);
    }

  }

  
  Slave::~Slave()
  {
    shared_memory::clear_shared_memory(memory_segment_+"_slave");
    shared_memory::delete_segment(memory_segment_+"_slave");
  }

  
  void Slave::pulse()
  {
    bool reset = sync_.pulse();
    if (reset){
      sync_.reinit();
      sync_.pulse();
    }
  }

}
