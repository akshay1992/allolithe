file(GLOB SRCS *.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/allolithe_tests)

ADD_EXECUTABLE(testModule ${SRCS})

TARGET_LINK_LIBRARIES(testModule
	allolithe
	libgtest
	libgmock
)

add_test(NAME testModule 
		COMMAND testModule)

add_dependencies(test_allolithe testModule)
