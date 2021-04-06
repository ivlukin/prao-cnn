
#include "Processor.h"

void Processor::printError(const char *message, int clError) {
    if (clError != 0) {
        std::cout << message << ". Error code: " << clError << std::endl;
        exit(-1);
    }
}
