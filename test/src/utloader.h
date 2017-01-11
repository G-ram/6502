#ifndef UTLOADER_H
#define UTLOADER_H

#include "headers/loader.h"

class UnitTestLoader : public Loader {
public:
    UnitTestLoader(std::string);
};

#endif
