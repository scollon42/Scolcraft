# Generic test that uses conan libs

add_executable(game
        main.cpp
        Shaders/Program.cpp
        Shaders/DefaultProgram.cpp
        Shaders/ShaderLoader.cpp
        Utils/PerlinNoise.cpp
        World/World.cpp World/World.h World/Block.cpp World/Block.h World/Chunk.cpp World/Chunk.h Renderer/Mesh.cpp Renderer/Mesh.h Renderer/Vertex.cpp Renderer/Vertex.h Renderer/ChunkRenderer.cpp Renderer/ChunkRenderer.h Renderer/ChunkMeshBuilder.cpp Renderer/ChunkMeshBuilder.h)

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
        #        CONAN_PKG::glad
        #        CONAN_PKG::sfml
        CONAN_PKG::glfw
        CONAN_PKG::glm
)


if (ENABLE_COTIRE)
    cotire(game)
endif ()
