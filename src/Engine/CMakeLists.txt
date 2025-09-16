add_library(BloxEngine STATIC
  gameinstance.cpp
  scene.cpp
  baseentity.cpp
  editorcamera.cpp
  components.cpp
  EditorUI/EditorPanel.cpp
  EditorUI/EditorViewportPanel.cpp
  EditorUI/SceneHierarchyPanel.cpp
  Renderer/forwardrenderer.cpp
  Renderer/Hosek/ArHosekSkyModel.cc
  Core/engine.cpp
  # add more cpp files to compile!
  # ej. game.cpp
)

target_compile_features(BloxEngine PUBLIC cxx_std_17)
set_target_properties(BloxEngine PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(BloxEngine
  PUBLIC
  "public/"
)

target_include_directories(BloxEngine
  PRIVATE
  "private/"
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	set(MY_DEBUG_OPTIONS /Wall /RTC)
	set(MY_RELEASE_OPTIONS /w3 /O2)
	target_compile_options(BloxEngine PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
	target_compile_options(BloxEngine PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(MY_DEBUG_OPTIONS -Wall -Wconversion -Wdouble-promotion -Weffc++ -Wextra -Wfloat-equal -Wmain -Wshadow -fstack-usage -pedantic -g3)
	set(MY_RELEASE_OPTIONS -O2)
	target_compile_options(BloxEngine PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
	target_compile_options(BloxEngine PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(MY_DEBUG_OPTIONS -Wall -Wconversion  -Wdouble-promotion -Weffc++ -Wextra -Wfloat-equal -Wmain -Wshadow -pedantic -g3)
	set(MY_RELEASE_OPTIONS -O2)
	target_compile_options(BloxEngine PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
	target_compile_options(BloxEngine PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
endif()

install(TARGETS BloxEngine RUNTIME DESTINATION ${BIN_DIR})
