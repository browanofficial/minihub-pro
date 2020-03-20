
set(cmd_browan_inc_directories
    "${extra_components_dir}/esp32-wifi-manager/src"
    "${AFR_MODULES_C_SDK_DIR}/standard/common/include/private"
    "${AFR_ROOT_DIR}/libraries/freertos_plus/standard/pkcs11/include"
    "${AFR_ROOT_DIR}/libraries/3rdparty/pkcs11"
    "${AFR_DEMOS_DIR}/dev_mode_key_provisioning/include"
    "${AFR_DEMOS_DIR}/include"
    "${extra_components_dir}/cmd_browan/deps/inc/lora_gateway"
    "${extra_components_dir}/cmd_browan/deps/inc/lora_config"
    "${extra_components_dir}/cmd_browan/deps/inc/basicstation"
    "."
)

if(RUN_IN_IDT_MODE)
    set(cmd_browan_inc_directories
        ${cmd_browan_inc_directories}
    )
endif()

set(COMPONENT_ADD_INCLUDEDIRS
    ${cmd_browan_inc_directories}
)

if(NOT RUN_IN_IDT_MODE)
    set(COMPONENT_SRCDIRS
        "."
    )

    set(COMPONENT_REQUIRES
        console
        nvs_flash
    )

    register_component()
    target_link_libraries(${COMPONENT_TARGET} "${extra_components_dir}/cmd_browan/deps/lib/libbasicstation.a")
    target_link_libraries(${COMPONENT_TARGET} "${extra_components_dir}/cmd_browan/deps/lib/liblora_config.a")
    target_link_libraries(${COMPONENT_TARGET} "${extra_components_dir}/cmd_browan/deps/lib/liblora_gateway.a")

    #add_definitions(-DCFG_selftests)
    add_definitions(-DCFG_esp32_porting)
else()
    message("INFO >> vendors/browan/boards/tb-300/components/cmd_browan/CMakeLists.txt: RUN_IN_IDT_MODE")
endif()
