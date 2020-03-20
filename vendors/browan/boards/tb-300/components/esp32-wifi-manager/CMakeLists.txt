#if(IDF_VERSION_MAJOR GREATER_EQUAL 4)
#    idf_component_register(SRC_DIRS src
#        REQUIRES log nvs_flash mdns wpa_supplicant lwip esp_http_server
#        INCLUDE_DIRS src
#        EMBED_FILES src/style.css src/jquery.gz src/code.js src/index.html)
#else()
#    set(COMPONENT_SRCDIRS src)
#    set(COMPONENT_ADD_INCLUDEDIRS src)
#    set(COMPONENT_REQUIRES log nvs_flash mdns wpa_supplicant lwip esp_http_server)
#    set(COMPONENT_EMBED_FILES src/style.css src/jquery.gz src/code.js src/index.html)
#    register_component()
#endif()

set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_REQUIRES log nvs_flash mdns wpa_supplicant lwip esp_http_server cmd_browan)
set(COMPONENT_EMBED_FILES src/style.css src/jquery.gz src/code.js src/index.html)

if(NOT RUN_IN_IDT_MODE)
    set(COMPONENT_SRCDIRS src)
    register_component()
else()
    message("INFO >> vendors/browan/boards/tb-300/components/esp32-wifi-manager/CMakeLists.txt: RUN_IN_IDT_MODE")
endif()
