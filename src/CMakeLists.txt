# - - - - - - - - - -
# List source files
# - - - - - - - - - -
set( SOURCE
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

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
add_executable (${PROJECT_NAME} ${SOURCE})


# - - - - - - - - - -
# Link/Include
# - - - - - - - - - -
# SFML
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system sfml-window sfml-graphics sfml-audio)

# - - - - - - - - - -
# Directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRECTORIES})