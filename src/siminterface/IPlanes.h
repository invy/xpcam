#ifndef IPLANES_H
#define IPLANES_H

class IPlanes
{
public:
    virtual void                 getNthAircraftModel(
                                       int                      inIndex,
                                       std::string&               outFileName,
                                       std::string&               outPath) = 0;
};

#endif // IPLANES_H
