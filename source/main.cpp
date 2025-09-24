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
#include <memory>
#include <thread>
#include <X11/Xlib.h>
#include <vector>
#include <functional>

#include "common/ErrorHandle.hpp"
#include "molecule/MoleculeManager.hpp"
#include "plot/Plot.hpp"
#include "windows/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"
#include "threads/ThreadManager.hpp"
#include "windows/buttons/AdjustButton.hpp"

// TODO add molecule circle
// TODO add molecule square
// TODO delete also

enum ShareData {
    CIRCLE_CNT = 0,
    SQUARE_CNT,
    AVG_WALLS_TEMP,
    SIZE
};

void ReactorThread(std::vector<Mephi::ThreadManager<double>>& shareDataManagers);
void PlotThread(Mephi::ThreadManager<double>& shareDataManager, const std::string& windowName); // TODO в одном окне

int main()
{
    XInitThreads();

    std::vector<Mephi::ThreadManager<double>> shareDataManagers(ShareData::SIZE);

    std::thread circlePlotThread  (PlotThread, std::ref(shareDataManagers[ShareData::CIRCLE_CNT]),     "Circle Count");
    std::thread squarePlotThread  (PlotThread, std::ref(shareDataManagers[ShareData::SQUARE_CNT]),     "Square Count");
    std::thread avgWallsTempThread(PlotThread, std::ref(shareDataManagers[ShareData::AVG_WALLS_TEMP]), "Average walls temp");
    std::thread reactorThread  (ReactorThread, std::ref(shareDataManagers));
    
    circlePlotThread.join();
    squarePlotThread.join();
    avgWallsTempThread.join();
    reactorThread.join();

    return 0;
}

void ReactorThread(std::vector<Mephi::ThreadManager<double>>& shareDataManagers) {
    constexpr unsigned int WINDOW_WIDTH    = 1720;
    constexpr unsigned int WINDOW_HEIGHT   = 900;
    constexpr unsigned int FRAMERATE_LIMIT = 15;
    constexpr size_t       MOLECULES_CNT   = 1000;
    const sf::Color WINDOW_BG_COLOR(20, 20, 20);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::Reactor reactor(
        Mephi::Rect(
            Mephi::Vector2d(100, 100), 
            Mephi::Vector2d(WINDOW_WIDTH - 500, WINDOW_HEIGHT - 100), 
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

    Mephi::MephiManager manager(
        std::move(reactor), 
        std::move(toolbar),
        Mephi::MoleculeManager(),
        MOLECULES_CNT
    );

    const Mephi::Vector2d toolboxLC = manager.GetToolbox().GetRect().GetLeftCorner();
    const Mephi::Vector2d toolboxRC = manager.GetToolbox().GetRect().GetRightCorner();
    const double          avgX      = (toolboxRC.x + toolboxLC.x) / 2;

    manager.GetToolbox().AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX - 25,      toolboxLC.y + 50),
            Mephi::Vector2d(avgX - 25 + 50, toolboxLC.y + 50 + 50),
            sf::Color(220, 20, 60)
        ),
        manager.GetReactor().GetTemp().top,
        10.
    )));

    manager.GetToolbox().AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX - 112.5,      toolboxLC.y + 150),
            Mephi::Vector2d(avgX - 112.5 + 50, toolboxLC.y + 150 + 50),
            sf::Color(220, 20, 60)
        ),
        manager.GetReactor().GetTemp().left,
        10.
    )));

    manager.GetToolbox().AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX + 75,      toolboxLC.y + 150),
            Mephi::Vector2d(avgX + 75 + 50, toolboxLC.y + 150 + 50),
            sf::Color(220, 20, 60)
        ),
        manager.GetReactor().GetTemp().right,
        10.
    )));

    manager.GetToolbox().AddChild(std::make_unique<Mephi::AdjustButton<double>>(Mephi::AdjustButton<double>(
        Mephi::Rect(
            Mephi::Vector2d(avgX - 25,      toolboxLC.y + 250),
            Mephi::Vector2d(avgX - 25 + 50, toolboxLC.y + 250 + 50),
            sf::Color(220, 20, 60)
        ),
        manager.GetReactor().GetTemp().bottom,
        10.
    )));

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

        shareDataManagers[ShareData::CIRCLE_CNT]    .setData(manager.GetMoleculeManager().GetCircleCnt());
        shareDataManagers[ShareData::SQUARE_CNT]    .setData(manager.GetMoleculeManager().GetSquareCnt());
        shareDataManagers[ShareData::AVG_WALLS_TEMP].setData(manager.GetReactor().GetTemp().Average());

        std::cout << manager.GetReactor().GetTemp().left << std::endl;

        window.display();
    }
    shareDataManagers[ShareData::CIRCLE_CNT].stop();
    shareDataManagers[ShareData::SQUARE_CNT].stop();
    shareDataManagers[ShareData::AVG_WALLS_TEMP].stop();
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