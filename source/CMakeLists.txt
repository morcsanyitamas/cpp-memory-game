set(SOURCES
        main.cpp
        UI/UI.cpp
        UI/Texture.cpp
        Game/Game.cpp
)

add_executable(memory-game ${SOURCES})

# Find SDL2 and SDL2_image
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)


# Link SDL2 and SDL2_image libraries
target_link_libraries(memory-game
        PRIVATE
        SDL2::SDL2main   # SDL2 main function library
        SDL2::SDL2       # SDL2 library
        SDL2_image::SDL2_image  # SDL2_image library for image handling
)