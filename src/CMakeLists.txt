find_package (PkgConfig)

set (FLYING_ROCK_REQUIRED
    sdl2
    SDL2_ttf
    SDL2_mixer
)

pkg_check_modules(FLYING_ROCK REQUIRED ${FLYING_ROCK_REQUIRED})

set (FLYING_ROCK_LOADER_SRC
    main.cpp
    asteroid_manager.cpp
    bullet_manager.cpp
    geometry.cpp
    position_updater.cpp
    score.cpp
    score_observer.cpp
    sdl_backend.cpp
    sdl_error.cpp
    sdl_renderer.cpp
    sdl_texture.cpp
    ship.cpp
    random_generator.cpp
    timer.cpp
    vector.cpp
    vector_lines.cpp
)

set (FLYING_ROCK_LOADER_HDR
    asteroid.h
    asteroid_manager.h
    bullet.h
    bullet_manager.h
    color.h
    default_asteroid_shapes.h
    default_ship_shape.h
    position_updater.h
    score.h
    score_observer.h
    sdl_backend.h
    sdl_error.h
    sdl_renderer.h
    sdl_texture.h
    ship.h
    random_generator.h
    timer.h
    vector.h
    vector_lines.h
)

include_directories (${FLYING_ROCK_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (flying_rocks ${FLYING_ROCK_LOADER_SRC} ${FLYING_ROCK_LOADER_HDR})

target_link_libraries (flying_rocks ${FLYING_ROCK_LIBRARIES} ${FLYING_ROCK_LDFLAGS})
