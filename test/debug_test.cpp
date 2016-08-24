#include "kdebug.h"
#include <unistd.h>
#include <iostream>

using namespace kdebug;

int main(){
    std::cout<< kdebug::log.back();
    LOG(null) << "info logging";
    LOG(info) << "info logging";
    sleep(1);

    kdebug::log.set_fileoutput("loggin.txt");

    LOG(warning) << "warning logging";
    LOG(error) << "error logging";

    LOG(file) << "file logging"<<1;
    LOG(file) << "file logging"<<2;

    log.list(std::cout);
    //std::cout << kdebug::debug.back();
    // std::cout << "test\n";
    return 0;
}
