cmake_minimum_required(VERSION 3.10)
project(simuGBS)
project (simuGBS VERSION 1.0)

#set(Eigen3_DIR CMAKE_INSTALL_PREFIX/share/eigen3/cmake)
#LIST(APPEND CMAKE_MODULE_PATH "/usr/include/")
find_package (Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)




set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

configure_file(src/simuGBS_Config.h.in ../src/simuGBS_Config.h)


add_subdirectory(src)
list(APPEND EXTRA_LIBS src)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src")

#file(GLOB SOURCES "src/*")

include_directories(${PROJECT_SOURCE_DIR}/include/)

#add_library(${PROJECT_NAME} SHARED src/simuGBS.cpp)



#add_executable(simuGBS ${SOURCES})
#add_executable(simuGBS src/simuGBS.cpp)

#add_executable(simuGBS src/simuGBS.cpp)
#target_include_directories(simuGBS PUBLIC
#			   "${PROJECT_BINARY_DIR}"
#			   )
#target_link_libraries (simuGBS Eigen3::Eigen)
