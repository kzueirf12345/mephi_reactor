#ifndef MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_CONSTANTS_HPP
#define MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_CONSTANTS_HPP

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

namespace Common 
{

namespace TokyoNightColors {
    static const sf::Color WindowBackground(0x1a, 0x1b, 0x26);
    static const sf::Color WindowBorder(0x41, 0x42, 0x5a);

    static const sf::Color ProgramBackground(0x16, 0x17, 0x20);
    
    static const sf::Color ButtonIdle(0x1a, 0x1b, 0x26);
    static const sf::Color ButtonPressed(0x2a, 0x2a, 0x3e);
    static const sf::Color BorderHovered(0x7a, 0xaa, 0xf2);
    static const sf::Color BorderIdle(0x41, 0x42, 0x5a);
    
    static const sf::Color ReactorBackground(0x14, 0x15, 0x1e);
    static const sf::Color MoleculeDebug(0xff, 0x00, 0xff);
    static const sf::Color MoleculeCircle(0xf7, 0x76, 0x8e);
    static const sf::Color MoleculeSquare(0xbb, 0x9a, 0xf7);
    
    static const sf::Color GraphBackground(0x1a, 0x1b, 0x26);
    static const sf::Color GraphDot(0x9e, 0xce, 0x6a);
    static const sf::Color GraphAxes(0x54, 0x57, 0x82);
    
    static const sf::Color ClockBackground(0x1a, 0x1b, 0x26);
    static const sf::Color ClockHands(0xc0, 0xca, 0xf5);
    static const sf::Color ClockDots(0xbb, 0x9a, 0xf7);
    
    static const sf::Color AccentGreen(0x9e, 0xce, 0x6a);
    static const sf::Color AccentBlue(0x7a, 0xaa, 0xf2);
    static const sf::Color AccentPurple(0xbb, 0x9a, 0xf7);
    static const sf::Color AccentPink(0xf7, 0x76, 0x8e);
    static const sf::Color TextPrimary(0xc0, 0xca, 0xf5);
    static const sf::Color TextSecondary(0xa9, 0xb1, 0xd6);

    static const sf::Color ScrollbarBackground(0x24, 0x25, 0x38);
}

namespace TNC = TokyoNightColors;

extern sf::Font GLOBAL_FONT;

constexpr unsigned int WINDOW_WIDTH    = 1720;
constexpr unsigned int WINDOW_HEIGHT   = 900;
constexpr unsigned int FRAMERATE_LIMIT = 20;

constexpr double MAX_MOLECULE_SPEED = 10;

}

#endif /*MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_CONSTANTS_HPP*/