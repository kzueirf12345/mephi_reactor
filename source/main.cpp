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
#include <cstddef>
#include <cstdlib>
#include <thread>
#include <X11/Xlib.h>

#include "common/ErrorHandle.hpp"
#include "plot/Plot.hpp"
#include "reactor/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"
#include "threads/ThreadManager.hpp"

Mephi::ThreadManager<size_t> ShareCircleCntManager;

void ReactorThread();
void PlotThread();

int main()
{
    XInitThreads();

    std::thread t1(PlotThread);
    std::thread t2(ReactorThread);
    
    t1.join();
    t2.join();

    return 0;
}

void ReactorThread() {
    constexpr unsigned int WINDOW_WIDTH    = 1920;
    constexpr unsigned int WINDOW_HEIGHT   = 1200;
    constexpr unsigned int FRAMERATE_LIMIT = 15;
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

    manager.GenerateMolecules(100);

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
        manager.Update();

        ShareCircleCntManager.setData(manager.GetCircleCnt());
        

        window.display();
    }
    ShareCircleCntManager.stop();
}

void PlotThread() {
    constexpr unsigned int WINDOW_WIDTH    = 700;
    constexpr unsigned int WINDOW_HEIGHT   = 700;
    constexpr unsigned int FRAMERATE_LIMIT = 60;
    const sf::Color WINDOW_BG_COLOR(20, 20, 20);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Plot"
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::Plot plot(
        Mephi::Vector2d(100, 100),
        Mephi::Vector2d(500, 500),
        1,
        1,
        Mephi::Vector2d(200, 200)
    );

    double ind = 0;

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
        
        size_t curCircleCnt;
        if (ShareCircleCntManager.getData(curCircleCnt)) {
            plot.PushDot(Mephi::Vector2d(ind, curCircleCnt));
        }

        plot.Draw(window);

        window.display();

        ++ind;
    }
}