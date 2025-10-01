add_library(molecule STATIC
    source/Molecule.cpp
    source/MoleculeCircle.cpp
    source/MoleculeSquare.cpp
    source/MoleculeManager.cpp
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

set_target_properties(molecule PROPERTIES FOLDER "source")

target_include_directories(molecule
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)

target_link_libraries(molecule
    PRIVATE 
        sfml-graphics
        common
        vector
        figures
)