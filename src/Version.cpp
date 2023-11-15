#include "Version.h"
#include "ProjectVersion.h"

NS_CUSTOM_BEGIN

    std::string fullVersion() {
        return ProjectVersion::fullVersion();
    }

NS_CUSTOM_END