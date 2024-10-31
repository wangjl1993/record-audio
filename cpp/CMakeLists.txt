project(record_voice)
cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_BUILD_TYPE Debug)

# from /home/ai_team/jielong/test_S539/android-ndk-r19c/platforms/android-23/arch-arm/usr/lib/libOpenSLES.so
set(OPENSL_ES_LIB ${CMAKE_CURRENT_SOURCE_DIR}/thirdlib/opensles/libOpenSLES.so) 


set(LAME_LIB ${CMAKE_CURRENT_SOURCE_DIR}/thirdlib/lame/${CMAKE_SYSTEM_NAME}/${CMAKE_ANDROID_ARCH_ABI}/liblame.so)
message("OPENSL_ES_LIB: ${OPENSL_ES_LIB}")
message("LAME_LIB: ${LAME_LIB}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdlib/lame/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdlib/opensles/include)
file(GLOB SRC_LIST  ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
add_executable(record_demo ${SRC_LIST})

target_link_libraries(record_demo ${OPENSL_ES_LIB})
target_link_libraries(record_demo ${LAME_LIB})