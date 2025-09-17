#include <SFML/Config.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <cmath>
#include <memory>

#include "common/ErrorHandle.hpp"
#include "reactor/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"
#include "molecule/MoleculeCircle.hpp"
#include "molecule/MoleculeSquare.hpp"

int main()
{
    constexpr unsigned int WINDOW_WIDTH         = 2048;
    constexpr unsigned int WINDOW_HEIGHT        = 1024;

    constexpr unsigned int FRAMERATE_LIMIT      = 50;

    const sf::Color WINDOW_BG_COLOR(240, 240, 240);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::Reactor (
        Mephi::Rect(
            Mephi::Vector2d(100, 100), 
            Mephi::Vector2d(500, 500), 
            sf::Color::Cyan, 
            sf::Color::Black, 
            5
        )
    );

    Mephi::MephiManager manager(
        Mephi::Reactor (
            Mephi::Rect(
                Mephi::Vector2d(100, 100), 
                Mephi::Vector2d(500, 500), 
                sf::Color::Cyan, 
                sf::Color::Black, 
                5
            )
        ), 
        {}
    );

    Mephi::MoleculeCircle molecule1(Mephi::Vector2d(200, 200), Mephi::Vector2d(-10, 10), 1, sf::Color::Red, 10);
    Mephi::MoleculeSquare molecule2(Mephi::Vector2d(300, 250), Mephi::Vector2d(10, -10), 1, sf::Color::Red, 10);

    manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule1));
    manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule2));


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(WINDOW_BG_COLOR);

        ERROR_HANDLE(manager.Draw(window));
        ERROR_HANDLE(manager.Update());

        window.display();
    }

    return 0;
}