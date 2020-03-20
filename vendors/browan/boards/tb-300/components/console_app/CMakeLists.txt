
if(NOT RUN_IN_IDT_MODE)
    set(COMPONENT_SRCS 
        "main/console_example_main.c"
        "cmd_nvs/cmd_nvs.c"
        "cmd_system/cmd_system.c"
        "cmd_wifi/cmd_wifi.c"
    )
endif()

set(COMPONENT_ADD_INCLUDEDIRS 
    "main"
    "cmd_nvs"
    "cmd_system"
    "cmd_wifi"
    "${board_components_dir}/cmd_browan"
)

set(COMPONENT_REQUIRES
    console
    fatfs
    spi_flash
    nvs_flash
)

if(NOT RUN_IN_IDT_MODE)
    register_component()
else()
    message("INFO >> vendors/browan/boards/tb-300/components/console_app/CMakeLists.txt: RUN_IN_IDT_MODE")
endif()
