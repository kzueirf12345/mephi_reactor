#ifndef MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP
#define MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"

namespace Mephi
{

class Plot : public Mephi::Window{
    private:
        double scaleX_;
        double scaleY_;
        Mephi::Vector2d originOffset_;
        std::vector<Mephi::Vector2d> segDots_;
        sf::Color dotColor_;
        double maxModY_;
        double maxModX_;

        Mephi::Vector2d Seg2Pix (const Mephi::Vector2d& segDot) const;
        Mephi::Vector2d Pix2Seg (const Mephi::Vector2d& pixDot) const;

        sf::VertexArray CreateAxis(const bool isX) const ;
        sf::VertexArray CreateGrid(const bool isX) const ;
    public:
        Plot(const Mephi::Rect& rect,double scaleX, double scaleY, 
             const Mephi::Vector2d& OriginOffset, const sf::Color& dotColor = sf::Color::Red)
        : Mephi::Window{rect}, scaleX_{scaleX}, scaleY_{scaleY}, 
          originOffset_{OriginOffset}, segDots_{}, dotColor_{dotColor}, 
          maxModY_{0}, maxModX_(0)
        {}

        Common::Error PushDot(const Mephi::Vector2d& segDot);

        virtual Common::Error Draw(sf::RenderWindow& window) override final;
};
    
}
#endif /*MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP*/
