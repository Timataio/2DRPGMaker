cmake_minimum_required (VERSION 3.8)

project(2DRPGMaker)

include_directories( ${OPEN_GL_INCLUDE_DIRS} extern/glad/include extern/glfw-3.3.8/include extern/stb_image/include)
add_subdirectory(extern/glfw)
add_executable(main src/MainLoop.cpp src/Graphics/WindowManager.cpp extern/glad/src/glad.c)

target_link_libraries(main glfw)

