cmake_minimum_required(VERSION 3.14)

set(PROJECT costs_n_meters)

project(${PROJECT} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/android)

#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/qml CACHE PATH "")

if(ANDROID)
    add_library(${PROJECT} SHARED
        src/qt/main.cpp
        # src/qt/qml.qrc
        src/qt/rc.qrc
    )

    add_custom_target(copy_qml_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/qml ${ANDROID_PACKAGE_SOURCE_DIR}/assets/qml
    )
    add_dependencies(${PROJECT} copy_qml_assets)

    add_custom_target(copy_jar_libs
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/libs ${ANDROID_PACKAGE_SOURCE_DIR}/libs
    )
    add_dependencies(${PROJECT} copy_jar_libs)

else()
    add_executable(${PROJECT}
        src/qt/main.cpp
        src/qt/qml.qrc
        src/qt/rc.qrc
    )
endif()

target_compile_definitions(${PROJECT}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
