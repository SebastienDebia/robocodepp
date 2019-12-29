#include "UI.hpp"

#include "Robot.hpp"
#include "HSL.hpp"

#include "tools/makeString.hpp"

#include <sstream>

namespace
{
    void operator +=( sf::Texture& texture, const std::string& file )
    {
        if( !texture.loadFromFile( file ) )
            throw std::logic_error( std::string("Failed to load resource: '") + file + "'" );
    }
    void operator +=( sf::Font& font, const std::string& file )
    {
        if( !font.loadFromFile( file ) )
            throw std::logic_error( std::string("Failed to load resource: '") + file + "'" );
    }

    sf::Image setMainColor( sf::Image& img, const sf::Color& newColor )
    {
        HSL newHsl = TurnToHSL(newColor);

        for( unsigned int x = 0; x < img.getSize().x; ++x )
        {
            for( unsigned int y = 0; y < img.getSize().y; ++y )
            {
                sf::Color originalColor = img.getPixel( x, y );
                HSL hsl = TurnToHSL( originalColor );
                hsl.Hue = newHsl.Hue;
                if( hsl.Saturation > 5 )
                {
                    hsl.Saturation = newHsl.Saturation;
                    hsl.Luminance = ( newHsl.Luminance + hsl.Luminance ) / 2;
                }
                sf::Color newColor = hsl.TurnToRGB();
                newColor.a = originalColor.a;
                img.setPixel( x, y, newColor );
            }
        }

        return img;
    }

    sf::Sprite makeSprite( const sf::Texture& texture, unsigned int x, unsigned int y, int angle )
    {
        sf::Sprite sprite( texture );
        sprite.setOrigin( sf::Vector2f( texture.getSize().x/2, texture.getSize().y/2 ) );
        sprite.setPosition( sf::Vector2f( x, y ) ); // absolute position
        sprite.setRotation( angle );
        return sprite;
    }
}

UI::UI( sf::RenderWindow& window, World& world )
: m_window( window ),
m_world( world )
{
    m_groundTexture += "images/ground/blue_metal/blue_metal_1.png";
    m_font += "fonts/Inconsolata-Regular.ttf";

    for( std::size_t i = 1; i <= 17; ++i )
    {
        m_explosionTextures[i] += tools::makeString() << "images/explosion/explosion1-" << i << ".png";
    }
}

void UI::draw()
{
    m_window.clear();

    for( unsigned int x = 0; x < m_world.getWidth(); x += m_groundTexture.getSize().x )
    {
        for( unsigned int y = 0; y < m_world.getHeight(); y += m_groundTexture.getSize().y )
        {
            sf::Sprite groundSprite( m_groundTexture );

            groundSprite.setPosition( x, y );

            m_window.draw( groundSprite );
        }
    }

    for( auto&& pRobot : m_world.getRobots() )
    {
        auto& robot = *pRobot;

        std::stringstream ss;
        ss << (int) robot.getEnergy();
        sf::Text energyText( ss.str(), m_font, 15 );
        energyText.setPosition( robot.getX() - energyText.getString().getSize() * 15. / 4.,
                                m_world.getHeight() - robot.getY() - Robot::HALF_HEIGHT_OFFSET - 15 - 10 );
        m_window.draw( energyText );
        m_window.draw( makeSprite( makeTexture( "body",   robot.getBodyColor() ),  robot.getX(), m_world.getHeight()-robot.getY(), robot.getAngle() ) );
        m_window.draw( makeSprite( makeTexture( "turret", robot.getGunColor() ),   robot.getX(), m_world.getHeight()-robot.getY(), robot.getTurretAngle() ) );
        m_window.draw( makeSprite( makeTexture( "radar",  robot.getRadarColor() ), robot.getX(), m_world.getHeight()-robot.getY(), robot.getRadarAngle() ) ); 
        sf::Text nameText( robot.getName(), m_font, 15 );
        nameText.setPosition( robot.getX() - nameText.getString().getSize() * 15. / 4.,
                              m_world.getHeight() - robot.getY() + Robot::HALF_HEIGHT_OFFSET + 10 );
        m_window.draw( nameText );

        Arc2D robotScanArc = robot.getScanArc();
        sf::ConvexShape scanArc;
        scanArc.setPointCount(3);
        //scanArc.setPoint( 0, sf::Vector2f(robotScanArc.origin().x(), robotScanArc.origin().y()) );
        //scanArc.setPoint( 1, sf::Vector2f(robotScanArc.origin().x(), robotScanArc.origin().y()) );
        //scanArc.setPoint( 2, sf::Vector2f(robotScanArc.origin().x(), robotScanArc.origin().y()) );
        scanArc.setPoint( 0, sf::Vector2f(robotScanArc.origin().x(), m_world.getHeight() -robotScanArc.origin().y()) );
        scanArc.setPoint( 1, sf::Vector2f(robotScanArc.start().x(), m_world.getHeight() -robotScanArc.start().y()) );
        scanArc.setPoint( 2, sf::Vector2f(robotScanArc.end().x(), m_world.getHeight() -robotScanArc.end().y()) );
        scanArc.setFillColor( sf::Color(150, 50, 250, 42) );
        m_window.draw( scanArc );
    }

    for( auto&& b : m_world.getBullets() )
    {
        auto state = b.getState();
        if(    state == Bullet::EXPLODED
            || state == Bullet::HIT_BULLET
            || state == Bullet::HIT_VICTIM
            || state == Bullet::HIT_WALL )
        {
            auto frame = b.getFrame();
            double scale = std::sqrt( 1000 * b.getPower()) / 128;

            if( frame < 1 || frame > 17 ) // sanity check
                continue;

            sf::Sprite expl( m_explosionTextures[frame] );
            expl.setOrigin( sf::Vector2f( m_explosionTextures[frame].getSize().x/2, m_explosionTextures[frame].getSize().y/2 ) );
            expl.setPosition( b.getX(), m_world.getHeight()-b.getY() );
            expl.setScale( scale, scale );
            m_window.draw( expl );
        }
        else
        {
            sf::CircleShape bullet( b.getPower() );

            bullet.setFillColor( b.getColor() );
            bullet.setPosition( b.getX(), m_world.getHeight()-b.getY() );
            
            m_window.draw( bullet );
        }
    }

    m_window.display();
}

const sf::Texture& UI::makeTexture( const std::string& part, sf::Color color )
{
    std::stringstream ss;
    ss << part << color.toInteger();
    std::string key = ss.str();

    auto it = m_textures.find( key );
    if( it == m_textures.end() )
    {
        sf::Image img;
        img.loadFromFile( std::string("images/") + part + ".png" );
        setMainColor( img, color );
        sf::Texture texture;
        texture.loadFromImage( img );
        m_textures[key] = texture;
    }

    return m_textures[key];
}