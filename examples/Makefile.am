EXTRA_DIST = README

NULL =

noinst_PROGRAMS = \
	simpleTCPClient \
	simpleTCPServer \
	simpleUDPClient \
	simpleUDPServer \
	$(NULL)

AM_CPPFLAGS = -I$(top_srcdir)/include

simpleTCPClient_SOURCES = \
	simpleTCPClient.cc \
	$(NULL)
simpleTCPClient_LDADD = \
	$(top_builddir)/src/libOSC++.la \
	$(NULL)

simpleTCPServer_SOURCES = \
	simpleTCPServer.cc \
	$(NULL)
simpleTCPServer_LDADD = \
	$(top_builddir)/src/libOSC++.la \
	$(NULL)

simpleUDPClient_SOURCES = \
	simpleUDPClient.cc \
	$(NULL)
simpleUDPClient_LDADD = \
	$(top_builddir)/src/libOSC++.la \
	$(NULL)

simpleUDPServer_SOURCES = \
	simpleUDPServer.cc \
	$(NULL)
simpleUDPServer_LDADD = \
	$(top_builddir)/src/libOSC++.la \
	$(NULL)
