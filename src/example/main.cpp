#include "Version.h"
#include <iostream>
#include "log/log.h"
#include "utils/utils.h"
#include "Version.h"

int main()
{
    example::Utils::PrintSystem();
    example::Utils::TestLogLevel();

    LOGD("version:%s", NS_CUSTOM::fullVersion().c_str());
    return 0;
}