#include "Version.h"
#include <iostream>
#include "log/log.h"

int main()
{
    std::cout << "Hello World!" << std::endl;
    LOGD("version:%s", NS_CUSTOM::fullVersion().c_str());
    return 0;
}