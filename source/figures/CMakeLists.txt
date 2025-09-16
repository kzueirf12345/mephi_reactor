add_library(figures STATIC
    source/Rect.cpp
)

set_target_properties(figures PROPERTIES FOLDER "source")

target_include_directories(figures
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(figures
    PRIVATE 
        sfml-graphics
        common
        vector
)