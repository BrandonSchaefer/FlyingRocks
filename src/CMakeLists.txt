find_package (PkgConfig)

set (PROJECT_NAME_REQUIRED
    )

pkg_check_modules(PROJECT_NAME REQUIRED ${PROJECT_NAME_REQUIRED})

set (PROJECT_NAME_LOADER_SRC
     main.cpp
    )

set (PROJECT_NAME_LOADER_HDR
    )

include_directories (${PROJECT_NAME_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (project_name ${PROJECT_NAME_LOADER_SRC} ${PROJECT_NAME_LOADER_HDR})

target_link_libraries (project_name ${PROJECT_NAME_LIBRARIES} ${PROJECT_NAME_LDFLAGS})
