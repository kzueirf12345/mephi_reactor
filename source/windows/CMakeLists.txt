add_library(windows STATIC
    source/Window.cpp
    source/Plot.cpp
    source/buttons/Button.cpp
)

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
)