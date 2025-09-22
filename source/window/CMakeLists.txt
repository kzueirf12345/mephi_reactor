add_library(window STATIC
    source/Window.cpp
    source/Reactor.cpp
)

set_target_properties(window PROPERTIES FOLDER "source")

target_include_directories(window
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(window
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
)