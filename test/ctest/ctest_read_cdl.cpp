#include "../../src/cdl.h"

int read_cdl(const char * filepath) {
    colour::Cdl cdl(filepath);
    if(cdl.status == 1) return 1;
    cdl.printCDL();
    return 0;
}

int main() {
    return read_cdl("test.cdl");
}
