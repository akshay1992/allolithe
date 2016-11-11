file(GLOB_RECURSE SIMPLE_OSCILLATOR_APP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable( simpleOscillatorApp ${SIMPLE_OSCILLATOR_APP})

target_include_directories( simpleOscillatorApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include  )
target_link_libraries(simpleOscillatorApp portaudio Gamma allolithe)