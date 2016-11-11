file(GLOB_RECURSE LITHE_MODULAR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(litheModularApp ${LITHE_MODULAR_SOURCES})

target_include_directories(litheModularApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include  )
target_link_libraries(litheModularApp portaudio Gamma allolithe)