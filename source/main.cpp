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

#include "common/ErrorHandle.hpp"
#include "reactor/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"

int main()
{
    constexpr unsigned int WINDOW_WIDTH         = 1920;
    constexpr unsigned int WINDOW_HEIGHT        = 1200;

    constexpr unsigned int FRAMERATE_LIMIT      = 15;

    const sf::Color WINDOW_BG_COLOR(20, 20, 20);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::MephiManager manager(
        Mephi::Reactor (
            Mephi::Rect(
                Mephi::Vector2d(100, 100), 
                Mephi::Vector2d(WINDOW_WIDTH - 500, WINDOW_HEIGHT - 100), 
                sf::Color::Cyan, 
                sf::Color::Black, 
                5
            )
        ), 
        {}
    );

    ERROR_HANDLE(manager.GenerateMolecules(1000));

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