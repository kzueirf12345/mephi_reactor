#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <cmath>
#include <cstdlib>
#include <X11/Xlib.h>
#include <memory>

#include "common/ErrorHandle.hpp"
#include "events/EventCoord.hpp"
#include "molecule/MoleculeManager.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "windows/Reactor.hpp"
#include "windows/Window.hpp"
#include "windows/Plot.hpp"

constexpr unsigned int WINDOW_WIDTH    = 1720;
constexpr unsigned int WINDOW_HEIGHT   = 900;
constexpr unsigned int FRAMERATE_LIMIT = 15;

sf::Font Common::GLOBAL_FONT = {};

enum GlobWindowIndexses {
    REACTOR = 0
};

int main()
{
    // =======================INITS=====================

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );
    window.setFramerateLimit(FRAMERATE_LIMIT);
    const sf::Color WINDOW_BG_COLOR(20, 20, 20);
    
    if (!Common::GLOBAL_FONT.loadFromFile("./data/Font.ttf")) {
        std::cerr << "Can't load font" << std::endl;
        return EXIT_FAILURE;
    }

    Mephi::Window globWindow(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            Mephi::Vector2d(WINDOW_WIDTH, WINDOW_HEIGHT),
            WINDOW_BG_COLOR
        )
    );

    auto reactor = std::make_unique<Mephi::Reactor> (
        Mephi::Rect(
            Mephi::Vector2d(100, 100),
            Mephi::Vector2d(1200, 350),
            sf::Color::Cyan, 
            sf::Color::Black, 
            5
        ),
        0.1
    );

    reactor->GenerateMolecules(500, 100);
    
    globWindow.AddChild(std::move(reactor));
    
    auto circlePlot = std::make_unique<Mephi::Plot> (
        Mephi::Rect(
            Mephi::Vector2d(100, 500),
            Mephi::Vector2d(300, 300)
        ),
        1, 
        1,
        Mephi::Vector2d(150, 150),
        0,
        [&globWindow](){ 
            return dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetMoleculeManager().GetCircleCnt();
        }
    );
    
    globWindow.AddChild(std::move(circlePlot));

    auto squarePlot = std::make_unique<Mephi::Plot> (
        Mephi::Rect(
            Mephi::Vector2d(450, 500),
            Mephi::Vector2d(300, 300)
        ),
        1, 
        1,
        Mephi::Vector2d(150, 150),
        0,
        [&globWindow](){ 
            return dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetMoleculeManager().GetSquareCnt();
        }
    );
    
    globWindow.AddChild(std::move(squarePlot));

    auto tempPlot = std::make_unique<Mephi::Plot> (
        Mephi::Rect(
            Mephi::Vector2d(800, 500),
            Mephi::Vector2d(300, 300)
        ),
        1, 
        1,
        Mephi::Vector2d(150, 150),
        0,
        [&globWindow](){ 
            return dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().Average();
        }
    );
    
    globWindow.AddChild(std::move(tempPlot));

    //=======================CYCLE==========================

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::MouseMoved:
                    globWindow.OnMouseMove(
                        Mephi::EventCoord(Mephi::Vector2d(sf::Mouse::getPosition(window)))
                    );
                    break;

                default:
                    break;
            }
        }

        window.clear();

        ERROR_HANDLE(globWindow.Draw(window));
        ERROR_HANDLE(globWindow.Update());

        std::cerr << dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().Average() << std::endl;
        std::cerr << globWindow[GlobWindowIndexses::REACTOR].IsHovered() << std::endl;

        window.display();
    }

    return EXIT_SUCCESS;
}