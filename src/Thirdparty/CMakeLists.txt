cmake_minimum_required(VERSION 3.11)

# TODO: Make the backend files not exist
add_library(imgui_raylib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/rlImGui.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp
)

# Link with raylib
target_link_libraries(imgui_raylib PUBLIC raylib)

target_include_directories(imgui_raylib
PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/imgui
  ${CMAKE_CURRENT_LIST_DIR}/imgui/backends
  ${CMAKE_CURRENT_LIST_DIR}/rlimgui/include
)
