set( SOURCE ${SOURCE}
	main.cpp
)

add_executable (${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system sfml-window sfml-graphics sfml-audio)