#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <thread>
#include <X11/Xlib.h>
#include <vector>
#include <functional>

#include "common/ErrorHandle.hpp"
#include "molecule/MoleculeManager.hpp"
#include "windows/Plot.hpp"
#include "windows/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"
#include "threads/ThreadManager.hpp"
#include "windows/Window.hpp"
#include "windows/buttons/AdjustButton.hpp"

// TODO add molecule circle
// TODO add molecule square
// TODO delete also

constexpr unsigned int WINDOW_WIDTH    = 1720;
constexpr unsigned int WINDOW_HEIGHT   = 900;
constexpr unsigned int FRAMERATE_LIMIT = 15;

sf::Font Common::GLOBAL_FONT = {};


Mephi::MephiManager InitManager();

int main()
{
    // =======================INITS=====================
    XInitThreads();

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );
    window.setFramerateLimit(FRAMERATE_LIMIT);
    const sf::Color WINDOW_BG_COLOR(20, 20, 20);
    
    Mephi::MephiManager manager(std::move(InitManager()));

    //=======================CYCLE==========================

    if (!Common::GLOBAL_FONT.loadFromFile("./data/Font.ttf")) {
        std::cerr << "Can't load font" << std::endl;
        abort();
    }

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

        manager.Draw(window);

        manager.Update(Mephi::Vector2i(sf::Mouse::getPosition(window)));

        window.display();
    }

    return 0;
}

Mephi::MephiManager InitManager() {
    constexpr size_t MOLECULES_CNT = 500;

    Mephi::Reactor reactor(
        Mephi::Rect(
            Mephi::Vector2d(100, 100), 
            Mephi::Vector2d(WINDOW_WIDTH - 500, WINDOW_HEIGHT - 500), 
            sf::Color::Cyan, 
            sf::Color::Black, 
            5
        )
    );

    Mephi::Window toolbar(
        Mephi::Rect(
            Mephi::Vector2d(
                reactor.GetRect().GetRightCorner().x + 100, 
                reactor.GetRect().GetLeftCorner().y),
            Mephi::Vector2d(
                WINDOW_WIDTH - 100, 
                reactor.GetRect().GetRightCorner().y)
        )
    );

    Mephi::Plot plotCircle(
        Mephi::Rect(
            Mephi::Vector2d(100, 500),
            Mephi::Vector2d(100 + 400, 500 + 400)
        ),
        1, 1, Mephi::Vector2d(200, 200)
    );

    Mephi::Plot plotSquare(
        Mephi::Rect(
            Mephi::Vector2d(600, 500),
            Mephi::Vector2d(600 + 400, 500 + 400)
        ),
        1, 1, Mephi::Vector2d(200, 200)
    );

    Mephi::Plot plotTemp(
        Mephi::Rect(
            Mephi::Vector2d(1100, 500),
            Mephi::Vector2d(1100 + 400, 500 + 400)
        ),
        1, 1, Mephi::Vector2d(200, 200)
    );

    const Mephi::Vector2d toolboxLC = toolbar.GetRect().GetLeftCorner();
    const Mephi::Vector2d toolboxRC = toolbar.GetRect().GetRightCorner();
    const double          avgX      = (toolboxRC.x + toolboxLC.x) / 2;

    toolbar.AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX - 25,      toolboxLC.y + 25),
            Mephi::Vector2d(avgX - 25 + 50, toolboxLC.y + 25 + 50),
            sf::Color(220, 20, 60)
        ),
        reactor.GetTemp().top,
        10.,
        "top"
    )));

    toolbar.AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX - 112.5,      toolboxLC.y + 125),
            Mephi::Vector2d(avgX - 112.5 + 50, toolboxLC.y + 125 + 50),
            sf::Color(220, 20, 60)
        ),
        reactor.GetTemp().left,
        10.,
        "left"
    )));

    toolbar.AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX + 75,      toolboxLC.y + 125),
            Mephi::Vector2d(avgX + 75 + 50, toolboxLC.y + 125 + 50),
            sf::Color(220, 20, 60)
        ),
        reactor.GetTemp().right,
        10.,
        "right"
    )));

    toolbar.AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX - 25,      toolboxLC.y + 225),
            Mephi::Vector2d(avgX - 25 + 50, toolboxLC.y + 225 + 50),
            sf::Color(220, 20, 60)
        ),
        reactor.GetTemp().bottom,
        10.,
        "bottom"
    )));

    Mephi::MephiManager manager(
        std::move(reactor), 
        std::move(toolbar),
        std::move(plotCircle),
        std::move(plotSquare),
        std::move(plotTemp),
        Mephi::MoleculeManager(),
        MOLECULES_CNT
    );

    sf::Font font;
    if (!font.loadFromFile("./data/Font.ttf")) {
        std::cerr << "Can't load font" << std::endl;
        abort();
    }

    return manager;
}