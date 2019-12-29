#include "World.hpp"

World::World()
: m_turn( 0 )
{
}

void World::addRobot( Robot* pRobot )
{
    m_robots.push_back( pRobot );
}

std::list<Robot*> World::getRobots()
{
    return m_robots;
}

std::list<Bullet> World::getBullets()
{
    return m_bullets;
}

void World::addBullet( const Bullet& bullet )
{
    m_bullets.push_back( bullet );
}

void World::tick()
{
    ++m_turn;

    for( auto&& bullet : m_bullets )
    {
        bullet.update( m_robots, m_bullets );
    }

    for( auto&& pRobot : m_robots )
    {
        pRobot->tick();
    }

    clearInactiveBullets();
}

void World::clearDeadRobots()
{
    m_robots.erase(
        std::remove_if(m_robots.begin(), m_robots.end(),
            [](const Robot* r) { return r->isDead(); }),
        m_robots.end());
}

void World::clearInactiveBullets()
{
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& b) { return b.getState() == Bullet::INACTIVE; }),
        m_bullets.end());
}

void World::reset()
{
    m_turn = 0;
    m_robots.clear();
    m_bullets.clear();
}