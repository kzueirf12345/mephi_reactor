#include "plot/Plot.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include <SFML/Graphics/Vertex.hpp>

Mephi::Vector2d Mephi::Plot::Seg2Pix(const Mephi::Vector2d& segDot) const {
    return Mephi::Vector2d(Mephi::Vector2d(segDot.x * scaleX_, -segDot.y * scaleY_) + originOffset_ + leftCorner_);
}

Mephi::Vector2d Mephi::Plot::Pix2Seg(const Mephi::Vector2d& pixDot) const {
    return Mephi::Vector2d(
        (pixDot.x - originOffset_.x) / scaleX_, 
        (pixDot.y - originOffset_.y) / scaleY_
    );
}

Common::Error Mephi::Plot::PushDot(const Mephi::Vector2d& segDot) {
    dots_.append(static_cast<sf::Vector2f>(Seg2Pix(segDot)));
    return Common::Error::SUCCESS;
}

sf::VertexArray Mephi::Plot::CreateAxis(const bool IsX) const
{
    sf::VertexArray Axis(sf::PrimitiveType::Lines, 2);

    Axis[0].color = Axis[1].color = outlineColor_;

    if (IsX) {
        Axis[0].position = sf::Vector2f(leftCorner_.x,  leftCorner_.y + originOffset_.y);
        Axis[1].position = sf::Vector2f(rightCorner_.x, leftCorner_.y + originOffset_.y);
    } else {
        Axis[0].position = sf::Vector2f(leftCorner_.x + originOffset_.x, leftCorner_.y);
        Axis[1].position = sf::Vector2f(leftCorner_.x + originOffset_.x, rightCorner_.y);
    }

    return Axis;
}

Common::Error Mephi::Plot::Draw(sf::RenderWindow& window) const {

    window.draw(GetSFRect());

    const sf::VertexArray xAxis = CreateAxis(true);
    const sf::VertexArray yAxis = CreateAxis(false);

    if (leftCorner_.y < xAxis[0].position.y && xAxis[0].position.y < rightCorner_.y) {
        window.draw(xAxis);
    }
    if (leftCorner_.x < yAxis[0].position.x && yAxis[0].position.x < rightCorner_.x) {
        window.draw(yAxis);
    }

    window.draw(CreateGrid(true ));
    window.draw(CreateGrid(false));

    window.draw(dots_);

    return Common::Error::SUCCESS;
}

sf::VertexArray Mephi::Plot::CreateGrid(const bool IsX) const
{
    constexpr sf::Uint8 GRID_LINE_OPACITY = 25;
    const sf::Color GridLineColor(
        outlineColor_.r, 
        outlineColor_.g, 
        outlineColor_.b, 
        GRID_LINE_OPACITY
    );

    if (IsX) {
        const size_t GridLinesCnt = (rightCorner_.y - leftCorner_.y) / scaleY_ + 1;

        sf::VertexArray Grid(sf::PrimitiveType::Lines, 2 * GridLinesCnt);

        for (size_t LineNum = 0; LineNum < GridLinesCnt; ++LineNum) {
            Grid[2 * LineNum].color = Grid[2 * LineNum + 1].color = GridLineColor;

            Grid[2 * LineNum]    .position.x = leftCorner_.x;
            Grid[2 * LineNum + 1].position.x = rightCorner_.x;

            Grid[2 * LineNum]    .position.y = leftCorner_.y + scaleY_ * LineNum;
            Grid[2 * LineNum + 1].position.y = Grid[2 * LineNum].position.y;
        }

        return Grid;
    } else {
        const size_t GridLinesCnt = (rightCorner_.x - leftCorner_.x) / scaleX_ + 1;

        sf::VertexArray Grid(sf::PrimitiveType::Lines, 2 * GridLinesCnt);

        for (size_t LineNum = 0; LineNum < GridLinesCnt; ++LineNum) {
            Grid[2 * LineNum].color = Grid[2 * LineNum + 1].color = GridLineColor;

            Grid[2 * LineNum]    .position.y = leftCorner_.y;
            Grid[2 * LineNum + 1].position.y = rightCorner_.y;

            Grid[2 * LineNum].    position.x = leftCorner_.x + scaleX_ * LineNum;
            Grid[2 * LineNum + 1].position.x = Grid[2 * LineNum].position.x;
        }

        return Grid;
    }
}