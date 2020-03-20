# Hello World demo

afr_demo_module(hello_world)

afr_set_demo_metadata(ID "HELLO_WORLD_DEMO")
afr_set_demo_metadata(DESCRIPTION "An example that demonstrates Hello World")
afr_set_demo_metadata(DISPLAY_NAME "Hello World")

afr_module_sources(
	${AFR_CURRENT_MODULE}
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/hello_world.c"
)

#afr_module_dependencies(
#	${AFR_CURRENT_MODULE}
#	INTERFACE
#		AFR::mqtt
#)
