cmake_minimum_required(VERSION 3.0.0)
project(udemos
  VERSION 0.1.0 
  DESCRIPTION "udemos"
  # HOMEPAGE_URL ""
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall")
string(APPEND CMAKE_CXX_FLAGS " -Wbuiltin-macro-redefined")
string(APPEND CMAKE_CXX_FLAGS " -pedantic")
# string(APPEND CMAKE_CXX_FLAGS " -Werror")

# if(NOT LINUX) should work, too, if you need that
if(UNIX AND NOT APPLE) 
    message(STATUS ">>> LINUX")
    # linux stuff here
    INCLUDE_DIRECTORIES(
        /opt/vcpkg/installed/x64-linux/include/
    )

    LINK_DIRECTORIES(
        /opt/vcpkg/installed/x64-linux/lib/
    )  
    set(CMAKE_CXX_FLAGS "-pthread")  
else()
    message(STATUS ">>> APPLE")
    # stuff that should happen not on Linux 
    INCLUDE_DIRECTORIES(
        # Uncomment this on M1 Mac
        /Users/sinisaabramovic/projects/externals/vcpkg/installed/arm64-osx/include/

        # Uncomment this on iMac - Intel
        #/Users/sinomac/Projects/externals/vcpkg/installed/x64-osx/include/
    )

    LINK_DIRECTORIES(
        # Uncomment this on M1 Mac
        /Users/sinisaabramovic/projects/externals/vcpkg/installed/arm64-osx/lib/

        # Uncomment this on iMac - Intel
        #/Users/sinomac/projects/externals/vcpkg/installed/x64-osx/lib/
    )
endif()

file(GLOB_RECURSE SOURCES "main.cpp" "src/*.cpp" "src/*.h" "src/*.hpp" "demo/*.cpp" "demo/*.h" "demo/*.hpp")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES})

if(UNIX AND NOT APPLE)
    message(STATUS ">>> LINUX")
    #/opt/vcpkg/installed/
    set(CMAKE_PREFIX_PATH "/opt/vcpkg/installed/x64-linux/")
else()
    message(STATUS ">>> APPLE")
    set(CMAKE_PREFIX_PATH "/Users/sinisaabramovic/projects/externals/vcpkg/installed/arm64-osx/")
endif()

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(rxcpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)  # link Boost Static libraries

#find_package(Boost CONFIG REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL ZLIB::ZLIB rapidjson rxcpp nlohmann_json::nlohmann_json)
