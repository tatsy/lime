#ifdef _MSC_VER
#pragma once
#endif

#ifndef _PYLIME_PYUTILS_H_
#define _PYLIME_PYUTILS_H_

#include <string>
#include <exception>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace py = boost::python;
namespace npy = boost::python::numpy;

class PyLimeException : public std::exception {
public:
    PyLimeException(const std::string &msg)
    : msg_(msg) {
    }
    
    const char * what() const throw() {
        return msg_.c_str();
    }
    
private:
    std::string msg_;
};


#endif  // _PYLIME_PYUTILS_H_
