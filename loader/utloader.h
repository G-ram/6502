#ifndef UTLOADER_H
#define UTLOADER_H

#include "loader.h"

class UnitTestLoader : public Loader {
public:
    UnitTestLoader(std::string);
};

#endif
