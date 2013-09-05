PLUGINS="./"
SDK=/home/igor/projects/xpsdk/SDK

TARGET=lin.xpl
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

CFLAGS=-Wall -I$(SDK)/CHeaders/XPLM -fPIC -fno-stack-protector -std=c++11 -DXPLM200 -DLIN=1
LNFLAGS=-shared -rdynamic -nodefaultlibs 
LIBS=-L/usr/lib #$(PLUGINS)/XPLM.so $(PLUGINS)/XPWidgets.so
CC=clang++


all: $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) -c $<
	
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(LNFLAGS) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp -f $(TARGET) $(PLUGINS)

        
