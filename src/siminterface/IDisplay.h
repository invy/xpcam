
#pragma once

typedef void (* hotKey_f)(void *inRefcon);

class IDisplay {
public:
	virtual void *registerHotKey(char                 inVirtualKey,    
                                   unsigned         inFlags,    
                                   const char *         inDescription,    
                                   hotKey_f         inCallback,    
                                   void *               inRefcon) = 0;
	virtual void unregisterHotKey(void *inHotKey) = 0;
};
