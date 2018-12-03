cmake_minimum_required(VERSION 2.8.7)
project(rocket)
include_directories("${CMAKE_SOURCE_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -Werror")

file(GLOB TCP_HEADER_FILES "src/tcp/*.h")
file(GLOB TCP_SOURCE_FILES "src/tcp/*.cc")
file(GLOB BASE_HEADER_FILES "src/base/*.h")
file(GLOB BASE_SOURCE_FILES "src/base/*.cc")
file(GLOB CONCURRENCY_HEADER_FILES "src/concurrency/*.h")
file(GLOB CONCURRENCY_SOURCE_FILES "src/concurrency/*.cc")
file(GLOB CONTAINER_HEADER_FILES "src/container/*.h")
file(GLOB CONTAINER_SOURCE_FILES "src/container/*.cc")

set(SOURCE_FILES
    ${BASE_SOURCE_FILES}
    ${CONCURRENCY_SOURCE_FILES}
    ${CONTAINER_SOURCE_FILES}
    ${TCP_SOURCE_FILES})

# build shared library
add_library(rocket SHARED ${SOURCE_FILES})
set_target_properties(rocket PROPERTIES VERSION 1.0 SOVERSION 1)

# build static library
add_library(rocket_s STATIC ${SOURCE_FILES})
set_target_properties(rocket_s PROPERTIES OUTPUT_NAME "rocket")

link_directories("${PROJECT_BINARY_DIR}")
set(TEST_TCP_SERVER_SOURCE_FILES examples/tcp_server.cc)
add_executable(rocket_tcp_server_test ${TEST_TCP_SERVER_SOURCE_FILES})
target_link_libraries(rocket_tcp_server_test rocket pthread)

set(TEST_TCP_CLIENT_SOURCE_FILES examples/tcp_client.cc)
add_executable(rocket_tcp_client_test ${TEST_TCP_CLIENT_SOURCE_FILES})
target_link_libraries(rocket_tcp_client_test rocket pthread)

set(TEST_CONSISTENT_HASH_SOURCE_FILES examples/consistent_hash_test.cc)
add_executable(consistent_hash_test ${TEST_CONSISTENT_HASH_SOURCE_FILES})
target_link_libraries(consistent_hash_test rocket pthread)

add_subdirectory(${PROJECT_SOURCE_DIR}/bin)