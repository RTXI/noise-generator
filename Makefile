PLUGIN_NAME = noisegen

HEADERS = noisegen.h\
          include/gen_whitenoise.h\
          include/generator.h

SOURCES = noisegen.cpp \
          moc_noisegen.cpp\
          include/gen_whitenoise.cpp\
          include/generator.cpp

LIBS = -lgsl

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
