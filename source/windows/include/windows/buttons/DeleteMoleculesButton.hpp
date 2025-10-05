#ifndef MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_DELETE_MOLECULES_BUTTON_BUTTON_HPP
#define MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_DELETE_MOLECULES_BUTTON_BUTTON_HPP

#include <cstdlib>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "common/ErrorHandle.hpp"
#include "windows/buttons/Button.hpp"
#include "windows/Reactor.hpp"

namespace Mephi
{

class DeleteMoleculesButton: public Mephi::Button {
    private:
        Mephi::Reactor* reactor_;
        size_t count_;
    public:
        DeleteMoleculesButton(const Mephi::Rect& rect, const std::string& textString,
                           Mephi::Reactor* reactor = nullptr, size_t count = 0,
                           bool isDraggable = false) 
            : Mephi::Button{rect, textString, isDraggable},
              reactor_{reactor}, count_{count}
        {}

        virtual Common::Error Update() override final;
};

}
#endif /*MEPHI_REACTOR_SOURCE_WINDOWS_INCLUDE_WINDOWS_BUTTONS_DELETE_MOLECULES_BUTTON_BUTTON_HPP*/
