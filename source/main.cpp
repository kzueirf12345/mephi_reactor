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
#include "molecule/Molecule.hpp"
#include "reactor/Reactor.hpp"
#include "vector/Vector.hpp"
#include "figures/Rect.hpp"
#include "mephi/MephiManager.hpp"
#include "molecule/MoleculeCircle.hpp"
#include "molecule/MoleculeSquare.hpp"

// #include <random>

// namespace Mephi
// {

// void GenerateMolecules(MephiManager& manager, size_t count)
// {
//     std::random_device rd;
//     std::mt19937 gen(rd());
    
//     std::uniform_real_distribution<double> distX(manager.GetReactor().GetLeftCorner().x, manager.GetReactor().GetRightCorner().x);
//     std::uniform_real_distribution<double> distY(manager.GetReactor().GetLeftCorner().y, manager.GetReactor().GetRightCorner().y);
    
//     std::uniform_real_distribution<double> distSpeed(0, 10);
//     std::uniform_real_distribution<double> distAngle(0.0, 2.0 * M_PI);
    
//     for (size_t i = 0; i < count; ++i)
//     {
//         const Mephi::Vector2d randomCoord(distX(gen), distY(gen));
        
//         const double speed = distSpeed(gen);
//         const double angle = distAngle(gen);
//         const Mephi::Vector2d randomSpeed(speed * std::cos(angle), speed * std::sin(angle));
        
//         const Mephi::MoleculeCircle molecule(randomCoord, randomSpeed, sf::Color::Red);
        
//         manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule));
//     }
// }

// }

