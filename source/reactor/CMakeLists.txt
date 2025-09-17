add_library(reactor STATIC
    source/Reactor.cpp
)

set_target_properties(reactor PROPERTIES FOLDER "source")

target_include_directories(reactor
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(reactor
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
)