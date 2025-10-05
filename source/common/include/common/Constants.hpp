#ifndef MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_CONSTANTS_HPP
#define MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_CONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>

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
}

namespace TNC = TokyoNightColors;

}

#endif /*MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_CONSTANTS_HPP*/