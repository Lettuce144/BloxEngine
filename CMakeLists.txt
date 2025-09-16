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

# IMPORTANT: Set some variables to make sure the project is built correctly
    # Raylib
    set(GLFW_BUILD_WAYLAND ON CACHE BOOL "Use Wayland for GLFW")
    set(GLFW_BUILD_X11 ON CACHE BOOL "Use X11 for GLFW")

    # FIXME:
    # This is a workaround for the fact a vendor from raylib-cpp does not support CMake 3.20+ yet
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)


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
        GIT_TAG 5.5
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    if (NOT DEFINED RAYLIB_CPP_VERSION)
        set(RAYLIB_CPP_VERSION v5.5.0)
    endif()
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG ${RAYLIB_CPP_VERSION}
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# EnTT
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

find_package(tomlplusplus QUIET)
if (NOT tomlplusplus_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        v3.4.0
    )
    FetchContent_MakeAvailable(tomlplusplus)
endif()

find_package(NativeFileDialog QUIET)
if (NOT NativeFileDialog_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        NativeFileDialog
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(NativeFileDialog)
endif()

find_package(vfspp QUIET)
if (NOT vfspp)
    include(FetchContent)
    FetchContent_Declare(
        vfspp
        GIT_REPOSITORY https://github.com/Lettuce144/vfspp-fixed_miniz-cpp.git
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(vfspp)
endif()

# ImGui
add_subdirectory(src/Thirdparty/)


# ------------------------------------------------------- END Dependencies

add_subdirectory(src/Engine)

add_subdirectory(src/Editor)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# ------------------------------------------------------- Link libraries here

# Note that PUBLIC means that the library will be linked to the target and other targets automatically 
# get the library linked to them as well. PRIVATE means that only the target will be linked to the library.
target_link_libraries(BloxEngine PUBLIC raylib raylib_cpp imgui_raylib nfd tomlplusplus::tomlplusplus)
target_link_libraries(BloxEngine PUBLIC EnTT::EnTT)
target_link_libraries(BloxEngine PUBLIC vfspp)
target_link_libraries(BloxEngineEditor PRIVATE BloxEngine)

add_compile_definitions(VFSPP_ENABLE_MULTITHREADING)

# Include directories
target_include_directories(BloxEngine PUBLIC Ent)
target_include_directories(BloxEngine PUBLIC ${NativeFileDialog_SOURCE_DIR}/src/include)
target_include_directories(BloxEngineEditor PUBLIC "Engine/public")

# ------------------------------------------------------- END Link libraries here

