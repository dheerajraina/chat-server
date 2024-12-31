# cmake_minimum_required(VERSION 3.14)

# project(ChatServer VERSION 1.0 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)


# set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
# set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")


# set(SRC_FILES
#     ${SRC_DIR}/core/main.cpp
#     ${SRC_DIR}/core/server.cpp
# )

# include_directories(${INCLUDE_DIR})

# find_package(Boost REQUIRED COMPONENTS system)
# if(Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIRS})
# endif()

# add_executable(chat_server ${SRC_FILES})


# # Link Boost libraries
# target_link_libraries(chat_server PRIVATE ${Boost_LIBRARIES})


# # Add threads (for Linux)
# find_package(Threads REQUIRED)
# target_link_libraries(chat_server PRIVATE Threads::Threads)


# # Output configuration
# message(STATUS "Project: ${PROJECT_NAME}")
# message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
# message(STATUS "Boost Include Dir: ${Boost_INCLUDE_DIRS}")
# message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")








cmake_minimum_required(VERSION 3.14)
project(ChatServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(PkgConfig REQUIRED)

# MongoDB C++ Driver
pkg_check_modules(LIBMONGOCXX REQUIRED libmongocxx)
pkg_check_modules(LIBBSONCXX REQUIRED libbsoncxx)



# Redis++
pkg_check_modules(REDISPP REQUIRED redis++)


find_package(jsoncpp REQUIRED)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Crow Library (assumes Crow is downloaded or installed locally)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)



set(SRC_FILES
    ${SRC_DIR}/core/main.cpp
    ${SRC_DIR}/core/server.cpp
    ${SRC_DIR}/modules/databases/mongodb/mongodb_client.cpp
    ${SRC_DIR}/modules/auth/routes/signup_route.cpp
    ${SRC_DIR}/modules/auth/routes/login_route.cpp
    ${SRC_DIR}/modules/user/routes/get_contacts_route.cpp
)

include_directories(${INCLUDE_DIR})


# Add the executable
add_executable(chat_server ${SRC_FILES})

# Link libraries
target_link_libraries(chat_server
    PUBLIC
        ${LIBMONGOCXX_LIBRARIES}
        ${LIBBSONCXX_LIBRARIES}
        ${REDISPP_LIBRARIES}
        jsoncpp
)

# Include directories
target_include_directories(chat_server PRIVATE
    ${REDISPP_INCLUDE_DIRS}
)



# Include directories
target_include_directories(chat_server PRIVATE
    ${LIBMONGOCXX_INCLUDE_DIRS}
    ${LIBBSONCXX_INCLUDE_DIRS}
    ${REDISPP_INCLUDE_DIRS}
    /usr/local/include/mongocxx/v_noabi  # Add this to include the correct versioning path
)

# Compiler flags
target_compile_options(chat_server PRIVATE
    ${LIBMONGOCXX_CFLAGS_OTHER}
    ${LIBBSONCXX_CFLAGS_OTHER}
    ${REDISPP_CFLAGS_OTHER}
)