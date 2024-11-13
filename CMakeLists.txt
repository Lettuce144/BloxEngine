cmake_minimum_required(VERSION 3.20)

project(
  BloxEngine
  VERSION 0.1.0
  DESCRIPTION "A Roblox engine clone made in C++ with RayLib"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)
	enable_testing()

  # Docs only available if this is the main app
  # find_package(Doxygen)
  # if(Doxygen_FOUND)
  #   add_subdirectory(docs)
  # else()
  #   message(STATUS "Doxygen not found, not building docs")
  # endif()
endif()

if(DEFINED CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Using vcpkg toolchain file")
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  # Find the packages needed
	# like: find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
else()
	message(STATUS "Using CMake find modules")
  # Find the packages needed
	# like: find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
endif()

# ------------------------------------------------------- Dependencies
include(FetchContent)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    if (NOT DEFINED RAYLIB_CPP_VERSION)
        set(RAYLIB_CPP_VERSION v5.0.2)
    endif()
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG ${RAYLIB_CPP_VERSION}
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

find_package(EnTT QUIET)
if (NOT EnTT_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.14.0
    )
    FetchContent_MakeAvailable(EnTT)
endif()

# ImGui
add_subdirectory(src/Thirdparty/)

# ------------------------------------------------------- END Dependencies

add_subdirectory(src/Engine)
add_subdirectory(src/GameDemo)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# ------------------------------------------------------- Link libraries here

# Note that PUBLIC means that the library will be linked to the target and other targets automatically 
# get the library linked to them as well. PRIVATE means that only the target will be linked to the library.
target_link_libraries(BloxEngine PUBLIC raylib raylib_cpp imgui_glfw)
target_link_libraries(BloxEngine PUBLIC EnTT::EnTT)

target_link_libraries(BloxEngineDemo PRIVATE BloxEngine)

target_include_directories(BloxEngine PUBLIC Ent)

# Include directories
target_include_directories(BloxEngineDemo PUBLIC "Engine/public")

# ------------------------------------------------------- END Link libraries here