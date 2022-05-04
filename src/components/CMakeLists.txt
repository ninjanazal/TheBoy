message("Including ${CMAKE_CURRENT_SOURCE_DIR}")


set( SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/addressbus.cpp
	
	PARENT_SCOPE
)