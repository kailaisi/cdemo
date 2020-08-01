# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
#设置cmake的最小支持版本
cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#创建一个库

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)
set(lib_src_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a)
message("CURRENT_DIR:" ${CMAKE_SOURCE_DIR})
set(CMAKE_VERBOSE_MAKEFILE on)
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

add_library( # Sets the name of the library.
        native-lib #库名称
        # Sets the library as a shared library.
        #设置是动态库（SHARED），还是静态库（STATIC）
        SHARED
        # Provides a relative path to your source file(s).
        #设置源文件的相对路径
        ${CMAKE_SOURCE_DIR}/src/main/cpp/native-lib.cpp
        ${CMAKE_SOURCE_DIR}/src/main/cpp/extra.h
        ${CMAKE_SOURCE_DIR}/src/main/cpp/dynamic-lib.cpp)

#搜索并指定预构建库并将路径存储为变量
#CMake已经包含了ndk中的系统类库。所以这里只需要添加你需要添加的其他类库
find_library( # Sets the name of the path variable.
        log-lib#路径变量名
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        #指定添加的NDK类库名称
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

add_library( avutil
        SHARED
        IMPORTED )
set_target_properties( avutil
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libavutil-56.so)
add_library( swresample
        SHARED
        IMPORTED )
set_target_properties( swresample
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libswresample-3.so)
add_library( avcodec
        SHARED
        IMPORTED )
set_target_properties( avcodec
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libavcodec-58.so)
add_library( avfilter
        SHARED
        IMPORTED )
set_target_properties( avfilter
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libavfilter-7.so)
add_library( swscale
        SHARED
        IMPORTED )
set_target_properties( swscale
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libswscale-5.so)
add_library( avdevice
        SHARED
        IMPORTED )
set_target_properties( avdevice
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libavdevice-58.so)
add_library( avformat
        SHARED
        IMPORTED )
set_target_properties( avformat
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libavformat.so)
add_library( postproc
        SHARED
        IMPORTED )
set_target_properties( postproc
        PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libpostproc.so)

#指定CMake应链接到目标库的库。这里可以设置多个
target_link_libraries( # Specifies the target library.
        native-lib
        avutil #工具库(大部分需要)
        swresample#音频采样数据格式转换
        avcodec#编解码(重要)
        avfilter      #滤镜特效处理
        swscale       #视频像素数据格式转换
        avdevice     #各种设备的输入输出
        avformat     #封装格式处理
        postproc     #后加工
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
