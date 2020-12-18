cmake_minimum_required(VERSION 3.10)

# set the project name
project(H264NetInvokeLayer)

set(CISCO_INCLUDE_PATH "include/Cisco" "include")
set(TEST_SOURCE src/Test.cpp)
set(SOURCES src/Encoder/encoder.cpp include/Cisco/codec_api.h)

include_directories("${PROJECT_SOURCE_DIR}/lib/OpenH264")
add_library(H264NetInvokeLayer SHARED ${SOURCES})

if( ANDROID )
	set (CISCO_LIBRARY "${PROJECT_SOURCE_DIR}/lib/OpenH264/arm64/openh264.so")
	add_library(lib_openh264 SHARED IMPORTED)
	set_target_properties(lib_openh264 PROPERTIES IMPORTED_LOCATION ${CISCO_LIBRARY})
	target_link_libraries(H264NetInvokeLayer lib_openh264)
elseif( WIN32 )	
	set (CISCO_LIBRARY "${PROJECT_SOURCE_DIR}/lib/OpenH264/openh264.lib")
	target_link_libraries(H264NetInvokeLayer PUBLIC ${CISCO_LIBRARY})
endif()

target_include_directories(H264NetInvokeLayer PUBLIC CISCO_INCLUDE_PATH)