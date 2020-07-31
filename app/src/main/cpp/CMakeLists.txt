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
add_library( # Sets the name of the library.
        native-lib #库名称

        # Sets the library as a shared library.
        #设置是动态库（SHARED），还是静态库（STATIC）
        SHARED

        # Provides a relative path to your source file(s).
        #设置源文件的相对路径
        extra.h
        global.h
        JavaListener.h
        native-lib.cpp
        thread.cpp
        dynamic-lib.cpp
        JavaListener.cpp)

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