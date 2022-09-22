AUTOMAKE_OPTIONS = foreign

# Define library binary name
noinst_LIBRARIES = libRapidjson.a
# Define source files
libRapidjson_a_SOURCES = $(top_srcdir)/utilities/rapidjson/src/rapidjson.cpp

# Define includes directories
AM_CXXFLAGS=-I$(top_srcdir)/utilities/rapidjson/inc/ \
            -I$(top_srcdir)/utilities/string/inc/ \
            -I$(top_srcdir)/utilities/fs/file/inc/ \
            -I$(top_srcdir)/external/rapidjson/inc/

METASOURCES = AUTO
