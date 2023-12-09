cmake_minimum_required(VERSION 3.5)

project(LEti_Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(LEti_Engine_include SOURCES ${LibFiles})

#include_directories("${PROJECT_SOURCE_DIR}/include/")

add_library(LEti_Engine STATIC
    source/FPS_Timer.cpp
    source/Math_Stuff.cpp
    source/Module.cpp
    source/Object_System/Object.cpp
    source/Object_System/Object_Base.cpp
    source/Transformation_Data.cpp
)

target_include_directories(LEti_Engine PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/OpenGL/GLM"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include/"
                            "${PROJECT_SOURCE_DIR}/../L_Variable/include/"
)

