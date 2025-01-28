add_library(dependencies INTERFACE)

add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/src/gl.c")
target_include_directories(glad
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/include/")

set(GLFW_BUILD_WAYLAND off)
add_subdirectory(glfw)

add_subdirectory(glm)

if(UNIX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(
    LIBAV
    REQUIRED
    IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil)

  target_link_libraries(dependencies INTERFACE PkgConfig::LIBAV)
endif()
if(WIN32)
  file(GLOB_RECURSE ffmpeg_win_static_libs
       "${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-win/lib/*.lib")
  # message(STATUS ${ffmpeg_win_static_libs})

  # add_library(ffmpeg INTERFACE ${ffmpeg_win_static_libs})
  # target_include_directories( ffmpeg INTERFACE
  # "${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-win/include/")

  target_link_libraries(dependencies INTERFACE ${ffmpeg_win_static_libs})
  target_include_directories(
    dependencies INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-win/include/")

endif()

target_link_libraries(dependencies INTERFACE glfw glad glm)
