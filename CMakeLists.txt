cmake_minimum_required(VERSION 3.31.5)

#Setup cxx std
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
project(InariKonKon VERSION 0.1.0 LANGUAGES C CXX)

#Incldue fmt
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG master)
FetchContent_MakeAvailable(fmt)

#Include GLFW
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

#Setup glm
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG master)
FetchContent_MakeAvailable(glm)

#OpenGL
#FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG 658f48e72aee3c6582e80b05ac0f8787a64fe6bb GIT_PROGRESS TRUE SOURCE_SUBDIR  cmake)
#FetchContent_MakeAvailable(glad)
#glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE LOADER API gl:core=4.3)

#setup basic directories
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

#Setup source controll
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCES_DIR}/*.cpp")

#Create the library
add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCES})

#Setup vulkan
find_package(Vulkan REQUIRED shaderc_combined)

#Set include dir
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

#Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt::fmt glfw glm::glm Vulkan::Vulkan Vulkan::shaderc_combined) #glad_gl_core_43)

#Setup TestApp
add_executable(TestApp TestApp.cpp)
target_link_libraries(TestApp InariKonKon)