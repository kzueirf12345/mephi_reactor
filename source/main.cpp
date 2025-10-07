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

#include "common/Constants.hpp"
#include "common/ErrorHandle.hpp"
#include "events/EventCoord.hpp"
#include "events/EventMouseButton.hpp"
#include "molecule/MoleculeManager.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "windows/Reactor.hpp"
#include "windows/Window.hpp"
#include "windows/Plot.hpp"
#include "windows/buttons/AddMoleculesButton.hpp"
#include "windows/buttons/DeleteMoleculesButton.hpp"
#include "windows/buttons/AdjustButton.hpp"
#include "windows/Clock.hpp"
#include "windows/ScrollBar.hpp"

sf::Font Common::GLOBAL_FONT = {};

int main()
{
    // =======================INITS=====================

    sf::RenderWindow window(
        sf::VideoMode(Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );
    window.setFramerateLimit(Common::FRAMERATE_LIMIT);
    
    if (!Common::GLOBAL_FONT.loadFromFile("./data/Font.ttf")) {
        std::cerr << "Can't load font" << std::endl;
        return EXIT_FAILURE;
    }

    Mephi::Window globWindow(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            Mephi::Vector2d(Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT)
        ),
        false
    );

    globWindow.GetFillColor() = Common::TNC::ProgramBackground;

    auto reactor = std::make_unique<Mephi::Reactor> (
        Mephi::Rect(
            Mephi::Vector2d(100, 100),
            Mephi::Vector2d(1000, 350)
        ),
        0.1
    );

    auto* reactorPtr = reactor.get();
    
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
        [&reactorPtr](){ 
            return reactorPtr->GetMoleculeManager().GetCircleCnt();
        }
    );

    auto* circlePlotPtr = circlePlot.get();

    auto scrollXCirclePlot = std::make_unique<Mephi::ScrollBar>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            Mephi::Vector2d(circlePlot->GetRect().Width(), 0.1 * circlePlot->GetRect().Height())
        ),
        [&circlePlotPtr](double percentage) { return circlePlotPtr->ChangeScaleX(percentage); }
    );

    auto* scrollXCirclePlotPtr = scrollXCirclePlot.get();

    circlePlot->AddChild(std::move(scrollXCirclePlot));

    auto scrollYCirclePlot = std::make_unique<Mephi::ScrollBar>(
        Mephi::Rect(
            Mephi::Vector2d(0, scrollXCirclePlotPtr->GetRect().Height()),
            Mephi::Vector2d(
                0.1 * circlePlot->GetRect().Width(), 
                circlePlot->GetRect().Height() - scrollXCirclePlotPtr->GetRect().Height()
            )
        ),
        [&circlePlotPtr](double percentage) { return circlePlotPtr->ChangeScaleY(percentage); },
        false
    );

    auto* scrollYCirclePlotPtr = scrollYCirclePlot.get();

    circlePlot->AddChild(std::move(scrollYCirclePlot));
    
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
        [&reactorPtr](){ 
            return reactorPtr->GetMoleculeManager().GetSquareCnt();
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
        [&reactorPtr](){ 
            return reactorPtr->GetTemp().Average();
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
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().left,
        100,
        "Left"
    );

    buttonPanel->AddChild(std::move(tempLeftButton));

    auto tempRightButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(325, 100),
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().right,
        100,
        "Right"
    ); 

    buttonPanel->AddChild(std::move(tempRightButton));

    auto tempTopButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(175, 25),
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().top,
        100,
        "Top"
    );

    buttonPanel->AddChild(std::move(tempTopButton));

    auto tempBottomButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(175, 175), 
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().bottom,
        100,
        "Bottom"
    ); 

    buttonPanel->AddChild(std::move(tempBottomButton));

    auto AddMoleculesButton = std::make_unique<Mephi::AddMoleculesButton> (
        Mephi::Rect(
            Mephi::Vector2d(25, 300), 
            Mephi::Vector2d(175, 100)
        ),
        "Add mol",
        reactorPtr,
        10,
        Common::MAX_MOLECULE_SPEED
    ); 

    buttonPanel->AddChild(std::move(AddMoleculesButton));

    auto DeleteMoleculesButton = std::make_unique<Mephi::DeleteMoleculesButton> (
        Mephi::Rect(
            Mephi::Vector2d(250, 300), 
            Mephi::Vector2d(175, 100)
        ),
        "Delete mol",
        reactorPtr,
        10
    ); 

    buttonPanel->AddChild(std::move(DeleteMoleculesButton));
    
    globWindow.AddChild(std::move(buttonPanel));

    auto Clock = std::make_unique<Mephi::Clock>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0), 
            Mephi::Vector2d(200, 200)
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

                    globWindow.OnMouseDrag(
                        Mephi::EventCoord(Mephi::Vector2d(sf::Mouse::getPosition(window)))
                    );
                    
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

        // std::cerr << "Plot " << circlePlotPtr->isSelected() << std::endl;
        // std::cerr << "Scroll " << scrollXCirclePlotPtr->isSelected() << std::endl;

        window.display();
    }

    return EXIT_SUCCESS;
}