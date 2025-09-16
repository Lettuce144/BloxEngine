cmake_minimum_required(VERSION 3.12)
project(BloxEngineEditor)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(EDITOR_INSTANCE)

add_executable(BloxEngineEditor
  editor.cpp
  EditorUI/EditorMenuBar.cpp
)

target_include_directories(BloxEngineEditor
  PRIVATE
  "private/"
)

