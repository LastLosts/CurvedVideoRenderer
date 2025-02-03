cmake_minimum_required(VERSION 3.20)

project(video-render LANGUAGES C CXX)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "src/*.hpp")

add_subdirectory(dependencies)

add_executable(renderer ${sources} ${headers})

if(WIN32)
  set_property(TARGET renderer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
                                        "${CMAKE_SOURCE_DIR}")

  file(GLOB ffmpeg_dlls "${CMAKE_SOURCE_DIR}/dependencies/ffmpeg-win/bin/*.dll")

  add_custom_command(
    TARGET renderer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ffmpeg_dlls}
            $<TARGET_FILE_DIR:renderer>)
endif()

target_link_libraries(renderer PRIVATE dependencies)
set_target_properties(renderer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin)
set_target_properties(renderer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin)
