cmake_minimum_required(VERSION 3.10)


project(MultiballCollision)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SDL2_DIR "${CMAKE_SOURCE_DIR}/include/SDL2")
set(SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/include/SDL2_ttf")
set(SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/include/SDL2_image")


include_directories(
    ${SDL2_DIR}/include/SDL2
    ${SDL2_TTF_DIR}/include/SDL2
    ${SDL2_IMAGE_DIR}/include/SDL2
)


include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(
    ${SDL2_DIR}/lib
    ${SDL2_TTF_DIR}/lib
    ${SDL2_IMAGE_DIR}/lib
)


file(GLOB SRC_FILES src/*.cpp)


add_executable(main ${SRC_FILES})


target_link_libraries(main SDL2 SDL2main SDL2_ttf SDL2_image)


if (WIN32)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_DIR}/bin/SDL2.dll
        ${SDL2_TTF_DIR}/bin/SDL2_ttf.dll
        ${SDL2_IMAGE_DIR}/bin/SDL2_image.dll
        $<TARGET_FILE_DIR:main>
    )
endif()
