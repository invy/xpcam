
#pragma once

typedef float (* flightLoop_f)(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon);

class IProcessing {
public:
	virtual void registerFlightLoopCallback(
                                   flightLoop_f     inFlightLoop,    
                                   float                inInterval,    
                                   void *               inRefcon) = 0;
	virtual void unregisterFlightLoopCallback(
                                   flightLoop_f     inFlightLoop,    
                                   void *               inRefcon) = 0;
};