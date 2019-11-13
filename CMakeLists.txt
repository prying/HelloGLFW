cmake_minimum_required(VERSION 3.15)

project(HelloGLFW)

include_directories(include)
set(SOURCE_FILES 
	src/main.cpp
	src/tools.cpp
	src/ShaderProgram.cpp)

# Add config stuff
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

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

# Salamandar
# Copy single files
macro(resource_files files)
    foreach(file ${files})
        message(STATUS "Copying resource ${file}")
        file(COPY ${file} DESTINATION ${Work_Directory})
    endforeach()
endmacro()

# Copy full directories
macro(resource_dirs dirs)
    foreach(dir ${dirs})
        # Replace / at the end of the path (copy dir content VS copy dir)
        string(REGEX REPLACE "/+$" "" dirclean "${dir}")
        message(STATUS "Copying resource ${dirclean}")
        file(COPY ${dirclean} DESTINATION ${Work_Directory})
    endforeach()
endmacro()