cmake_minimum_required(VERSION 3.9.0)

project(Read_Cosika VERSION 1.0)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/compiled")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_INSTALL_PREFIX}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1 CACHE BOOL "for clang" FORCE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS " -g -O2")
#set(CMAKE_CXX_FLAGS_RELEASE "-o2")

set(HESS "/data/home/zhipz/hessioxxx/lib/libhessio.so")
find_package(ROOT 6.24 CONFIG REQUIRED COMPONENTS Minuit)
include("${ROOT_USE_FILE}")
root_generate_dictionary(Class ${PROJECT_SOURCE_DIR}/include/Photon_bunches.h  ${PROJECT_SOURCE_DIR}/include/events.h 
                        LINKDEF ${PROJECT_SOURCE_DIR}/include/LinkDef.h)


add_library(class SHARED) 
target_sources(class PRIVATE ${PROJECT_SOURCE_DIR}/src/Photon_bunches.cpp ${PROJECT_SOURCE_DIR}/src/Tel_groups.cpp ${PROJECT_SOURCE_DIR}/src/rec_tools.c Class.cxx)
target_include_directories(class PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(class PRIVATE ${ROOT_LIBRARIES} )

add_executable(Read_Corsika)
target_sources(Read_Corsika PUBLIC ${PROJECT_SOURCE_DIR}/src/get_photons.cpp )
target_include_directories(Read_Corsika PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Read_Corsika PRIVATE class ${HESS} ${ROOT_LIBRARIES})

add_executable(Draw)
target_sources(Draw PUBLIC ${PROJECT_SOURCE_DIR}/src/Draw.cpp)
target_include_directories(Draw PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Draw PRIVATE class ${ROOT_LIBRARIES})

