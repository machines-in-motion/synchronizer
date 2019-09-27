#pragma once

#include "synchronizer/private/slave_sync.hpp"


namespace synchronizer {

  class Slave {

  public:

    /**
     * @brief Tools for setting a process to run alternatively
     * asynchronously (i.e. at own desired frequency) or
     * synchronously to another process managed by a Master
     * using the same memory_segment id.
     * At startup, the slave will init to async mode, and 
     * will switch to synchronous if the function set_sync 
     * of the master is called.
     * Note that as slave needs to be started before its master,
     * and can survive several masters.
     * param[in] memory_segment segment id of the shared memory
     * shared between the master and the slave
     * param[in] frequency the slave will run at when in async mode 
     */
    Slave(std::string memory_segment_,
	  double async_frequency);

    ~Slave();
    
    /**
     * @brief Either sleep to matches the frequency (async mode)
     * or uses internally wait/notify functions to alternate run with
     * the master (sync mode)
     */
    void pulse();

  private:

    std::string memory_segment_;
    Slave_sync sync_;

  };
  
}
