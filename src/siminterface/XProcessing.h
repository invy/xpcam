
#include <XPLMProcessing.h>
#include "IProcessing.h"

class XProcessing : public IProcessing {
public:
	virtual void registerFlightLoopCallback(
                                   flightLoop_f     inFlightLoop,    
                                   float                inInterval,    
                                   void *               inRefcon)
	{
		XPLMRegisterFlightLoopCallback((XPLMFlightLoop_f)inFlightLoop, inInterval, inRefcon);
	}
	virtual void unregisterFlightLoopCallback(
                                   flightLoop_f     inFlightLoop,    
                                   void *               inRefcon)
	{
		XPLMUnregisterFlightLoopCallback((XPLMFlightLoop_f)inFlightLoop, inRefcon);
	}
};