set (GTEST_BACKEND_SOURCE
   test_main.cpp
)

add_executable (gtest-test-PROJECT_NAME ${GTEST_BACKEND_SOURCE})

target_link_libraries (
    gtest-test-PROJECT_NAME
    pthread
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARY}
    ${GMOCK_MAIN_LIBRARY}
)

add_custom_target (check COMMAND gtest-test-PROJECT_NAME)

add_dependencies(gtest-test-PROJECT_NAME GMock)
