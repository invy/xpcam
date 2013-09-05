#ifndef XPLANES_H
#define XPLANES_H

#include "XPLMPlanes.h"

class XPlanes : public IPlanes
{
public:
    virtual void getNthAircraftModel(int inIndex, std::string& outFileName, std::string& outPath) {
        char fname[256] = "";
        char path[256] = "";
        XPLMGetNthAircraftModel(inIndex, fname, path);
        outFileName = fname;
        outPath = path;
    }
};

#endif // XPLANES_H
