#include "ØlkastPkg.h"
#include "TelepilsPkg.h"


enum ProtocolDescriptor : uint8_T {
    ØLKAST = 1,
    TELEPILS = 2,
};



ProtocolDescriptor protocol_of_binary_pkg(pkg){
    // 
}


bool pkg_is_valid(pkg){
    ProtocolDescriptor protocol = protocol_of_binary_pkg(pkg);
    verdict = true;

    verdict = (sizeof(pkg) != sizeof(ØlkastPkg)); /// Hmmm, parametrisk hvordan?
    // parity check?

    return verdict;
}

