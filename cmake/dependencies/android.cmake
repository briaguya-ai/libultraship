find_package(SDL3 QUIET)
if (NOT ${SDL3_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG preview-3.1.8
    )
    message("SDL3 not found. Downloading now...")
    FetchContent_MakeAvailable(SDL3)
    message("SDL3 downloaded to " ${FETCHCONTENT_BASE_DIR}/sdl3-src)
endif()

target_link_libraries(ImGui PUBLIC SDL3::SDL3)