int main()
{
    constexpr unsigned int WINDOW_WIDTH         = 2048;
    constexpr unsigned int WINDOW_HEIGHT        = 1024;

    constexpr unsigned int FRAMERATE_LIMIT      = 30;

    const sf::Color WINDOW_BG_COLOR(240, 240, 240);

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Mephi reactor!!! (I eto glavni factor, i mi tam hranim edu)"
    );

    window.setFramerateLimit(FRAMERATE_LIMIT);

    Mephi::MephiManager manager(
        Mephi::Reactor (
            Mephi::Rect(
                Mephi::Vector2d(100, 100), 
                Mephi::Vector2d(600, 600), 
                sf::Color::Cyan, 
                sf::Color::Black, 
                5
            )
        ), 
        {}
    );

    // Mephi::MoleculeCircle molecule1(Mephi::Vector2d(150, 150), Mephi::Vector2d(-1.5, 1.5), sf::Color::Red);
    // Mephi::MoleculeCircle molecule2(Mephi::Vector2d(250, 250), Mephi::Vector2d(2.0, -1.0), sf::Color::Red);
    // Mephi::MoleculeCircle molecule3(Mephi::Vector2d(350, 350), Mephi::Vector2d(-1.0, -2.0), sf::Color::Red);
    // Mephi::MoleculeCircle molecule4(Mephi::Vector2d(450, 450), Mephi::Vector2d(2.5, 0.5), sf::Color::Red);
    // Mephi::MoleculeCircle molecule5(Mephi::Vector2d(550, 550), Mephi::Vector2d(-0.5, -2.5), sf::Color::Red);

    // Mephi::MoleculeCircle molecule6(Mephi::Vector2d(200, 300), Mephi::Vector2d(1.8, -1.2), sf::Color::Red);
    // Mephi::MoleculeCircle molecule7(Mephi::Vector2d(300, 400), Mephi::Vector2d(-2.2, 0.8), sf::Color::Red);
    // Mephi::MoleculeCircle molecule8(Mephi::Vector2d(400, 500), Mephi::Vector2d(1.2, 2.2), sf::Color::Red);
    // Mephi::MoleculeCircle molecule9(Mephi::Vector2d(500, 200), Mephi::Vector2d(-0.8, -1.8), sf::Color::Red);
    // Mephi::MoleculeCircle molecule10(Mephi::Vector2d(300, 150), Mephi::Vector2d(2.8, 0.2), sf::Color::Red);

    // Mephi::MoleculeCircle molecule11(Mephi::Vector2d(180, 220), Mephi::Vector2d(-2.5, 1.0), sf::Color::Red);
    // Mephi::MoleculeCircle molecule12(Mephi::Vector2d(280, 320), Mephi::Vector2d(1.5, -2.0), sf::Color::Red);
    // Mephi::MoleculeCircle molecule13(Mephi::Vector2d(380, 420), Mephi::Vector2d(-1.2, -1.5), sf::Color::Red);
    // Mephi::MoleculeCircle molecule14(Mephi::Vector2d(480, 180), Mephi::Vector2d(2.0, 1.8), sf::Color::Red);
    // Mephi::MoleculeCircle molecule15(Mephi::Vector2d(220, 380), Mephi::Vector2d(-0.2, -2.8), sf::Color::Red);

    // Mephi::MoleculeCircle molecule16(Mephi::Vector2d(320, 280), Mephi::Vector2d(1.0, 2.5), sf::Color::Red);
    // Mephi::MoleculeCircle molecule17(Mephi::Vector2d(420, 380), Mephi::Vector2d(-2.8, -0.5), sf::Color::Red);
    // Mephi::MoleculeCircle molecule18(Mephi::Vector2d(280, 480), Mephi::Vector2d(0.5, -1.2), sf::Color::Red);
    // Mephi::MoleculeCircle molecule19(Mephi::Vector2d(380, 180), Mephi::Vector2d(-1.8, 2.2), sf::Color::Red);
    // Mephi::MoleculeCircle molecule20(Mephi::Vector2d(480, 280), Mephi::Vector2d(2.2, -0.8), sf::Color::Red);

    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule1));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule2));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule3));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule4));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule5));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule6));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule7));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule8));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule9));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule10));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule11));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule12));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule13));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule14));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule15));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule16));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule17));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule18));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule19));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule20));

    // Mephi::MoleculeSquare molecule1(Mephi::Vector2d(150, 150), Mephi::Vector2d(-1.5, 1.5), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule2(Mephi::Vector2d(250, 250), Mephi::Vector2d(2.0, -1.0), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule3(Mephi::Vector2d(350, 350), Mephi::Vector2d(-1.0, -2.0), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule4(Mephi::Vector2d(450, 450), Mephi::Vector2d(2.5, 0.5), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule5(Mephi::Vector2d(550, 550), Mephi::Vector2d(-0.5, -2.5), 2, sf::Color::Blue);

    // Mephi::MoleculeSquare molecule6(Mephi::Vector2d(200, 300), Mephi::Vector2d(1.8, -1.2), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule7(Mephi::Vector2d(300, 400), Mephi::Vector2d(-2.2, 0.8), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule8(Mephi::Vector2d(400, 500), Mephi::Vector2d(1.2, 2.2), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule9(Mephi::Vector2d(500, 200), Mephi::Vector2d(-0.8, -1.8), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule10(Mephi::Vector2d(300, 150), Mephi::Vector2d(2.8, 0.2), 2, sf::Color::Blue);

    // Mephi::MoleculeSquare molecule11(Mephi::Vector2d(180, 220), Mephi::Vector2d(-2.5, 1.0), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule12(Mephi::Vector2d(280, 320), Mephi::Vector2d(1.5, -2.0), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule13(Mephi::Vector2d(380, 420), Mephi::Vector2d(-1.2, -1.5), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule14(Mephi::Vector2d(480, 180), Mephi::Vector2d(2.0, 1.8), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule15(Mephi::Vector2d(220, 380), Mephi::Vector2d(-0.2, -2.8), 2, sf::Color::Blue);

    // Mephi::MoleculeSquare molecule16(Mephi::Vector2d(320, 280), Mephi::Vector2d(1.0, 2.5), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule17(Mephi::Vector2d(420, 380), Mephi::Vector2d(-2.8, -0.5), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule18(Mephi::Vector2d(280, 480), Mephi::Vector2d(0.5, -1.2), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule19(Mephi::Vector2d(380, 180), Mephi::Vector2d(-1.8, 2.2), 2, sf::Color::Blue);
    // Mephi::MoleculeSquare molecule20(Mephi::Vector2d(480, 280), Mephi::Vector2d(2.2, -0.8), 2, sf::Color::Blue);

    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule1));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule2));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule3));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule4));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule5));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule6));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule7));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule8));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule9));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule10));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule11));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule12));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule13));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule14));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule15));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule16));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule17));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule18));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule19));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule20));

    ERROR_HANDLE(manager.GenerateMolecules(40));

    // Mephi::MoleculeCircle molecule1(Mephi::Vector2d(200, 200), Mephi::Vector2d(-15, 15), 1, sf::Color::Red, 10);
    // Mephi::MoleculeSquare molecule2(Mephi::Vector2d(250, 250), Mephi::Vector2d(-10, 10), 1, sf::Color::Blue, 10);

    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeCircle>(molecule1));
    // manager.GetMolecules().push_back(std::make_unique<Mephi::MoleculeSquare>(molecule2));

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