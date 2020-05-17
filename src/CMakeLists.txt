# Generic test that uses conan libs

add_executable(game
        main.cpp
        Game.cpp
        Shaders/Program.cpp
        Shaders/DefaultProgram.cpp
        Shaders/ShaderLoader.cpp
        Utils/PerlinNoise.cpp
        World/World.cpp
        World/Block.cpp
        World/Chunk.cpp
        Renderer/Mesh.cpp
        Renderer/Vertex.cpp
        Renderer/ChunkRenderer.cpp
        Renderer/ChunkMeshBuilder.cpp
        Renderer/BlockFace.cpp
        Camera.cpp
        Inputs/InputManager.cpp
        Window.cpp
        )

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
)


if (ENABLE_COTIRE)
    cotire(game)
endif ()
