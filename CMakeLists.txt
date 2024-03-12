cmake_minimum_required(VERSION 3.25)
project(OpenGLTest VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)


include_directories(${GLEW_INCLUDE_DIRS})
include_directories(E:\\Coding\\CppProjects\\CommonLibs\\glm,
                    E:\\Coding\\CppProjects\\CommonLibs\\glfw\\include,
                    E:\\Coding\\msys64\\mingw64\\include\\assimp)
add_executable(OpenGLTest main.cpp)
target_link_libraries(OpenGLTest PRIVATE ${GLEW_LIBRARIES} ${ASSIMP_LIBRARIES})