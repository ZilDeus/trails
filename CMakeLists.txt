cmake_minimum_required(VERSION 3.26)
project(trails)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
#add_compile_options(-Wall -Wextra -Wpedantic)

if (UNIX)
  set(LINK_LIBRARIES "GL" "X11" "glfw3" "m")
  set(BIN_PATH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
endif (UNIX)


if (WIN32 AND MINGW)
  set(LINK_LIBRARIES "opengl32" "user32" "glfw3")
  set(BIN_PATH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
endif (WIN32 AND MINGW)


add_custom_target(run_heatMap
  COMMAND ./heatMap
  DEPENDS "${CMAKE_BINARY_DIR}/heatMap"
  COMMENT "running heatMap"
  )

add_custom_target(run_trails
  COMMAND ./trails
  DEPENDS "${CMAKE_BINARY_DIR}/trails"
  COMMENT "running trails"
  )

set(MyCSources ./src/glad.c ./src/array.c ./src/shader.c ./src/texture.c ./src/framebuffer.c ./src/vertexarray.c)
add_executable(${PROJECT_NAME} ./src/trails.c ${MyCSources})
add_executable(heatMap ./src/map.c ${MyCSources})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})
target_link_libraries(heatMap ${LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ./include/)
target_include_directories(heatMap PRIVATE ./include/)
