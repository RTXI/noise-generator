PLUGIN_NAME = noise_generator

RTXI_INCLUDES =

HEADERS = noise-generator.h\

SOURCES = noise-generator.cpp \
          moc_noise-generator.cpp

LIBS = -lgsl -lrtgen
 
### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
