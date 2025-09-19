add_library(threads STATIC
    source/ThreadManager.cpp
)

set_target_properties(threads PROPERTIES FOLDER "source")

target_include_directories(threads
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(threads
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
)