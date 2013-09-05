
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "XPLMUtilities.h"
#include "pluginmain.h"
#include "siminterface/XDataAccess.h"
#include "siminterface/XProcessing.h"
#include "siminterface/XDisplay.h"
#include "siminterface/XPlanes.h"

typedef PluginInstance<XDataAccess, XProcessing, XDisplay, XPlanes> XPluginInstance;

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
    strcpy(outName, "PilotHead Camera");
    strcpy(outSig, "pilothead.effects.camera");
    strcpy(outDesc, "A plugin that simulates pilot's head movements.");
    std::cout << "initializing plugin" << std::endl;
    XPluginInstance::getInstance().start();
    return 1;
}

PLUGIN_API void	XPluginStop(void)
{
    XPluginInstance::getInstance().stop();
}

PLUGIN_API int XPluginEnable(void)
{
    return XPluginInstance::getInstance().enable();
}

PLUGIN_API void XPluginDisable(void)
{
    XPluginInstance::getInstance().disable();
}


PLUGIN_API void XPluginReceiveMessage(
                    XPLMPluginID	/*inFromWho*/,
                    long			/*inMessage*/,
                    void *			/*inParam*/)
{
}

