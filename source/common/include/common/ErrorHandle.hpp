#ifndef MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_ERROR_HANDLE_HPP
#define MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_ERROR_HANDLE_HPP

#include <string>
#include <iostream>
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

#include "Concole.hpp"

namespace Common
{

enum Error {
    SUCCESS          = 0,
    STANDART_ERRNO   = 1,
    UNKNOWN_ENUM_ARG = 2
};

std::string ErrorToString(const enum Common::Error Error);

#define ERROR_HANDLE(CallFunc, ...)                                                                 \
    do {                                                                                            \
        Common::Error Error = CallFunc;                                                             \
        if (Error)                                                                                  \
        {                                                                                           \
            std::cerr << RED_TEXT("!!!         ERROR!!! ")  #CallFunc  YELLOW_TEXT(" ||| ");        \
            std::cerr << RED_TEXT("Type: ") << Common::ErrorToString(Error) << std::endl;           \
            __VA_ARGS__                                                                             \
            return Error;                                                                           \
        }                                                                                           \
    } while(0)

#define STANDART_ERROR_HANDLE(CallFunc, ...)                                                        \
    do {                                                                                            \
        int Error = CallFunc;                                                                       \
        if (Error)                                                                                  \
        {                                                                                           \
            std::cerr << RED_TEXT("!!!STANDART ERROR!!! ")  #CallFunc  YELLOW_TEXT(" ||| ");        \
            std::cerr << RED_TEXT("Type: ") << Common::ErrorToString(Error) << std::endl;           \
            __VA_ARGS__                                                                             \
            return Error;                                                                           \
        }                                                                                           \
    } while(0)


// #define SFML_ERROR_HANDLE(CallFunc, ...)                                                            \
//     do {                                                                                            \
//         sf:: Error = CallFunc;                                                                       \
//         if (Error)                                                                                  \
//         {                                                                                           \
//             std::cerr << RED_TEXT("!!!   SFML ERROR!!! ")  #CallFunc  YELLOW_TEXT(" ||| ");         \
//             std::cerr << RED_TEXT("Type: ") << Common::ErrorToString(Error) << std::endl;           \
//             __VA_ARGS__                                                                             \
//             return Error;                                                                           \
//         }                                                                                           \
//     } while(0)

}

#endif /*MEPHI_REACTOR_SOURCE_COMMON_INCLUDE_COMMON_ERROR_HANDLE_HPP*/