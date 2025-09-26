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