#include "windows/Plot.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

Mephi::Vector2d Mephi::Plot::Seg2Pix(const Mephi::Vector2d& segDot) const {
    return Mephi::Vector2d(Mephi::Vector2d(segDot.x * scaleX_, -segDot.y * scaleY_) + originOffset_ + rect_.GetLeftCorner());
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

sf::VertexArray Mephi::Plot::CreateAxis(const bool IsX) const {
    sf::VertexArray Axis(sf::PrimitiveType::Lines, 2);

    Axis[0].color = Axis[1].color = rect_.GetOutlineColor();

    if (IsX) {
        Axis[0].position = sf::Vector2f(rect_.GetLeftCorner().x,  rect_.GetLeftCorner().y + originOffset_.y);
        Axis[1].position = sf::Vector2f(rect_.GetRightCorner().x, rect_.GetLeftCorner().y + originOffset_.y);
    } else {
        Axis[0].position = sf::Vector2f(rect_.GetLeftCorner().x + originOffset_.x, rect_.GetLeftCorner().y);
        Axis[1].position = sf::Vector2f(rect_.GetLeftCorner().x + originOffset_.x, rect_.GetRightCorner().y);
    }

    return Axis;
}

Common::Error Mephi::Plot::Draw(sf::RenderWindow& window) {

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

    if (IsX) {
        const size_t GridLinesCnt = (rect_.GetRightCorner().y - rect_.GetLeftCorner().y) / scaleY_ + 1;

        sf::VertexArray Grid(sf::PrimitiveType::Lines, 2 * GridLinesCnt);

        for (size_t LineNum = 0; LineNum < GridLinesCnt; ++LineNum) {
            Grid[2 * LineNum].color = Grid[2 * LineNum + 1].color = GridLineColor;

            Grid[2 * LineNum]    .position.x = rect_.GetLeftCorner().x;
            Grid[2 * LineNum + 1].position.x = rect_.GetRightCorner().x;

            Grid[2 * LineNum]    .position.y = rect_.GetLeftCorner().y + scaleY_ * LineNum;
            Grid[2 * LineNum + 1].position.y = Grid[2 * LineNum].position.y;
        }

        return Grid;
    } else {
        const size_t GridLinesCnt = (rect_.GetRightCorner().x - rect_.GetLeftCorner().x) / scaleX_ + 1;

        sf::VertexArray Grid(sf::PrimitiveType::Lines, 2 * GridLinesCnt);

        for (size_t LineNum = 0; LineNum < GridLinesCnt; ++LineNum) {
            Grid[2 * LineNum].color = Grid[2 * LineNum + 1].color = GridLineColor;

            Grid[2 * LineNum]    .position.y = rect_.GetLeftCorner().y;
            Grid[2 * LineNum + 1].position.y = rect_.GetRightCorner().y;

            Grid[2 * LineNum].    position.x = rect_.GetLeftCorner().x + scaleX_ * LineNum;
            Grid[2 * LineNum + 1].position.x = Grid[2 * LineNum].position.x;
        }

        return Grid;
    }
}