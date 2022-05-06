message("Including ${CMAKE_CURRENT_SOURCE_DIR}")

set ( SOURCE 
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/instruction.cpp
	PARENT_SCOPE
)