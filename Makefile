PLUGIN_NAME = noisegen

HEADERS = noisegen.h

LIBS = -lgsl

SOURCES = noisegen.cpp \
          moc_noisegen.cpp\
          ../include/gen_whitenoise.cpp\
          ../include/gen_whitenoise.h\
          ../include/generator.cpp\
          ../include/generator.h

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
