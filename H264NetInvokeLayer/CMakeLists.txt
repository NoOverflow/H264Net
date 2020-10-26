cmake_minimum_required(VERSION 3.10)

# set the project name
project(H264NetInvokeLayer)

set(CISCO_INCLUDE_PATH "include/Cisco" "include")
set(SOURCES src/Test.cpp src/Encoder/encoder.cpp include/Cisco/codec_api.h)

include_directories("${PROJECT_SOURCE_DIR}/lib/OpenH264")
add_subdirectory("lib/OpenH264")

# add the executable
add_executable(H264NetInvokeLayer ${SOURCES})

target_include_directories(H264NetInvokeLayer PUBLIC CISCO_INCLUDE_PATH)
set(CISCO_LIBRARY "${PROJECT_SOURCE_DIR}/lib/OpenH264/welsecore.lib" "${PROJECT_SOURCE_DIR}/lib/OpenH264/WelsVP.lib" "${PROJECT_SOURCE_DIR}/lib/OpenH264/welsenc.lib")
target_link_libraries(H264NetInvokeLayer PUBLIC ${CISCO_LIBRARY})