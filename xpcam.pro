# Shared library without any Qt functionality
TEMPLATE = lib
QT -= gui core webkit

CONFIG += warn_on plugin
CONFIG -= thread qt rtti

VERSION = 1.0.0

INCLUDEPATH += ../../SDK/CHeaders/XPLM
INCLUDEPATH += ../../SDK/CHeaders/Wrappers
INCLUDEPATH += ../../SDK/CHeaders/Widgets

INCLUDEPATH += src/

QMAKE_CXXFLAGS += -fPIC -fno-stack-protector -std=c++11 -DXPLM210 -DLIN=1 -g -O0
#QMAKE_LFLAGS += -mi386linux
#QMAKE_LFLAGS -= -ccc-gcc-name g++
QMAKE_LFLAGS += -fPIC -shared -rdynamic -nodefaultlibs

#QMAKE_LINK = ld

# Defined to use X-Plane SDK 2.0 capabilities - no backward compatibility before 9.0
DEFINES += XPLM210

CONFIG(debug, debug|release) {
    message("debug, debug|release")
    DEFINES+=DEBUG
}

CONFIG(release, debug|release) {
    message("release, debug|release")
}


win32 {
        DEFINES += APL=0 IBM=1 LIN=0
        LIBS += -L../../SDK/Libraries/Win
        LIBS += -lXPLM -lXPWidgets
        TARGET = win.xpl

}

unix:!macx {
        DEFINES += APL=0 IBM=0 LIN=1
        LIBS+= -lboost_serialization
        TARGET = lin.xpl
        # WARNING! This requires the latest version of the X-SDK !!!!
#        QMAKE_CXXFLAGS += -fvisibility=hidden -fPIC
#        QMAKE_CXXFLAGS += -std=c++11
}

macx {
        DEFINES += APL=1 IBM=0 LIN=0
        TARGET = mac.xpl
        QMAKE_LFLAGS += -flat_namespace -undefined suppress

        # Build for multiple architectures.
        # The following line is only needed to build universal on PPC architectures.
        # QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
        # The following line defines for which architectures we build.
        CONFIG += x86 ppc
}

HEADERS += \
    src/siminterface/XProcessing.h \
    src/siminterface/XDisplay.h \
    src/siminterface/XDataAccess.h \
    src/siminterface/IProcessing.h \
    src/siminterface/IDisplay.h \
    src/siminterface/IDataAccess.h \
    src/siminterface/IPlanes.h \
    src/siminterface/xplanes.h \
    src/headcamera_helpers.h \
    src/pluginmain.h \
    src/cameraengine.h \
    src/siminterface/simdata.h \
    src/effects/vibrationeffect.h \
    src/effects/cameraeffect.h \
    src/effects/camerarolleffect.h \
    src/effects/groundshakingeffect.h \
    src/effects/buffetingeffect.h \
    src/effects/gloadeffect.h \
    src/cameraposition.h \
    test/functional/camerarolleffectfunctionaltests.h \
    test/base/testdataaccess.h \
    test/serialization/serializationtest.h \
    src/Singleton.h

SOURCES += \
    src/xpcam.cpp \
    test/functional/camerarolleffectfunctionaltests.cpp \
    test/base/testdataaccess.cpp \
    test/serialization/serializationtest.cpp

HEADERS += \
    src/siminterface/XPlanes.h




