cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_CXX_FLAGS "/utf-8 /EHsc")

project(InariKonKon VERSION 0.1.0 LANGUAGES CXX)

set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCES_DIR}/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR}/glfw/include/)
add_subdirectory(${EXTERNAL_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR}/vulkan/include/)
target_link_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR}/vulkan/lib/)
target_link_libraries(${PROJECT_NAME} vulkan-1)
target_link_libraries(${PROJECT_NAME} shaderc_shared)

add_executable(TestApp TestApp.cpp)
target_link_libraries(TestApp InariKonKon)