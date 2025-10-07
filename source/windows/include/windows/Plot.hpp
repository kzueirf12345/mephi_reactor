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
        double scaleX_, maxScaleX_, minScaleX_;
        double scaleY_, maxScaleY_, minScaleY_;
        Mephi::Vector2d originOffset_;
        std::vector<Mephi::Vector2d> segDots_;
        sf::Color dotColor_;
        double maxModY_;
        double maxModX_;
        double xSegVal_;

        using TGetYValFoo = std::function<double()>;
        TGetYValFoo getYValFoo_;

        Mephi::Vector2d Seg2Pix (const Mephi::Vector2d& segDot) const;
        Mephi::Vector2d Pix2Seg (const Mephi::Vector2d& pixDot) const;

        sf::VertexArray CreateAxis(const bool isX) const ;
        sf::VertexArray CreateGrid(const bool isX) const ;
    public:
        Plot(const Mephi::Rect& rect, double scaleX, double scaleY, 
             const Mephi::Vector2d& OriginOffset,
             double startXSegVal = 0, TGetYValFoo getYValFoo = NULL)
        : Mephi::Window{rect}, scaleX_{scaleX}, scaleY_{scaleY}, 
          originOffset_{OriginOffset}, segDots_{}, dotColor_{Common::TNC::GraphDot}, 
          maxModY_{0}, maxModX_{0}, xSegVal_{startXSegVal}, getYValFoo_{getYValFoo},
          minScaleX_{0.001}, maxScaleX_{10}, minScaleY_{0.001}, maxScaleY_{10}
        {
            rect_.GetFillColor() = Common::TNC::GraphBackground;
            rect_.GetOutlineColor() = Common::TNC::GraphAxes;
        }

        Common::Error ChangeScaleX(double percentage);
        Common::Error ChangeScaleY(double percentage);

        Common::Error PushDot(const Mephi::Vector2d& segDot);
        Common::Error PushDot(const double ySegVal);

        virtual Common::Error Draw(sf::RenderWindow& window) const override final;
        virtual Common::Error Update() override final;
};
    
}
#endif /*MEPHI_REACTOR_SOURCE_PLOT_INCLUDE_PLOT_PLOT_HPP*/
