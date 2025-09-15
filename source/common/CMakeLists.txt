add_library(common STATIC
    source/ErrorHandle.cpp
)

set_target_properties(common PROPERTIES FOLDER "source")

target_include_directories(common
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)