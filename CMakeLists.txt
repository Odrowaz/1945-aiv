cmake_minimum_required(VERSION 4.0)
project(1945 LANGUAGES C)

file(GLOB SRC_FILES game/src/*.c)

add_library(ECS SHARED
    ecs/src/ecs.c
)
target_include_directories(ECS PUBLIC
    ecs/include
)

add_executable(1945_Game ${SRC_FILES})
target_include_directories(1945_Game PRIVATE game/include)
target_compile_features(1945_Game PRIVATE c_std_11)
target_link_directories(1945_Game PRIVATE lib)
target_link_libraries(1945_Game PRIVATE raylibdll)
target_link_libraries(1945_Game PRIVATE ECS)


add_custom_command(TARGET 1945_Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib.dll"
        $<TARGET_FILE_DIR:1945_Game>
)

add_custom_command(TARGET 1945_Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:1945_Game>/assets"
)
