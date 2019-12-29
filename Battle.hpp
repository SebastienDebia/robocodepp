#pragma once

#include "Robot.hpp"

class World;

class Battle
{
public:
    Battle( World& world, std::size_t numRounds );

    void addRobot( Robot* pRobot );
    void tick();

protected:
    bool ended();
    void handleDeadRobots();
    void setupRound();
    void endRound();
    void endBattle();

private:
    World& m_world;
    std::list<Robot*> m_robots;
    std::size_t m_numRounds;
    std::size_t m_round;
};