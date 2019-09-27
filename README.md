# Synchronizer

## Overview

Allows a master process to synchronize with a slave process.
The mode (synchronous or asynchronous) can be changed programmatically during runtime.

For exemple, in the slave process:

```cpp

# running at 200Hz when in async mode
double frequency = 200;

# will have loop running sync or async
synchronizer::Slave synchronizer("synchronizer_id",frequency);

while (running)
{
   # code doing things

   # pulsing instead of sleeping
   synchronizer.pulse();
}

```
and in the master process:

```cpp

synchronizer::Master master ("synchronizer_id");

master.start_sync();
for(int i=0;i<1000;i++)
{
    # code doing thing

    # sleeping to get desired frequency
   usleep(1000); # 

    # requesting the slave to run one iteration
    master.pulse()

}

master.stop_sync();

```

The expected behavior:

- when slave is started, it runs at 200Hz
- when master is started, master and slave run at same frequency (~ 1Khz)
- when master exit (calls stop_sync), slave returns at 200Hz

## Python

Python bindings are generated via Pybind11. 
Python/c++ slave/master processes  are compatible one with another (e.g. slave in python, master in cpp)

## Demos

For concrete examples (c++ and python), look at the demos folder. Note that the slave must be started before master.

## Limitation

A slave can be linked to only one master (at a time), and a master to one slave.

## Installation and compilation

This is a catkin package. 
Recommanded method for cloning is to use [treep](https://pypi.org/project/treep/) with configuration [treep_machines_in_motion](https://github.com/machines-in-motion/treep_machines_in_motion) and project "CORE_ROBOTICS"

## Licence

BSD-3 clause

## Copyright

Max Planck Gesellschaft,  University of New York

## Author

Vincent Berenz, Max Planck Institute for Intelligent Systems


