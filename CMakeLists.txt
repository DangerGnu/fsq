cmake_minimum_required(VERSION 3.0)

project(fsq)

set(src 
	src/main.cpp
	src/Game.cpp
	src/Application.cpp
	src/graphics/Shader.cpp
	src/graphics/Batch.cpp
	src/graphics/Texture.cpp
	)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME}
	opengl32
	sdl2/x64/SDL2
	sdl2/x64/SDL2main
	glew2/x64/glew32s
	sdl2/x64/SDL2test
	sdl2_image/x64/SDL2_image)

