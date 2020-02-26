#pragma once
#include <GGEngine/Engine/Utilities/Math/MathUtilities.h>
#include <SFML/Graphics.hpp>
#include <vector>

class DisplayLayer {
public:
    enum class Level : uint8_t {
        Bottom,
        Middle,
        Top,
        HUD,
        GUI,
        Count
    };

public:
    DisplayLayer() = default;

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates());

    virtual void display(sf::RenderTarget& target);

private:
    std::vector<std::pair<const sf::Drawable&, sf::RenderStates>> layerElemets;
};
