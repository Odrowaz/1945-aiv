cmake_minimum_required(VERSION 4.0)
project(1945 LANGUAGES C)

file(GLOB SRC_FILES src/*.c)

add_executable(1945_App ${SRC_FILES})
target_include_directories(1945_App PRIVATE include/)
target_compile_features(1945_App PRIVATE c_std_11)
target_link_directories(1945_App PRIVATE lib)
target_link_libraries(1945_App PRIVATE raylibdll)

add_custom_command(TARGET 1945_App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib.dll"
        $<TARGET_FILE_DIR:1945_App>
)

add_custom_command(TARGET 1945_App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"   # cartella sorgente
        "$<TARGET_FILE_DIR:1945_App>/assets"   # cartella destinazione
)