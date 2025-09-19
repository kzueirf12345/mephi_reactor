#ifndef MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP
#define MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP

#include <SFML/Graphics/Rect.hpp>

#include "figures/Rect.hpp"
#include "vector/Vector.hpp"
namespace Mephi
{

class Plot : public Mephi::Rect{
    private:
        double scaleX_;
        double scaleY_;
        Mephi::Vector2d originOffset_;

        Mephi::Vector2d Seg2Pix (const Mephi::Vector2d SegDot) const;
        Mephi::Vector2d Pix2Seg (const Mephi::Vector2d PixDot) const;

        sf::VertexArray CreateAxis(const bool isX) const ;
        sf::VertexArray CreateGrid(const bool isX) const ;
    public:
        Plot(const Mephi::Vector2d&  leftCorner, const Mephi::Vector2d& rightCorner,
             double scaleX, double scaleY, const Mephi::Vector2d& OriginOffset)
        : Mephi::Rect{leftCorner, rightCorner}, scaleX_(scaleX), scaleY_(scaleY), 
          originOffset_(OriginOffset)
        {}

        Common::Error Draw(sf::RenderWindow& Window) const;
};
    
}
#endif /*MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP*/
