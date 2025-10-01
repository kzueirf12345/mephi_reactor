add_library(windows STATIC
    source/Window.cpp
    source/Reactor.cpp
    source/Plot.cpp
    source/buttons/Button.cpp
    source/Clock.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -g
        -O0
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )
    add_link_options(
        -fsanitize=address
        -fsanitize=undefined
    )
else()
    add_compile_options(-O2)
endif()

set_target_properties(windows PROPERTIES FOLDER "source")

target_include_directories(windows
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(windows
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
        molecule
        events
)