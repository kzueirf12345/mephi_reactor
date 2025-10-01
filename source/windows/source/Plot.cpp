#include "windows/Plot.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

Mephi::Vector2d Mephi::Plot::Seg2Pix(const Mephi::Vector2d& segDot) const {
    return Mephi::Vector2d(segDot.x * scaleX_, -segDot.y * scaleY_) 
         + originOffset_ 
         + AbsoluteCoord();
}

Mephi::Vector2d Mephi::Plot::Pix2Seg(const Mephi::Vector2d& pixDot) const {
    return Mephi::Vector2d(
        (pixDot.x - originOffset_.x) / scaleX_, 
        (pixDot.y - originOffset_.y) / scaleY_
    );
}

Common::Error Mephi::Plot::PushDot(const Mephi::Vector2d& segDot) {
    segDots_.push_back(segDot);
    maxModY_ = std::max(maxModY_, std::abs(segDot.y));
    maxModX_ = std::max(maxModX_, std::abs(segDot.x));

    scaleY_ = rect_.Height() / (2.5 * maxModY_);
    scaleX_ = rect_.Width() / (2.5 * maxModX_);

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Plot::PushDot(const double ySegVal) {
    segDots_.push_back({xSegVal_, ySegVal});
    ++xSegVal_;

    maxModY_ = std::max(maxModY_, std::abs(ySegVal));
    maxModX_ = std::max(maxModX_, std::abs(xSegVal_));

    scaleY_ = rect_.Height() / (2.5 * maxModY_);
    scaleX_ = rect_.Width() / (2.5 * maxModX_);

    return Common::Error::SUCCESS;
}

sf::VertexArray Mephi::Plot::CreateAxis(const bool IsX) const {
    sf::VertexArray Axis(sf::PrimitiveType::Lines, 2);

    Axis[0].color = Axis[1].color = rect_.GetOutlineColor();

    const Mephi::Vector2d absLeftCorner  = AbsoluteCoord();
    const Mephi::Vector2d absRightCorner = absLeftCorner + Mephi::Vector2d(rect_.Width(), rect_.Height());

    if (IsX) {
        Axis[0].position = sf::Vector2f(absLeftCorner.x,  absLeftCorner.y + originOffset_.y);
        Axis[1].position = sf::Vector2f(absRightCorner.x, absLeftCorner.y + originOffset_.y);
    } else {
        Axis[0].position = sf::Vector2f(absLeftCorner.x + originOffset_.x, absLeftCorner.y);
        Axis[1].position = sf::Vector2f(absLeftCorner.x + originOffset_.x, absRightCorner.y);
    }

    return Axis;
}

Common::Error Mephi::Plot::Draw(sf::RenderWindow& window) const {

    ERROR_HANDLE(Mephi::Window::Draw(window));
    
    const sf::VertexArray xAxis = CreateAxis(true);
    const sf::VertexArray yAxis = CreateAxis(false);

    if (rect_.GetLeftCorner().y < xAxis[0].position.y && xAxis[0].position.y < rect_.GetRightCorner().y) {
        window.draw(xAxis);
    }
    if (rect_.GetLeftCorner().x < yAxis[0].position.x && yAxis[0].position.x < rect_.GetRightCorner().x) {
        window.draw(yAxis);
    }
    
    window.draw(CreateGrid(true ));
    window.draw(CreateGrid(false));

    sf::VertexArray pixDots_(sf::PrimitiveType::LinesStrip, segDots_.size());

    for (size_t vertexInd = 0; vertexInd < segDots_.size(); ++vertexInd) {
        pixDots_[vertexInd] = {
            static_cast<sf::Vector2f>(Seg2Pix(segDots_[vertexInd])),
            dotColor_
        };
    }
    
    window.draw(pixDots_);

    return Common::Error::SUCCESS;
}

sf::VertexArray Mephi::Plot::CreateGrid(const bool IsX) const
{
    constexpr sf::Uint8 GRID_LINE_OPACITY = 25;
    const sf::Color GridLineColor(
        rect_.GetOutlineColor().r, 
        rect_.GetOutlineColor().g, 
        rect_.GetOutlineColor().b, 
        GRID_LINE_OPACITY
    );

    const Mephi::Vector2d absLeftCorner  = AbsoluteCoord();
    const Mephi::Vector2d absRightCorner = absLeftCorner + Mephi::Vector2d(rect_.Width(), rect_.Height());

    if (IsX) {
        const size_t GridLinesCnt = rect_.Height() / scaleY_ + 1;

        sf::VertexArray Grid(sf::PrimitiveType::Lines, 2 * GridLinesCnt);

        for (size_t LineNum = 0; LineNum < GridLinesCnt; ++LineNum) {
            Grid[2 * LineNum].color = Grid[2 * LineNum + 1].color = GridLineColor;

            Grid[2 * LineNum]    .position.x = absLeftCorner.x;
            Grid[2 * LineNum + 1].position.x = absRightCorner.x;

            Grid[2 * LineNum]    .position.y = absLeftCorner.y + scaleY_ * LineNum;
            Grid[2 * LineNum + 1].position.y = Grid[2 * LineNum].position.y;
        }

        return Grid;
    } else {
        const size_t GridLinesCnt = rect_.Width() / scaleX_ + 1;

        sf::VertexArray Grid(sf::PrimitiveType::Lines, 2 * GridLinesCnt);

        for (size_t LineNum = 0; LineNum < GridLinesCnt; ++LineNum) {
            Grid[2 * LineNum].color = Grid[2 * LineNum + 1].color = GridLineColor;

            Grid[2 * LineNum]    .position.y = absLeftCorner.y;
            Grid[2 * LineNum + 1].position.y = absRightCorner.y;

            Grid[2 * LineNum].    position.x = absLeftCorner.x + scaleX_ * LineNum;
            Grid[2 * LineNum + 1].position.x = Grid[2 * LineNum].position.x;
        }

        return Grid;
    }
}

Common::Error Mephi::Plot::Update() {
    ERROR_HANDLE(Mephi::Window::Update());

    ERROR_HANDLE(PushDot(getYValFoo_()));
    
    return Common::Error::SUCCESS;
}