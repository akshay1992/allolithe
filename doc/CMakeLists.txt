find_package(Doxygen)
if(DOXYGEN_FOUND)
add_custom_target(allolithe_docs
	${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/docs/DoxyFile
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/docs
)

add_dependencies(doc allolithe_docs)
endif(DOXYGEN_FOUND)