if (NOT ENABLE_WAYLAND)
    return()
endif()
include_directories (
  ${CMAKE_SOURCE_DIR}/sample_common/include
  ${CMAKE_SOURCE_DIR}/sample_misc/wayland/include
)

list(APPEND LIBS sample_common)

set(DEPENDENCIES  libva libdrm wayland-client)

make_library(mfx_wayland none shared)

