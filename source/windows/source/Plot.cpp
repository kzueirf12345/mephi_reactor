#include "windows/Plot.hpp"
#include "common/ErrorHandle.hpp"
#include "vector/Vector.hpp"
#include "windows/Window.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

Mephi::Plot::Plot(const Mephi::Rect& rect, const Mephi::Vector2d& OriginOffset, 
                  double startXSegVal, TGetYValFoo getYValFoo)
    : Mephi::Window{rect}, 
      scaleX_{1}, scaleY_{1}, 
      originOffset_{OriginOffset}, 
      segDots_{}, 
      dotColor_{Common::TNC::GraphDot}, 
      maxModY_{0}, maxModX_{0}, 
      xSegVal_{startXSegVal}, 
      getYValFoo_{getYValFoo},
      dataMinX_{0}, dataMaxX_{0},
      dataMinY_{0}, dataMaxY_{0},
      manualScaleX_{false}, manualScaleY_{false},
      manualViewX_{false}, manualViewY_{false}
{
    rect_.GetFillColor() = Common::TNC::GraphBackground;
    rect_.GetOutlineColor() = Common::TNC::GraphAxes;
}

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

    if (segDots_.size() == 1) {
        dataMinX_ = dataMaxX_ = segDot.x;
        dataMinY_ = dataMaxY_ = segDot.y;
    } else {
        dataMinX_ = std::min(dataMinX_, segDot.x);
        dataMaxX_ = std::max(dataMaxX_, segDot.x);
        dataMinY_ = std::min(dataMinY_, segDot.y);
        dataMaxY_ = std::max(dataMaxY_, segDot.y);
    }

    double rangeX = std::max(dataMaxX_ - dataMinX_, 1e-6);
    double rangeY = std::max(dataMaxY_ - dataMinY_, 1e-6);

    if (!manualScaleX_) {
        scaleX_ = rect_.Width()  / (rangeX * 1.2);
    }
    if (!manualScaleY_) {
        scaleY_ = rect_.Height() / (rangeY * 1.2);
    }

    if (!manualViewX_) {
        originOffset_.x = -dataMinX_ * scaleX_ + rect_.Width()  * 0.1;
    }
    if (!manualViewY_) {
        originOffset_.y =  dataMaxY_ * scaleY_ + rect_.Height() * 0.1;
    }

    return Common::Error::SUCCESS;
}
Common::Error Mephi::Plot::PushDot(const double ySegVal) {

    ERROR_HANDLE(PushDot(Mephi::Vector2d(xSegVal_, ySegVal)));
    
    ++xSegVal_;

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

    sf::VertexArray pixDots_(sf::PrimitiveType::LinesStrip);

    for (size_t vertexInd = 0; vertexInd < segDots_.size(); ++vertexInd) {
        const sf::Vector2f absPixCoord = static_cast<sf::Vector2f>(Seg2Pix(segDots_[vertexInd]));
        const sf::Vector2f relPixCoord = absPixCoord - static_cast<sf::Vector2f>(AbsoluteCoord());
        if (0 < relPixCoord.x && relPixCoord.x < rect_.Width() 
         && 0 < relPixCoord.y && relPixCoord.y < rect_.Height()) {
            pixDots_.append({absPixCoord, dotColor_});
        }
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

Common::Error Mephi::Plot::ChangeScaleX(double percentage) {
    const double dataRangeX = dataMaxX_ - dataMinX_;
    if (std::abs(percentage - 0.0) < 1e-6 || !dataRangeX) {
        manualScaleX_ = false;
        return Common::Error::SUCCESS;
    }

    manualScaleX_ = true;

    const double scaleToFit = rect_.Width() / (dataRangeX);

    const double minScale = scaleToFit / 2;
    const double maxScale = scaleToFit * 2; 

    scaleX_ = minScale + percentage * (maxScale - minScale);

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Plot::ChangeScaleY(double percentage) {
    const double dataRangeY = dataMaxY_ - dataMinY_;
    if (std::abs(percentage - 0.0) < 1e-6 || !dataRangeY) {
        manualScaleY_ = false;
        return Common::Error::SUCCESS;
    }

    manualScaleY_ = true;

    const double scaleToFit = rect_.Height() / (dataRangeY);

    const double minScale = scaleToFit / 2;
    const double maxScale = scaleToFit * 2; 

    scaleY_ = minScale + percentage * (maxScale - minScale);

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Plot::ChangeViewX(double percentage) {
    const double dataRangeX = dataMaxX_ - dataMinX_;
    if (std::abs(percentage - 0) < 1e-6 || !dataRangeX) {
        manualViewX_ = false;
        return Common::Error::SUCCESS;
    }

    manualViewX_ = true;

    const double maxShiftInSeg = dataRangeX;
    const double targetMinX = dataMinX_ + percentage * maxShiftInSeg;

    originOffset_.x = -targetMinX * scaleX_;

    return Common::Error::SUCCESS;
}

Common::Error Mephi::Plot::ChangeViewY(double percentage) {
    const double dataRangeY = dataMaxY_ - dataMinY_;
    if (std::abs(percentage - 0) < 1e-6 || !dataRangeY) {
        manualViewY_ = false;
        return Common::Error::SUCCESS;
    }
    manualViewY_ = true;

    const double maxShiftInSeg = dataRangeY;
    const double targetMinY = dataMinY_ - (1 + percentage) * maxShiftInSeg;

    originOffset_.y = -targetMinY * scaleY_;

    return Common::Error::SUCCESS;
}