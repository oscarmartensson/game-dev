cmake_minimum_required(VERSION 3.10.0)

set(PROJECT_NAME "tearsplash")

project(${PROJECT_NAME})

# Tell to build shared library
option(BUILD_SHARED_LIBS "Build shared library" OFF)

# Set directory for source files.
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Specify directory for header files.
set(INLCUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/includes)

# Specify directory to look for x86 libraries.
set(LIB_DIR_X86 ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libs/x86)

# Set source files.
set(SOURCES
    ${SOURCE_DIR}/AudioEngine.cpp
    ${SOURCE_DIR}/Camera2D.cpp
    ${SOURCE_DIR}/Errors.cpp
    ${SOURCE_DIR}/ImageLoader.cpp
    ${SOURCE_DIR}/InputManager.cpp
    ${SOURCE_DIR}/IOManager.cpp
    ${SOURCE_DIR}/PhysicsObject.cpp
    ${SOURCE_DIR}/ImageLoader.cpp
    ${SOURCE_DIR}/PicoPNG.cpp
    ${SOURCE_DIR}/ResourceManager.cpp
    ${SOURCE_DIR}/ShaderProgram.cpp
    ${SOURCE_DIR}/Sprite.cpp
    ${SOURCE_DIR}/Spritebatch.cpp
    ${SOURCE_DIR}/Tearsplash.cpp
    ${SOURCE_DIR}/TextureCache.cpp
    ${SOURCE_DIR}/Timing.cpp
    ${SOURCE_DIR}/Window.cpp)

# Set header files.
set(HEADERS
    ${INLCUDE_DIR}/TearSplash/AudioEngine.h
    ${INLCUDE_DIR}/TearSplash/Camera2D.h
    ${INLCUDE_DIR}/TearSplash/Errors.h
    ${INLCUDE_DIR}/TearSplash/GLTexture.h
    ${INLCUDE_DIR}/TearSplash/ImageLoader.h
    ${INLCUDE_DIR}/TearSplash/InputManager.h
    ${INLCUDE_DIR}/TearSplash/IOManager.h
    ${INLCUDE_DIR}/TearSplash/PicoPNG.h
    ${INLCUDE_DIR}/TearSplash/ResourceManager.h
    ${INLCUDE_DIR}/TearSplash/ShaderProgram.h
    ${INLCUDE_DIR}/TearSplash/Sprite.h
    ${INLCUDE_DIR}/TearSplash/SpriteBatch.h
    ${INLCUDE_DIR}/TearSplash/Tearsplash.h
    ${INLCUDE_DIR}/TearSplash/TextureCache.h
    ${INLCUDE_DIR}/TearSplash/Timing.h
    ${INLCUDE_DIR}/TearSplash/Vertex.h
    ${INLCUDE_DIR}/TearSplash/Window.h)

# Tell linker to look for libraries here.
link_directories(${LIB_DIR_X86})

set(BUILD_TYPE STATIC)
if(${BUILD_SHARED_LIBS})
    message("Building shared libraries")
    set(BUILD_TYPE SHARED)
endif()

add_library(${PROJECT_NAME} ${BUILD_TYPE} ${SOURCES} ${HEADERS})

# Tell target to look for header files here.
target_include_directories(${PROJECT_NAME} PUBLIC ${INLCUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC STATIC opengl32 glew32 SDL2 SDL2_mixer SDL2_ttf SLD2main)