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

set(path_so ../jniLibs/armeabi-v7a)
include_directories(include)
add_library( # Sets the name of the library.
        native-lib #库名称

        # Sets the library as a shared library.
        #设置是动态库（SHARED），还是静态库（STATIC）
        SHARED

        # Provides a relative path to your source file(s).
        #设置源文件的相对路径
        native-lib.cpp
        extra.h
        dynamic-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
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
#指定CMake应链接到目标库的库。这里可以设置多个
target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

add_library(avcodec-58
        SHARED
        IMPORTED)
set_target_properties(avcodec-58
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libavcodec-58.so)

add_library(avdevice-58
        SHARED
        IMPORTED)
set_target_properties(avdevice-58
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libavdevice-58.so)

add_library(avfilter-7
        SHARED
        IMPORTED)
set_target_properties(avfilter-7
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libavfilter-7.so)

add_library(avformat-58
        SHARED
        IMPORTED)
set_target_properties(avformat-58
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libavformat-58.so)

add_library(avutil-56
        SHARED
        IMPORTED)
set_target_properties(avutil-56
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libavutil-56.so)

add_library(postproc-55
        SHARED
        IMPORTED)
set_target_properties(postproc-55
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libpostproc-55.so)

add_library(swresample-3
        SHARED
        IMPORTED)
set_target_properties(swresample-3
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libswresample-3.so)

add_library(swscale-5
        SHARED
        IMPORTED)
set_target_properties(swscale-5
        PROPERTIES
        IMPORTED_LOCATION
        ${path_so}/libswscale-5.so)


target_link_libraries( # Specifies the target library.
        native-lib

        avcodec-58
        avdevice-58
        avfilter-7
        avformat-58
        avutil-56
        postproc-55
        swresample-3
        swscale-5
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})