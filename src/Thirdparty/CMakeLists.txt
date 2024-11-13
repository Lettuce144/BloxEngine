cmake_minimum_required(VERSION 3.11)

add_library(imgui_raylib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imguiimgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imguiimgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imguiimgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imguiimgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imguiimgui_widgets.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/rlimgui/rlImGui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imguibackends/imgui_impl_opengl3.cpp
)


# target_link_libraries(imgui_raylib PUBLIC raylib ${OPENGL_LIBRARIES})

target_include_directories(imgui_raylib
PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/imgui
  ${CMAKE_CURRENT_LIST_DIR}/imgui/backends
  ${CMAKE_CURRENT_LIST_DIR}/rlimgui/include
)
