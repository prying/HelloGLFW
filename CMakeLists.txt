cmake_minimum_required(VERSION 3.15)

project(HelloGLFW)

include_directories(include)
set(SOURCE_FILES 
	src/main.cpp
	src/tools.cpp
	src/ShaderProgram.cpp)

# GLFW
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw-3.3)
include_directories(lib/glfw-3.3)
include_directories(lib/glfw-3.3/deps)
include_directories(lib/glfw-3.3/include)

set(GLAD "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
         "${GLFW_SOURCE_DIR}/deps/glad_gl.c")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

add_executable(HelloGLFW ${SOURCE_FILES} ${TINYCTHREAD} ${GETOPT} ${GLAD})

target_link_libraries(HelloGLFW glfw ${GLFW_LIBRARIES})

# Add shaders dir to target
add_custom_command(TARGET HelloGLFW POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:HelloGLFW>)