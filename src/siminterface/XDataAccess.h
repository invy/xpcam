
#pragma once
#include <XPLMDataAccess.h>

#include "IDataAccess.h"

class XDataAccess : public IDataAccess {
public:
	virtual void* findDataRef(const std::string &s) {
		return XPLMFindDataRef(s.c_str());
    }
	virtual int getDatai(const void* ref) {
		return XPLMGetDatai((XPLMDataRef)ref);
    }
	virtual float getDataf(const void* ref) {
		return XPLMGetDataf((XPLMDataRef)ref);
    }
	virtual double getDatad(const void* ref) {
		return XPLMGetDatad((XPLMDataRef)ref);
    }
	virtual void setDatai(const void* ref, const int v) {
		XPLMSetDatai((XPLMDataRef)ref, v);
    }
	virtual void setDataf(const void* ref, const float v) {
		XPLMSetDataf((XPLMDataRef)ref, v);
    }
	virtual void setDatad(const void* ref, const double v) {
		XPLMSetDatad((XPLMDataRef)ref, v);
    }
};
