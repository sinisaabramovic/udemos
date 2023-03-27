cmake_minimum_required(VERSION 3.0.0)
project(udemos
  VERSION 0.1.0 
  DESCRIPTION "udemos"
  LANGUAGES CXX
)

# Set build type
set(CMAKE_BUILD_TYPE Debug)

# Set C++ language options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion")

# Set compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPENSSL_API_COMPAT=0x10100000L")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")
endif()

# set(CMAKE_TOOLCHAIN_FILE "/Users/sinisaabramovic/projects/externals/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find required libraries
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Set include and link directories
if(UNIX AND NOT APPLE)
    message(STATUS ">>> LINUX")
    # Linux specific settings
    include_directories(
        /opt/vcpkg/installed/x64-linux/include/
    )
    link_directories(
        /opt/vcpkg/installed/x64-linux/lib/
    )  
    set(CMAKE_CXX_FLAGS "-pthread")  
else()
    message(STATUS ">>> APPLE")
    # Apple specific settings
    include_directories(
        # Uncomment this on M1 Mac
        /Users/sinisaabramovic/projects/externals/vcpkg/installed/arm64-osx/include/

        # Uncomment this on iMac - Intel
        #/Users/sinomac/Projects/externals/vcpkg/installed/x64-osx/include/
    )
    link_directories(
        # Uncomment this on M1 Mac
        /Users/sinisaabramovic/projects/externals/vcpkg/installed/arm64-osx/lib/

        # Uncomment this on iMac - Intel
        #/Users/sinomac/projects/externals/vcpkg/installed/x64-osx/lib/
    )
    if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
        set(ARCH_FLAGS "-arch arm64")
    else()
        set(ARCH_FLAGS "-arch x86_64")
    endif()
endif()

# Add source files
file(GLOB_RECURSE SOURCES "main.cpp" "src/*.cpp" "src/*.h" "src/*.hpp" "demo/*.cpp" "demo/*.h" "demo/*.hpp")

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set compiler options for the executable
target_compile_options(${PROJECT_NAME} PRIVATE ${ARCH_FLAGS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shorten-64-to-32)

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE -lcrypto -lz OpenSSL::SSL ZLIB::ZLIB nlohmann_json::nlohmann_json)

