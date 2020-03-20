# C SDK LoRa Basics Station demo
if(NOT RUN_IN_IDT_MODE)
    afr_demo_module(basicstation)
endif()

afr_set_demo_metadata(ID "BASIC_STATION_DEMO")
afr_set_demo_metadata(DESCRIPTION "An example that demonstrates LoRa Basics Station")
afr_set_demo_metadata(DISPLAY_NAME "LoRa Basics Station Demo")

if(NOT RUN_IN_IDT_MODE)
    afr_module_sources(
        ${AFR_CURRENT_MODULE}
        INTERFACE
            "${CMAKE_CURRENT_LIST_DIR}/lora_basic_station_demo.c"
    )

    afr_module_dependencies(
        ${AFR_CURRENT_MODULE}
        INTERFACE
            AFR::mqtt
    )
else()
       message("INFO >> demos/lora_basic_station/CMakeLists.txt: RUN_IN_IDT_MODE")
endif()
