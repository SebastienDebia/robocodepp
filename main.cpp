
#include "UI.hpp"
#include "World.hpp"
#include "Battle.hpp"

#include "testBots/SpinRobot.hpp"
#include "testBots/StaticRobot.hpp"
#include "testBots/VelociRobot.hpp"
#include "testBots/SuperTracker.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Robocode++");

    World world;
    UI ui( window, world );

    SpinRobot r1( world, 400, 340 );
    StaticRobot r2( world, 200, 200, 0, 0, 360 * 1./30 );
    StaticRobot r3( world, 200, 400, 360 * 1./30 );
    StaticRobot r4( world, 400, 300, 360 * 1./30 );
    VelociRobot r5( world, 100, 100 );
    SuperTracker r6( world, 100, 200 );

    Battle battle( world, 10 );

    battle.addRobot( &r1 );
    battle.addRobot( &r3 );
    battle.addRobot( &r2 );
    battle.addRobot( &r4 );
    battle.addRobot( &r5 );
    battle.addRobot( &r6 );

    while (window.isOpen())
    {
        sf::Clock clock;

        sf::Event event;
        if( window.pollEvent(event) )
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        battle.tick();

        ui.draw();
        
        sf::sleep( sf::seconds(1./30) - clock.getElapsedTime() );
        //sf::sleep( sf::seconds(1.) - clock.getElapsedTime() );
    }

    return 0;
}