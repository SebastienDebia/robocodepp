#pragma once

#include "Robot.hpp"
#include "Bullet.hpp"

#include <list>

class World
{
public:
    World();

    void addRobot( Robot* pRobot );

    std::list<Robot*> getRobots();
    std::list<Bullet> getBullets();

    unsigned int getWidth() { return 800; }
    unsigned int getHeight() { return 600; }
    std::size_t getTurn() { return m_turn; }

    void addBullet( const Bullet& bullet );

    void tick();

    void clearDeadRobots();
    void clearInactiveBullets();
    void reset();

private:
    std::size_t m_turn;
    std::list<Robot*> m_robots;
    std::list<Bullet> m_bullets;
};