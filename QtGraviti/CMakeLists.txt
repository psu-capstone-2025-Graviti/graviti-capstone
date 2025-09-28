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
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Qml
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


################################
# GTest
################################
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
enable_testing()

#Prile flags - needed for CTest code coverage?
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /PROFILE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /PROFILE")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /PROFILE")

# Code coverage flags for msvc
add_compile_options(/Zi /Od)
add_link_options(/PROFILE)

file(GLOB_RECURSE TEST_SRC_FILES
    "tests/*.cpp"
)

add_executable(GravitiTests ${TEST_SRC_FILES})
    
target_include_directories(GravitiTests
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
$<INSTALL_INTERFACE:include/>)

#Link libraries for testing
target_link_libraries(GravitiTests
    PRIVATE
    GravitiLib
    gtest
    gtest_main
)


# Set properties for code coverage on test executable
set_target_properties(GravitiTests PROPERTIES
    COMPILE_FLAGS "/Zi /Od"
    LINK_FLAGS "/PROFILE"
)

# Ensure the library is also instrumented for coverage
set_target_properties(GravitiLib PROPERTIES
    COMPILE_FLAGS "/Zi /Od"
    LINK_FLAGS "/PROFILE"
)


# Create test results directory in build directory
#file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/test-results")

gtest_discover_tests(GravitiTests
    XML_OUTPUT_DIR "${CMAKE_BINARY_DIR}/test-results"
    DISCOVERY_TIMEOUT 60
    PROPERTIES
    ENVIRONMENT "GTEST_OUTPUT=xml:${CMAKE_BINARY_DIR}/test-results/"
)

# Set CTest properties for XML output
set_property(DIRECTORY PROPERTY CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS 1000)
set_property(DIRECTORY PROPERTY CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS 1000)


set_target_properties(GravitiTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

set_target_properties(GravitiTests PROPERTIES
    COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    COMPILE_PDB_NAME "GravitiTests"
    COMPILE_PDB_NAME_DEBUG "GravitiTests_debug"
    COMPILE_PDB_NAME_RELEASE "GravitiTests_release"
)

set_target_properties(GravitiLib PROPERTIES
    COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    COMPILE_PDB_NAME "GravitiLib"
    COMPILE_PDB_NAME_DEBUG "GravitiLib_debug"
    COMPILE_PDB_NAME_RELEASE "GravitiLib_release"
)
