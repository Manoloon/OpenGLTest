cmake_minimum_required(VERSION 3.25)
project(OpenGLTest VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(Externals)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)
endif()

# set(GLM_DIR "E:\\Coding\\CppProjects\\CommonLibs\\glm")
# set(GLFW_DIR "E:\\Coding\\CppProjects\\CommonLibs\\glfw\\include")
# set(ASSIMP_DIR "E:\\Coding\\msys64\\mingw64\\include\\assimp")

find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

# include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/Externals)
# include_directories(${IMGUI_DIR})
# include_directories(${GLM_DIR})
# include_directories(${GLFW_DIR})
# include_directories(${ASSIMP_DIR})

add_executable(OpenGLTest main.cpp) #${IMGUI_SOURCES}
# target_include_directories(OpenGLTest PRIVATE ${IMGUI_DIR})
target_link_libraries(OpenGLTest PUBLIC glfw)
target_link_libraries(OpenGLTest PRIVATE GLEW::GLEW assimp::assimp ImGui)