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
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(ANDROID_PROJECT_SOURCES
    src/qt/main.cpp
    src/qt/rc.qrc
    src/qt/qtquickcontrols2.conf
)

set(PROJECT_SOURCES
    ${ANDROID_PROJECT_SOURCES}
    src/qt/qml.qrc
)

set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/qml CACHE PATH "")


if(ANDROID)

    set(ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/android CACHE INTERNAL "")

    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${PROJECT}
            ${ANDROID_PROJECT_SOURCES}
        )
    else()
        add_library(${PROJECT} SHARED
            ${ANDROID_PROJECT_SOURCES}
        )
    endif()

    add_custom_target(copy_qml_assets
        COMMAND echo "@@ Copying qml assets to android template"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/qml ${ANDROID_PACKAGE_SOURCE_DIR}/assets/qml
    )
    add_dependencies(${PROJECT} copy_qml_assets)

    add_custom_target(copy_jar_libs
        COMMAND echo "@@ Copying libraries to android template"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/libs ${ANDROID_PACKAGE_SOURCE_DIR}/libs
    )
    add_dependencies(${PROJECT} copy_jar_libs)

    # ============================================================================
    # workaround:
    # Qt6 androiddeployqt somehow ignores ANDROID_PACKAGE_SOURCE_DIR variable,
    # so we replace files explicitly
    # ============================================================================
    # if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    #     set(DIR_ANDROID_BUILD ${CMAKE_BINARY_DIR}/android-build)

    #     add_custom_target(copy_android_template
    #         COMMAND echo "@@ Copying android template to android-build"
    #         COMMAND cp -a ${ANDROID_PACKAGE_SOURCE_DIR}/libs ${DIR_ANDROID_BUILD}
    #         COMMAND cp -a ${ANDROID_PACKAGE_SOURCE_DIR}/assets ${DIR_ANDROID_BUILD}
    #         COMMAND cp -a ${ANDROID_PACKAGE_SOURCE_DIR}/res ${DIR_ANDROID_BUILD}
    #     )
    #     add_dependencies(copy_android_template copy_qml_assets copy_jar_libs)
    #     add_dependencies(${PROJECT} copy_android_template)
    # endif()
    # ============================================================================

else()

    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${PROJECT}
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${PROJECT}
            ${PROJECT_SOURCES}
        )
    endif()

endif()


target_compile_definitions(${PROJECT}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
if (ANDROID)
    include(/home/user/Android/Sdk/android_openssl/CMakeLists.txt)
endif()
