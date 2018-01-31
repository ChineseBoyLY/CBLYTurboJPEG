cmake_minimum_required(VERSION 3.4.1)


set(CMAKE_VERBOSE_MAKEFILE on)
set(libs "${CMAKE_SOURCE_DIR}/src/main/jniLibs")
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)

add_library( turbo STATIC IMPORTED )
set_target_properties( turbo PROPERTIES
                       IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libturbo.a" )

add_library( yuv STATIC IMPORTED )
set_target_properties( yuv PROPERTIES
                       IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libyuv.a" )

add_library( jni_jpeg
             SHARED
             src/main/cpp/libjpegTurbo.cpp )


find_library( log-lib
              log )

target_link_libraries( jni_jpeg
                       turbo
                       yuv

                       ${log-lib} )