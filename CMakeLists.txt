cmake_minimum_required(VERSION 3.12)
project(Thermal VERSION 2020.3.0 LANGUAGES CXX C)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/image.cpp
    src/SimplexNoise.cpp
    src/scene3d.cpp
    src/obj_loader.cpp
    src/transform.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(LIBS_DIR "packages")
target_include_directories(${PROJECT_NAME} PRIVATE "${LIBS_DIR}/include")

set(GLFW_DIR "${LIBS_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

set(GLAD_DIR "${LIBS_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

set(MATH_DIR "${LIBS_DIR}/apmath")
add_subdirectory("${MATH_DIR}")
target_link_libraries(${PROJECT_NAME} apmath)

set(FUNC_DIR "${LIBS_DIR}/apfun")
add_subdirectory("${FUNC_DIR}")
target_link_libraries(${PROJECT_NAME} apfun)

# set(GLUE_DIR "$")
target_include_directories(${PROJECT_NAME} PRIVATE "include")

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
