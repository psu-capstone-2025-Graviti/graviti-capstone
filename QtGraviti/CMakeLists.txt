# CMakeList.txt : CMake project for Graviti, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.16)

project(Graviti VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##THIS MUST BE MODIFIED BY EACH USER
##set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/msvc2022_64")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Qml Core Gui QuickWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Qml Core Gui QuickWidgets )

project (GravitiLib)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE GRAVLIB_SRC_FILES
    "src/Simulation/*.cpp"
)

add_library(GravitiLib ${GRAVLIB_SRC_FILES} )


# TODO: Add tests and install targets if needed.




target_include_directories(GravitiLib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>)




file(GLOB_RECURSE APP_SRC_FILES
    "src/App/*.cpp"
    "src/App/*.h"
    "src/App/*.ui"
    "src/App/*.qml"
)

set(PROJECT_SOURCES
        src/main.cpp
        qml.qrc
        ${APP_SRC_FILES}
)

#ADD QT_EXECUTABLE
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Graviti
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GravitiLib PROPERTY CXX_STANDARD 20)
endif()
target_link_libraries(Graviti PRIVATE
    GravitiLib
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickWidgets
)

set_target_properties(Graviti PROPERTIES
    WIN32_EXECUTABLE FALSE #If this is TRUE, the console wont show up
)

#include(GNUInstallDirs)
install(TARGETS Graviti
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Graviti)
endif()



############################
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# CTest sets BUILD_TESTING to on. To disable tests add -DBUILD_TESTING=OFF when invoking cmake
if(BUILD_TESTING)

    # A test is just an executable that is linked against the unit testing library
    add_executable(graviti src/main.cpp "tests/Sources.cpp" "tests/T_Entity.cpp" "tests/T_NBodyEngine.cpp" "tests/T_PhysicalState.cpp")
    
    target_include_directories(my_test_exe
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>)

    # register the test with CTest, telling it what executable to run
    add_test(NAME abc COMMAND my_test_exe)

endif()