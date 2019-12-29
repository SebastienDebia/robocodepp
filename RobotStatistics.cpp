#include "RobotStatistics.hpp"

#include "Robot.hpp"

RobotStatistics::RobotStatistics( Robot* pRobot )
{
    m_pRobot = pRobot;
}

void RobotStatistics::scoreRobotDeath(int enemiesRemaining)
{
    switch (enemiesRemaining) {
    case 0:
        if (!m_pRobot->isWinner()) {
            totalFirsts++;
        }
        break;

    case 1:
        totalSeconds++;
        break;

    case 2:
        totalThirds++;
        break;
    }
}

BattleResults RobotStatistics::getFinalResults()
{
    return BattleResults( m_pRobot->getName(), rank, totalScore, totalSurvivalScore, totalLastSurvivorBonus,
            totalBulletDamageScore, totalBulletKillBonus, totalRammingDamageScore, totalRammingKillBonus, totalFirsts,
            totalSeconds, totalThirds );
}