PLUGIN_NAME = noise_generator

RTXI_INCLUDES = /usr/local/lib/rtxi_includes

HEADERS = noise-generator.h\
          $(RTXI_INCLUDES)/generator.h\
          $(RTXI_INCLUDES)/gen_whitenoise.h

SOURCES = noise-generator.cpp \
          moc_noise-generator.cpp\
          $(RTXI_INCLUDES)/generator.cpp\
          $(RTXI_INCLUDES)/gen_whitenoise.cpp

LIBS = -lgsl
 
### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
