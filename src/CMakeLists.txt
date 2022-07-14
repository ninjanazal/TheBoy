# - - - - - - - - - -
# List source files
# - - - - - - - - - -
set( SOURCE
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set ( HEADERS )

# - - - - - - - - - -
# Precompilation Defined Macros
add_compile_definitions(VERBOSE=${VERBOSE})
add_compile_definitions(IOOUT=true)

add_compile_definitions(SFML_STATIC TRUE)


# - - - - - - - - - -
# Set Include directories path
# - - - - - - - - - -
set ( INC_DIRECTORIES
	Common/
	Components/
	EmulatorController/
)


# - - - - - - - - - -
# Add sub folders
# - - - - - - - - - -
add_subdirectory(Common)
add_subdirectory(Components)
add_subdirectory(EmulatorController)


# message(${SOURCE})
# - - - - - - - - - -
# Create executable with the defined source
# - - - - - - - - - -
add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCE})


set(SFML_DIR ${CMAKE_SOURCE_DIR}/Vendor/SFML/${TARGETCONFIG}/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics system window audio REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system sfml-window sfml-graphics sfml-audio)


if ( ${TARGETCONFIG} STREQUAL "vs" )
	message("[ SETUP FOR VS TARGET ]")
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
elseif( ${TARGETCONFIG} STREQUAL "mingw" )
	message("[ SETUP FOR MINGW TARGET ]")
endif()

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/src)




# - - - - - - - - - -
# Link/Include
# - - - - - - - - - -
# - - - - - - - - - -
# Directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRECTORIES})


