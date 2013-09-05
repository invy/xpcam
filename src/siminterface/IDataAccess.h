
#pragma once

#include <string>

class IDataAccess {
public:
	virtual void* findDataRef(const std::string &) = 0;
	virtual int getDatai(const void*) = 0;
	virtual float getDataf(const void*) = 0;
	virtual double getDatad(const void*) = 0;
	virtual void setDatai(const void*, const int) = 0;
	virtual void setDataf(const void*, const float) = 0;
	virtual void setDatad(const void*, const double) = 0;
};
