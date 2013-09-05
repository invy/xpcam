#pragma once

#include <XPLMDisplay.h>
#include <XPLMUtilities.h>

#include "IDisplay.h"


class XDisplay : public IDisplay {
public:
	virtual void *registerHotKey(char                 inVirtualKey,    
                                   unsigned         inFlags,    
                                   const char *         inDescription,    
                                   hotKey_f         inCallback,    
                                   void *               inRefcon)
	{
		return XPLMRegisterHotKey(inVirtualKey, (XPLMKeyFlags)inFlags, 
				inDescription,
				(XPLMHotKey_f)inCallback,
				inRefcon);
	}
	virtual void unregisterHotKey(void *inHotKey)
	{
		XPLMUnregisterHotKey(inHotKey);
	}
};
