#include "synchronizer/master.hpp"
#include "synchronizer/slave.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>


using namespace synchronizer;


PYBIND11_MODULE(synchronizer,m){

  pybind11::class_<Slave>(m,"Slave")
    .def(pybind11::init<std::string,double>())
    .def("pulse",&Slave::pulse);

  pybind11::class_<Master>(m,"Master")
    .def(pybind11::init<std::string>())
    .def("pulse",&Master::pulse)
    .def("start_sync",&Master::start_sync,
	 pybind11::arg("timeout_us")=-1)
    .def("stop_sync",&Master::stop_sync);
    
}


