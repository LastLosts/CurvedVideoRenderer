cmake_minimum_required(VERSION 3.20)

project(video-render LANGUAGES C CXX)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "src/*.hpp")

add_subdirectory(dependencies)

add_executable(renderer ${sources} ${headers})

if(WIN32)
  file(GLOB ffmpeg_dlls "${CMAKE_SOURCE_DIR}/dependencies/ffmpeg-win/bin/*.dll")

  add_custom_command(
    TARGET renderer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ffmpeg_dlls}
            $<TARGET_FILE_DIR:renderer>)
endif()

target_link_libraries(renderer PRIVATE dependencies)
