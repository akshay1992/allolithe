file(GLOB SRCS *.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/allolithe_tests)

ADD_EXECUTABLE(testAtlas ${SRCS})

TARGET_LINK_LIBRARIES(testAtlas
	allolithe
	libgtest
	libgmock
)

add_test(NAME testAtlas 
		COMMAND testAtlas)

add_dependencies(test_allolithe testAtlas)
