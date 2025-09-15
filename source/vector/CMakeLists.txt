add_library(vector STATIC
    source/Vector.cpp
)

set_target_properties(vector PROPERTIES FOLDER "source")

target_include_directories(vector
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(vector
    PRIVATE 
        sfml-graphics
        common
)