#include "Battle.hpp"

#include "World.hpp"

#include "BattleEndedEvent.hpp"
#include "RoundEndedEvent.hpp"

#include <iostream>
#include <random>

Battle::Battle( World& world, std::size_t numRounds )
: m_world( world ),
m_numRounds( numRounds ),
m_round( 0 )
{
}

void Battle::addRobot( Robot* pRobot )
{
    m_robots.push_back( pRobot );
}

void Battle::tick()
{
    if( ended() )
    {
        return;
    }

    if( m_world.getTurn() == 0 )
    {
        ++m_round;
        setupRound();
    }
    
    m_world.tick();

    handleDeadRobots();

    if( m_world.getRobots().size() <= 1 )
    {
        endRound();
        if( ended() )
        {
            endBattle();
        }
    }
}

bool Battle::ended()
{
    return m_world.getRobots().size() <= 1 && m_round >= m_numRounds;
}

void Battle::handleDeadRobots()
{
    for( auto&& pRobot : m_robots )
    {
        if( pRobot->isDead() )
        {
            pRobot->getRobotStatistics().scoreRobotDeath( std::count_if(m_robots.begin(), m_robots.end(),
                [](const Robot* r) { return r->isAlive(); } ) );
        }
    }

    m_world.clearDeadRobots();
}

void Battle::setupRound()
{
    for( auto&& pRobot : m_robots )
    {
        std::size_t retry_count = 0;
        do
        {
            ++retry_count;

            // pick a random position
            std::uniform_int_distribution<> xDist( ( Robot::WIDTH * 2 ), m_world.getWidth() - ( Robot::WIDTH * 2 ) );
            std::uniform_int_distribution<> yDist( ( Robot::HEIGHT * 2 ), m_world.getHeight() - ( Robot::HEIGHT * 2 ) );
            //std::uniform_int_distribution<> xDist( 0, 200 );
            //std::uniform_int_distribution<> yDist( 0, 200 );
            int x = xDist(Utils::getRandom());
            int y = yDist(Utils::getRandom());

            // move robot there
            pRobot->setPosition( x, y );
        } while( pRobot->isCollidingRobot( m_world.getRobots() ) && retry_count < 100 );

        // add it to the world
        m_world.addRobot( pRobot );
    }

    for( auto&& pRobot : m_robots )
    {
        pRobot->reset();
    }
}

void Battle::endRound()
{
    for( auto&& pRobot : m_robots )
    {
        pRobot->addEvent( std::make_unique<RoundEndedEvent>() );
        if (pRobot->isAlive() && !pRobot->isWinner())
        {
            pRobot->getRobotStatistics().scoreLastSurvivor();
            pRobot->setWinner(true);
            std::cout << "SYSTEM: " << pRobot->getNameForEvent( pRobot ) << " wins the round." << std::endl;
            //pRobot->addEvent( std::make_unique<WinEvent>() );
        }
        pRobot->getRobotStatistics().generateTotals();
    }

    m_world.reset();
}

void Battle::endBattle()
{
    for( auto&& pRobot : m_robots )
    {
        pRobot->addEvent( std::make_unique<BattleEndedEvent>( ) );
    }
}