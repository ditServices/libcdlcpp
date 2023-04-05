#include "../../src/cdl.h"

int read_cdl(const char * filepath) {
    colour::Cdl cdl(filepath);
    cdl.printCDL();
    return 0;
}

int main() {
    return read_cdl("test.cdl");
}
