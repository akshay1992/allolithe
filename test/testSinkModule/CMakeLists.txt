file(GLOB SRCS *.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/allolithe_tests)

ADD_EXECUTABLE(testSinkModule ${SRCS})

TARGET_LINK_LIBRARIES(testSinkModule
	allolithe
	libgtest
	libgmock
)

add_test(NAME testSinkModule 
		COMMAND testSinkModule)

add_dependencies(test_allolithe testSinkModule)
