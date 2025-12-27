include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)
 
FetchContent_MakeAvailable(ftxui)
 
add_executable(main main.cpp)
set_target_properties(main PROPERTIES OUTPUT_NAME "switcheroo")
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bin/WinDivert.lib
)

# Copy DLL to output directory
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/WinDivert.dll"
  $<TARGET_FILE_DIR:main>
)

# Copy SYS to output directory
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/WinDivert.sys"
  $<TARGET_FILE_DIR:main>
)
