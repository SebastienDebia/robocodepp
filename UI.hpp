#pragma once

#include "World.hpp"

#include <SFML/Graphics.hpp>

class UI
{
public:
    UI( sf::RenderWindow& window, World& world );
    void draw();

protected:
    const sf::Texture& makeTexture( const std::string& part, sf::Color color );

private:
    sf::RenderWindow& m_window;
    World& m_world;

    std::map<std::string, sf::Texture> m_textures;

    sf::Texture m_groundTexture;
    sf::Font m_font;
    std::map<std::size_t, sf::Texture> m_explosionTextures;
};