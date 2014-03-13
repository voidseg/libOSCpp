EXTRA_DIST = README

noinst_HEADERS = sampleplayer.h OSCFunctionCallable.h

if LIBOSC_TESTS
noinst_PROGRAMS = newnames oscmatch mikmodserver

AM_CPPFLAGS = -I$(top_srcdir)/include

newnames_SOURCES = newnames.cc
newnames_LDADD   = $(top_builddir)/src/libOSC++.la

oscmatch_SOURCES = test-oscmatch.cc oscmatch.cc

mikmodserver_SOURCES = mikmod-server.cc sampleplayer.c
mikmodserver_LDFLAGS = -lmikmod -lpthread
mikmodserver_LDADD   = $(top_builddir)/src/libOSC++.la
endif
