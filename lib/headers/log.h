#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <bitset>

#define LOG(msg) \
    std::cout << __FILE__ << "(" << __LINE__ << ") => " << msg << std::endl

#define ELOG(msg) \
    std::cerr << __FILE__ << "(" << __LINE__ << ") => " << msg << std::endl

#define GLOG(msg) \
    std::cerr << msg << std::endl

#define GNLOG(msg) \
    std::cerr << msg

std::string hexify(const std::string&);
std::string hexify(const unsigned char& input);
std::string hexify(const unsigned short& input);
std::string binify(const unsigned char& input);
std::string binify(const unsigned short& input);
#endif
