cmake_minimum_required(VERSION 3.14)
project(OpenGL)

set(CMAKE_CXX_STANDARD 14)

include_directories(OpenGL)
include_directories(OpenGL/include)
include_directories(OpenGL/lib)

add_custom_target(glutdlllib
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/OpenGL/dll/glut32.dll ${CMAKE_BINARY_DIR}
        )


set(OpenGlLibs glaux glu32 glui32 glut32 opengl32)

add_executable(OpenGL main.cpp EventHandler.cpp)

add_executable(Sphere Sphere.cpp)

add_executable(Wheel Wheel.cpp)

add_executable(B1 "Old Onlines/A1_Online.cpp")

add_executable(A2 "Old Onlines/A2_Online.cpp")

add_executable(B2 "Old Onlines/B1_Online.cpp")

add_executable(Online Online.cpp)

add_executable(ST Sierpinski.cpp)

target_link_libraries(OpenGL ${OpenGlLibs})
target_link_libraries(Sphere ${OpenGlLibs})
target_link_libraries(Wheel ${OpenGlLibs})
target_link_libraries(B1 ${OpenGlLibs})
target_link_libraries(A2 ${OpenGlLibs})
target_link_libraries(B2 ${OpenGlLibs})
target_link_libraries(Online ${OpenGlLibs})
target_link_libraries(ST ${OpenGlLibs})


add_dependencies(OpenGL glutdlllib)
add_dependencies(Sphere glutdlllib)
add_dependencies(Wheel glutdlllib)
add_dependencies(B1 glutdlllib)
add_dependencies(A2 glutdlllib)
add_dependencies(B2 glutdlllib)
add_dependencies(Online glutdlllib)
add_dependencies(ST glutdlllib)

#add_executable(WheelOthers WheelOthers/ahsantariq.cpp)
#add_executable(SphereOthers SphereOthers/1305079.cpp)
#target_link_libraries(WheelOthers ${OpenGlLibs})
#target_link_libraries(SphereOthers ${OpenGlLibs})
