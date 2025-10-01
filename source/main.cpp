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
#include <iostream>
#include <memory>

#include "common/ErrorHandle.hpp"
#include "events/EventCoord.hpp"
#include "events/EventMouseButton.hpp"
#include "molecule/MoleculeManager.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "windows/Reactor.hpp"
#include "windows/Window.hpp"
#include "windows/Plot.hpp"
#include "windows/buttons/AdjustButton.hpp"
#include "windows/Clock.hpp"

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
        ),
        false
    );

    auto reactor = std::make_unique<Mephi::Reactor> (
        Mephi::Rect(
            Mephi::Vector2d(100, 100),
            Mephi::Vector2d(1000, 350),
            sf::Color::Cyan, 
            sf::Color::Black, 
            5
        ),
        0.1
    );

    reactor->GenerateMolecules(500, 10);
    
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

    auto buttonPanel = std::make_unique<Mephi::Window> (
        Mephi::Rect(
            Mephi::Vector2d(1200, 100),
            Mephi::Vector2d(450, 700)
        ),
        true
    );

    auto tempLeftButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(25, 100),
            Mephi::Vector2d(100, 50),
            sf::Color(220, 20, 60)
        ),
        &dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().left,
        100,
        "Left"
    );

    buttonPanel->AddChild(std::move(tempLeftButton));

    auto tempRightButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(325, 100),
            Mephi::Vector2d(100, 50),
            sf::Color(220, 20, 60)
        ),
        &dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().right,
        100,
        "Right"
    ); 

    buttonPanel->AddChild(std::move(tempRightButton));

    auto tempTopButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(175, 25),
            Mephi::Vector2d(100, 50),
            sf::Color(220, 20, 60)
        ),
        &dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().top,
        100,
        "Top"
    );

    buttonPanel->AddChild(std::move(tempTopButton));

    auto tempBottomButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(175, 175), 
            Mephi::Vector2d(100, 50),
            sf::Color(220, 20, 60)
        ),
        &dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().bottom,
        100,
        "Bottom"
    ); 

    buttonPanel->AddChild(std::move(tempBottomButton));
    
    globWindow.AddChild(std::move(buttonPanel));

    auto Clock = std::make_unique<Mephi::Clock>(
        Mephi::Rect(
            Mephi::Vector2d(175, 175), 
            Mephi::Vector2d(200, 200),
            sf::Color(220, 20, 60)
        )
    );

    globWindow.AddChild(std::move(Clock));

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

                    if (sf::Mouse::isButtonPressed(Mephi::EventMouseButton::MOVE_BUTTON_)) {
                        globWindow.OnMouseDrag(
                            Mephi::EventCoord(Mephi::Vector2d(sf::Mouse::getPosition(window)))
                        );
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    globWindow.OnMousePress(Mephi::EventMouseButton(
                        Mephi::Vector2d(sf::Mouse::getPosition(window)),
                        event.mouseButton.button
                    ));
                    
                    break;

                case sf::Event::MouseButtonReleased:
                    globWindow.OnMouseUnpress(Mephi::EventMouseButton(
                        Mephi::Vector2d(sf::Mouse::getPosition(window)),
                        event.mouseButton.button
                    ));
                    break;

                default:
                    break;
            }
        }

        window.clear();

        ERROR_HANDLE(globWindow.Draw(window));
        ERROR_HANDLE(globWindow.Update());

        // std::cerr << dynamic_cast<Mephi::Reactor*>(&globWindow[REACTOR])->GetTemp().Average() << std::endl;
        // std::cerr << "is Hovered " << globWindow[1].IsHovered() << std::endl;
        // std::cerr << "is Selected " << globWindow[1].isSelected() << std::endl;

        window.display();
    }

    return EXIT_SUCCESS;
}