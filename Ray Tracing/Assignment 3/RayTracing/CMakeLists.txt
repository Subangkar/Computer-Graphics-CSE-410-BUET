cmake_minimum_required(VERSION 3.14)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing Utils.hpp main.cpp )

include_directories(OpenGL)
include_directories(OpenGL/include)
include_directories(OpenGL/lib)

add_custom_target(glutdlllib
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/OpenGL/dll/glut32.dll ${CMAKE_BINARY_DIR}
        )


set(OpenGlLibs glaux glu32 glui32 glut32 opengl32)

target_link_libraries(RayTracing ${OpenGlLibs})

add_dependencies(RayTracing glutdlllib)


#add_executable(Test 1105001.cpp)
#
#target_link_libraries(Test ${OpenGlLibs})
#
#add_dependencies(Test glutdlllib)
