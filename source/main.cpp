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
#include <cassert>

#include "common/Constants.hpp"
#include "common/ErrorHandle.hpp"
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

static std::unique_ptr<Mephi::Window> CreateGlobWindow();

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

    auto globWindow = CreateGlobWindow();
    if (!globWindow) {
        return EXIT_FAILURE;
    }

    //=======================CYCLE==========================

    while (window.isOpen())
    {
        ERROR_HANDLE(globWindow->HandleEvents(window));

        window.clear();

        ERROR_HANDLE(globWindow->Draw(window));
        ERROR_HANDLE(globWindow->Update());

        window.display();
    }

    return EXIT_SUCCESS;
}

static Common::Error AddPlotScrollBars(Mephi::Plot* plot);

std::unique_ptr<Mephi::Window> CreateGlobWindow() {

// ----------------------- GLOB WINDOW -----------------------

    auto globWindow = std::make_unique<Mephi::Window>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            Mephi::Vector2d(Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT)
        ),
        false
    );

    globWindow->GetFillColor() = Common::TNC::ProgramBackground;

// ----------------------- REACTOR -----------------------

    auto reactor = std::make_unique<Mephi::Reactor> (
        Mephi::Rect(
            Mephi::Vector2d(100, 100),
            Mephi::Vector2d(1000, 350)
        ),
        0.1
    );
    
    auto* reactorPtr = dynamic_cast<Mephi::Reactor*>(globWindow->AddChild(std::move(reactor)));

// ----------------------- PLOTS -----------------------
    
    auto circlePlot = std::make_unique<Mephi::Plot> (
        Mephi::Rect(
            Mephi::Vector2d(100, 500),
            Mephi::Vector2d(300, 300)
        ),
        Mephi::Vector2d(150, 150),
        0,
        [reactorPtr](){ 
            return reactorPtr->GetMoleculeManager().GetCircleCnt();
        }
    );

    auto* circlePlotPtr = dynamic_cast<Mephi::Plot*>(globWindow->AddChild(std::move(circlePlot)));

    if (AddPlotScrollBars(circlePlotPtr)) {
        return nullptr;
    }

    auto squarePlot = std::make_unique<Mephi::Plot>(
        Mephi::Rect(
            Mephi::Vector2d(450, 500),
            Mephi::Vector2d(300, 300)
        ),
        Mephi::Vector2d(150, 150),
        0,
        [reactorPtr]() {
            return reactorPtr->GetMoleculeManager().GetSquareCnt();
        }
    );

    auto* squarePlotPtr = dynamic_cast<Mephi::Plot*>(globWindow->AddChild(std::move(squarePlot)));

    if (AddPlotScrollBars(squarePlotPtr)) {
        return nullptr;
    }

    auto tempPlot = std::make_unique<Mephi::Plot>(
        Mephi::Rect(
            Mephi::Vector2d(800, 500),
            Mephi::Vector2d(300, 300)
        ),
        Mephi::Vector2d(150, 150),
        0,
        [reactorPtr]() {
            return reactorPtr->GetTemp().Average();
        }
    );

    auto* tempPlotPtr = dynamic_cast<Mephi::Plot*>(globWindow->AddChild(std::move(tempPlot)));

    if (AddPlotScrollBars(tempPlotPtr)) {
        return nullptr;
    }

// ----------------------- WALL TEMP BUTTONS -----------------------

    auto buttonPanel = std::make_unique<Mephi::Window> (
        Mephi::Rect(
            Mephi::Vector2d(1200, 100),
            Mephi::Vector2d(450, 700)
        ),
        true
    );

    auto* buttonPanelPtr = globWindow->AddChild(std::move(buttonPanel));

    auto tempLeftButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(25, 100),
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().left,
        100,
        "Left"
    );

    buttonPanelPtr->AddChild(std::move(tempLeftButton));

    auto tempRightButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(325, 100),
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().right,
        100,
        "Right"
    ); 

    buttonPanelPtr->AddChild(std::move(tempRightButton));

    auto tempTopButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(175, 25),
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().top,
        100,
        "Top"
    );

    buttonPanelPtr->AddChild(std::move(tempTopButton));

    auto tempBottomButton = std::make_unique<Mephi::AdjustButton<double>> (
        Mephi::Rect(
            Mephi::Vector2d(175, 175), 
            Mephi::Vector2d(100, 50)
        ),
        &reactorPtr->GetTemp().bottom,
        100,
        "Bottom"
    ); 

    buttonPanelPtr->AddChild(std::move(tempBottomButton));

