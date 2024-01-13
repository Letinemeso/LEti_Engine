cmake_minimum_required(VERSION 3.5)

project(LEti_Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE LEti_Engine_include "include/*.h")
add_custom_target(_LEti_Engine_include SOURCES ${LEti_Engine_include})

#include_directories("${PROJECT_SOURCE_DIR}/include/")

FILE(GLOB_RECURSE LEti_Engine_source "source/*.cpp")
add_library(LEti_Engine STATIC ${LEti_Engine_source})

target_include_directories(LEti_Engine PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/OpenGL/GLM"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include/"
                            "${PROJECT_SOURCE_DIR}/../L_Variable/include/"
)

