# Generic test that uses conan libs

add_executable(game
        main.cpp
        Game/Game.cpp
        Game/States/MainState.cpp
        Utils/PerlinNoise.cpp
        World/Chunks/Chunk.cpp
        World/Chunks/ChunkBlock.cpp
        World/Blocks/BlockType.cpp
        World/Blocks/Block.cpp
        World/Blocks/BlockTextureCoordinates.cpp
        World/World.cpp
        Renderer/Mesh.cpp
        Renderer/Vertex.cpp
        Renderer/ChunkRenderer.cpp
        Renderer/ChunkMeshBuilder.cpp
        Renderer/BlockFace.cpp
        Camera.cpp
        Inputs/InputManager.cpp
        Window.cpp
        Shaders/Shader.cpp
        Shaders/ShaderBuilder.cpp
        Shaders/DefaultShader.cpp
        Events/KeyboardEvent.cpp
        Events/MousePositionEvent.cpp
        Utils/Profiler.cpp
        Texture/Atlas.cpp
        Texture/Texture.cpp Texture/loader.cpp Texture/loader.h)

target_include_directories(game PRIVATE .)

target_link_libraries(
        game PRIVATE
        project_options
        project_warnings
        pthread
        CONAN_PKG::docopt.cpp
        CONAN_PKG::fmt
        CONAN_PKG::spdlog
        CONAN_PKG::stb
        CONAN_PKG::glew
        CONAN_PKG::glfw
        CONAN_PKG::glm
        CONAN_PKG::nlohmann_json
)


if (ENABLE_COTIRE)
    cotire(game)
endif ()
