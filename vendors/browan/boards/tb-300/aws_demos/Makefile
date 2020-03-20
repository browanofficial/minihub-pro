#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := aws_demos

ifndef AMAZON_FREERTOS_PATH
export AMAZON_FREERTOS_PATH := $(CURDIR)/../../../../..
endif

ifndef IDF_PATH
export IDF_PATH := $(AMAZON_FREERTOS_PATH)/vendors/browan/esp-idf
EXTRA_COMPONENT_DIRS :=  $(AMAZON_FREERTOS_PATH)/vendors/browan/boards/tb-300/aws_demos/application_code/browan_code \
                         $(AMAZON_FREERTOS_PATH)/vendors/browan/boards/tb-300/components

ifndef AFR_ESP_LWIP
EXTRA_COMPONENT_DIRS += $(AMAZON_FREERTOS_PATH)/vendors/browan/boards/tb-300/components/freertos_tcpip
else
CFLAGS += -DAFR_ESP_LWIP
endif

CFLAGS+=-Wno-unused-function -Wno-unused-variable
include $(IDF_PATH)/make/project.mk

else
$(error ERROR: IDF_PATH is defined in your environement, it will not point to vendors/browan/esp-idf. To remove the variable run the command "unset IDF_PATH")
endif


