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
#include <vector>
#include <functional>

#include "common/ErrorHandle.hpp"
#include "molecule/MoleculeManager.hpp"
#include "plot/Plot.hpp"
#include "reactor/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"
#include "threads/ThreadManager.hpp"

enum ShareData {
    CIRCLE_CNT = 0,
    SQUARE_CNT,
    SIZE
};

void ReactorThread(std::vector<Mephi::ThreadManager<double>>& shareDataManagers);
void PlotThread(Mephi::ThreadManager<double>& shareDataManager, const std::string& windowName);

int main()
{
    XInitThreads();

    std::vector<Mephi::ThreadManager<double>> shareDataManagers(ShareData::SIZE);

    std::thread circlePlotThread1(PlotThread, std::ref(shareDataManagers[ShareData::CIRCLE_CNT]), "Circle Count");
    std::thread squarePlotThread1(PlotThread, std::ref(shareDataManagers[ShareData::SQUARE_CNT]), "Square Count");
    std::thread reactorThread2(ReactorThread, std::ref(shareDataManagers));
    
    circlePlotThread1.join();
    squarePlotThread1.join();
    reactorThread2.join();

    return 0;
}

void ReactorThread(std::vector<Mephi::ThreadManager<double>>& shareDataManagers) {
    constexpr unsigned int WINDOW_WIDTH    = 1920;
    constexpr unsigned int WINDOW_HEIGHT   = 1200;
    constexpr unsigned int FRAMERATE_LIMIT = 15;
    constexpr size_t       MOLECULES_CNT   = 1000;
    const sf::Color WINDOW_BG_COLOR(20, 20, 20);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::MoleculeManager moleculeManager = Mephi::MoleculeManager();

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
        Mephi::MoleculeManager(),
        MOLECULES_CNT
    );

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

        shareDataManagers[ShareData::CIRCLE_CNT].setData(manager.GetMoleculeManager().GetCircleCnt());
        shareDataManagers[ShareData::SQUARE_CNT].setData(manager.GetMoleculeManager().GetSquareCnt());

        window.display();
    }
    shareDataManagers[ShareData::CIRCLE_CNT].stop();
    shareDataManagers[ShareData::SQUARE_CNT].stop();
}

void PlotThread(Mephi::ThreadManager<double>& shareDataManager, const std::string& windowName) {
    constexpr unsigned int WINDOW_WIDTH    = 700;
    constexpr unsigned int WINDOW_HEIGHT   = 700;
    constexpr unsigned int FRAMERATE_LIMIT = 60;
    constexpr double       PLOT_DELAY_ITER = 40;
    const sf::Color WINDOW_BG_COLOR(20, 20, 20);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        windowName
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::Plot plot(
        Mephi::Vector2d(100, 100),
        Mephi::Vector2d(500, 500),
        1,
        1,
        Mephi::Vector2d(200, 200)
    );

    double curValX = -PLOT_DELAY_ITER;

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
        
        double curValY;
        if (shareDataManager.getData(curValY) && curValX > 0) {
            plot.PushDot(Mephi::Vector2d(curValX, curValY));
        }

        plot.Draw(window);

        window.display();

        ++curValX;
    }
}