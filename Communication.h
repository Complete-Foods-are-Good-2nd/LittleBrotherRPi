#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <set>
#include <string>
#include <vector>

class Communication {
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
