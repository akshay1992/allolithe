file(GLOB SRCS *.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/allolithe_tests)

ADD_EXECUTABLE(testTemplate ${SRCS})

TARGET_LINK_LIBRARIES(testTemplate
	allolithe
	libgtest
	libgmock
)

add_test(NAME testTemplate 
		COMMAND testTemplate)

add_dependencies(test_allolithe testTemplate)