// ----------------------- MOLECULES BUTTONS -----------------------

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

    buttonPanelPtr->AddChild(std::move(AddMoleculesButton));

    auto DeleteMoleculesButton = std::make_unique<Mephi::DeleteMoleculesButton> (
        Mephi::Rect(
            Mephi::Vector2d(250, 300), 
            Mephi::Vector2d(175, 100)
        ),
        "Delete mol",
        reactorPtr,
        10
    ); 

    buttonPanelPtr->AddChild(std::move(DeleteMoleculesButton));

    auto Clock = std::make_unique<Mephi::Clock>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0), 
            Mephi::Vector2d(200, 200)
        )
    );

    globWindow->AddChild(std::move(Clock));

    return globWindow;
}

Common::Error AddPlotScrollBars(Mephi::Plot* plot) {
    assert(plot);

// ----------------------- SCROLL SCALE X PLOT -----------------------

    auto scrollScaleXPlot = std::make_unique<Mephi::ScrollBar>(
        Mephi::Rect(
            Mephi::Vector2d(0, 0),
            Mephi::Vector2d(plot->GetRect().Width(), 0.1 * plot->GetRect().Height())
        ),
        [plot](double percentage) { return plot->ChangeScaleX(percentage); }
    );

    auto* scrollScaleXPlotPtr = dynamic_cast<Mephi::ScrollBar*>(
        plot->AddChild(std::move(scrollScaleXPlot))
    );

// ----------------------- SCROLL SCALE Y PLOT -----------------------

    auto scrollScaleYPlot = std::make_unique<Mephi::ScrollBar>(
        Mephi::Rect(
            Mephi::Vector2d(0, scrollScaleXPlotPtr->GetRect().Height()),
            Mephi::Vector2d(
                0.1 * plot->GetRect().Width(), 
                plot->GetRect().Height() - scrollScaleXPlotPtr->GetRect().Height()
            )
        ),
        [plot](double percentage) { return plot->ChangeScaleY(percentage); },
        false
    );

    auto* scrollScaleYPlotPtr = dynamic_cast<Mephi::ScrollBar*>(
        plot->AddChild(std::move(scrollScaleYPlot))
    );

// ----------------------- SCROLL VIEW X PLOT -----------------------

    auto scrollViewXPlot = std::make_unique<Mephi::ScrollBar>(
        Mephi::Rect(
            Mephi::Vector2d(
                0.1 * plot->GetRect().Width(), 
                0.9 * plot->GetRect().Height()
            ),
            Mephi::Vector2d(
                0.9 * plot->GetRect().Width(), 
                0.1 * plot->GetRect().Height()
            )
        ),
        [plot](double percentage) { return plot->ChangeViewX(percentage); }
    );

    auto* scrollViewXPlotPtr = dynamic_cast<Mephi::ScrollBar*>(
        plot->AddChild(std::move(scrollViewXPlot))
    );

// ----------------------- SCROLL VIEW Y PLOT -----------------------

    auto scrollViewYPlot = std::make_unique<Mephi::ScrollBar>(
        Mephi::Rect(
            Mephi::Vector2d(
                0.9 * plot->GetRect().Width(), 
                scrollScaleXPlotPtr->GetRect().Height()
            ),
            Mephi::Vector2d(
                0.1 * plot->GetRect().Width(), 
                plot->GetRect().Height() 
                    - scrollScaleXPlotPtr->GetRect().Height()
                    - scrollViewXPlotPtr->GetRect().Height()
            )
        ),
        [plot](double percentage) { return plot->ChangeViewY(percentage); },
        false
    );

    auto* scrollViewYPlotPtr = dynamic_cast<Mephi::ScrollBar*>(
        plot->AddChild(std::move(scrollViewYPlot))
    );


    return Common::Error::SUCCESS;
}