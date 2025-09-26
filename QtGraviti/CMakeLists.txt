# CMakeList.txt : CMake project for Graviti, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)



project (GravitiLib)



add_library(GravitiLib "src/BatchSimEnvironment.cpp" "src/Entity.cpp" "src/EntityManager.cpp" "src/NBodyEngine.cpp" "src/PhysicalState.cpp" )


# TODO: Add tests and install targets if needed.



include_directories(${CMAKE_SOURCE_DIR}/include)

target_include_directories(GravitiLib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>)

add_executable(graviti src/main.cpp)
# Add source to this project's executable.

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GravitiLib PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(graviti GravitiLib)




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

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable(runUnitTests tests/T_Entity.cpp tests/T_NBodyEngine.cpp tests/T_PhysicalState.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main GravitiLib)
add_test( runUnitTests runUnitTests )