message("Including ${CMAKE_CURRENT_SOURCE_DIR}")


set ( SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/emulatorController.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/emulView.cpp

	PARENT_SCOPE
)

set ( HEADERS 
	${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/emulatorController.h
	${CMAKE_CURRENT_SOURCE_DIR}/emulView.h

	PARENT_SCOPE
)