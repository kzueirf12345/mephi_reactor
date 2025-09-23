add_library(mephi STATIC
    source/MephiManager.cpp
)

set_target_properties(mephi PROPERTIES FOLDER "source")

target_include_directories(mephi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(mephi
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
        molecule
        windows
)