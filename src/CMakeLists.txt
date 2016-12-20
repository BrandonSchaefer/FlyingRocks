find_package (PkgConfig)

set (ASTEROIDS_REQUIRED
    sdl2
    SDL2_image
    SDL2_ttf
)

pkg_check_modules(ASTEROIDS REQUIRED ${ASTEROIDS_REQUIRED})

set (ASTEROIDS_LOADER_SRC
    main.cpp
    sdl_backend.cpp
    sdl_error.cpp
    vector.cpp
)

set (ASTEROIDS_LOADER_HDR
    sdl_backend.h
    sdl_error.h
    vector.h
)

include_directories (${ASTEROIDS_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (asteroids ${ASTEROIDS_LOADER_SRC} ${ASTEROIDS_LOADER_HDR})

target_link_libraries (asteroids ${ASTEROIDS_LIBRARIES} ${ASTEROIDS_LDFLAGS})
