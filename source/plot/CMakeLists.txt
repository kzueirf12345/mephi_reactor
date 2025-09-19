add_library(plot STATIC
    source/Plot.cpp
)

set_target_properties(plot PROPERTIES FOLDER "source")

target_include_directories(plot
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(plot
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
)