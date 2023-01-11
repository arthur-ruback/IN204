cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(IN204_Messenger VERSION 0.1)


add_executable(Client source/client.cpp)

add_executable(Server source/server.cpp)

target_link_libraries(Client sfml-system sfml-graphics sfml-window sfml-network)

target_link_libraries(Server sfml-system sfml-graphics sfml-window sfml-network)

#find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Sample toolchain file for building for Windows from an Ubuntu Linux system.
#
# Typical usage:
#    *) install cross compiler: `sudo apt-get install mingw-w64`
#    *) cd build
#    *) cmake -DCMAKE_TOOLCHAIN_FILE=~/windows.cmake ..

#set(CMAKE_SYSTEM_NAME Windows)
#set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

# # cross compilers to use for C, C++ and Fortran
# set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
# set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
# set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
# set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

# # target environment on the build host system
# set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# # modify default behavior of FIND_XXX() commands
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# set(SFML_INCLUDE_DIR "gitlab_ci/SFML-2.5.1-WIN/include")
# include_directories(gitlab_ci/SFML-2.5.1-WIN/include)
# set(SFML_LIBRARY_DIR "gitlab_ci/SFML-2.5.1-WIN/lib")
# link_directories(gitlab_ci/SFML-2.5.1-WIN/lib)
# set(SFML_DIR "gitlab_ci/SFML-2.5.1-WIN/lib/cmake/SFML")