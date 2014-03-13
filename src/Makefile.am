lib_LTLIBRARIES = libOSC++.la

STD_CXXFLAGS = -Wall
STD_CFLAGS   = -Wall

if LIBOSC_DEBUG
DEBUG_CXXFLAGS = -ggdb
DEBUG_CFLAGS   = -ggdb
endif

if LIBOSC_VERBOSE
VERBOSE_CXXFLAGS = -DLIBOSC_DEBUG
VERBOSE_CFLAGS = -DLIBOSC_DEBUG
endif

if LIBOSC_REPLY
REPLY_CXXFLAGS = -DLIBOSC_REPLY
REPLY_CFLAGS = -DLIBOSC_REPLY
endif

AM_CXXFLAGS = $(STD_CXXFLAGS) $(DEBUG_CXXFLAGS) $(VERBOSE_CXXFLAGS) $(REPLY_CXXFLAGS)
AM_CFLAGS = $(STD_CFLAGS) $(DEBUG_CFLAGS) $(VERBOSE_CFLAGS) $(REPLY_CFLAGS)

INCLUDES = -I$(top_srcdir)/include

libOSC___la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @LIBOSC_LDFLAGS@
libOSC___la_SOURCES = \
	OSC-string-help.c \
	OSCAssociativeNamespace.cc \
	OSCNamespaceItem.cc \
	OSCCCallable.cc \
	OSCCallable.cc \
	OSCLister.cc \
	OSCPacker.cc \
	OSCProcessor.cc \
	OSCUnpacker.cc \
	Processor.cc \
	InetOSC.cc \
	InetTCPMaster.cc \
	InetTCPSlave.cc \
	InetUDPMaster.cc \
	Transmit.cc \
	NullTransmit.cc \
	InetTransportManager.cc \
	OSCHierarchicalNamespace.cc \
	oscmatch.cc

if LIBOSC_REPLY
    libOSC___la_SOURCES += InetUDPReplier.cc
endif
