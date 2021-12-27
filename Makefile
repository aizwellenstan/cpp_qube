CXX = g++

# Change this to point to your Qube installation
SRCTOP = ../..
# SRCTOP = /usr/local/pfx/qube
# SRCTOP = /Applications/pfx/qube

QB_INC = -I$(SRCTOP)/include
INCS = $(QB_INC)

QBPACKAGE_LIB = -L$(SRCTOP)/lib -lqbpackage
QBAPI_LIB = -L$(SRCTOP)/lib -lQbApi

Z_LIB = -lz
SSL_LIB = 
PCRE_LIB = -lpcre
CRYPTO_LIB = -lcrypto
PTHREAD_LIB = -lpthread
EXPAT_LIB = -lexpat
GLOBAL_LIB = $(Z_LIB) $(SSL_LIB) $(PTHREAD_LIB) $(PCRE_LIB) $(EXPAT_LIB)

LIBS = $(QBAPI_LIB) $(QBPACKAGE_LIB) $(GLOBAL_LIB)

# For Mac OSX, need something like the following
# LIBS += /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/CoreServices.framework/CoreServices /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/IOKit.framework/IOKit


CXXFLAGS = $(INCS) -g

PROGRAMS=callback1 callback2 qbjobs prioritymod rangesubmit updateworkerresources updateworkerproperties deleteworkerresources deleteworkerproperties

all: $(PROGRAMS)

callback1: callback1.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

callback2: callback2.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

qbjobs: qbjobs.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

prioritymod: prioritymod.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

rangesubmit: rangesubmit.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

updateworkerresources: updateworkerresources.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

updateworkerproperties: updateworkerproperties.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

deleteworkerresources: deleteworkerresources.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

deleteworkerproperties: deleteworkerproperties.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(PROGRAMS) *.o *~
