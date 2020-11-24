#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <set>
#include <unistd.h>
#include <boost/format.hpp>

class Communication
{
private:
    std::set<std::string> get_serial_device_names();
    std::pair<std::string, std::string> generate_loopback_serial_devices();
public:
    std::string serial_device_dir;
    std::string momo_serial_device_name;
    std::string my_serial_device_name;

    Communication();
    // ~Communication();
    void start_stream();
};
