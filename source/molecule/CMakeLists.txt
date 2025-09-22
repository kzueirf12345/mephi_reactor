add_library(molecule STATIC
    source/Molecule.cpp
    source/MoleculeCircle.cpp
    source/MoleculeSquare.cpp
    source/MoleculeManager.cpp
)

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