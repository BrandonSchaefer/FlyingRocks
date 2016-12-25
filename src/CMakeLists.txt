find_package (PkgConfig)

set (ASTEROIDS_REQUIRED
    sdl2
    SDL2_ttf
    SDL2_mixer
)

pkg_check_modules(ASTEROIDS REQUIRED ${ASTEROIDS_REQUIRED})

set (ASTEROIDS_LOADER_SRC
    main.cpp
    asteroid_manager.cpp
    bullet_manager.cpp
    geometry.cpp
    position_updater.cpp
    sdl_backend.cpp
    sdl_error.cpp
    sdl_renderer.cpp
    ship.cpp
    timer.cpp
    vector.cpp
    vector_lines.cpp
)

set (ASTEROIDS_LOADER_HDR
    asteroid.h
    asteroid_manager.h
    bullet.h
    bullet_manager.h
    color.h
    position_updater.h
    sdl_backend.h
    sdl_error.h
    sdl_renderer.h
    ship.h
    timer.h
    vector.h
    vector_lines.h
)

include_directories (${ASTEROIDS_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (asteroids ${ASTEROIDS_LOADER_SRC} ${ASTEROIDS_LOADER_HDR})

target_link_libraries (asteroids ${ASTEROIDS_LIBRARIES} ${ASTEROIDS_LDFLAGS})
