# - - - - - - - - - -
# List source files
# - - - - - - - - - -
set( SOURCE
	main.cpp
	EmulatorController/emulatorController.cpp
)


# - - - - - - - - - -
# Create executable with the defined source
# - - - - - - - - - -
add_executable (${PROJECT_NAME} ${SOURCE})


# - - - - - - - - - -
# Link/Include
# - - - - - - - - - -
# SFML
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system sfml-window sfml-graphics sfml-audio)

# Headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)