#pragma once

#include "synchronizer/private/master_sync.hpp"


namespace synchronizer {

  class Master {

  public:

    /**
     * @brief Tools for setting a process managed by a Slave
     * to run either asynchronously to the process managed 
     * by the master (i.e. both processes run in parallel ignoring
     * each other) or to run synchronously to another process managed 
     * the Master (i.e. the two processes run alternatively)
     * At startup, the slave will init to async mode, and 
     * will switch to sync mode when the corresponding call to 
     * the start_sync of the master is called.
     * Note that as slave needs to be started before its master,
     * and can survive several masters.
     * param[in] memory_segment segment id of the shared memory
     * shared between the master and the slave
     */
    Master(std::string memory_segment);

    
    ~Master();


    /** @brief Request the master and the slave to 
     * start running synchronously
     * param[in] the timeout (in micro seconds)
     *  the master should wait to get the 
     * acknowledgment from the slave the request to run sync has
     * been received. Infinite wait if negative.
     */
    void start_sync(long int timeout_us=-1);

    
    /** @brief Request the master and slave to 
     * switch to async mode
     */
    void stop_sync();
    
    /**
     * @brief Either does nothing (async mode)
     * or uses internally wait/notify functions to alternate run with
     * the slave (sync mode)
     */
    void pulse();
  
  private:

    std::string memory_segment_;
    std::unique_ptr<Master_sync> sync_;
  
  };


}
