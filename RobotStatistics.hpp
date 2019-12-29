#pragma once

class Robot;

#include "BattleResults.hpp"
#include "Rules.hpp"

#include <map>
#include <string>

class RobotStatistics
{
public:
    RobotStatistics( Robot* pRobot );

	void reset( int numberOfRobots ) {
		resetScores();

        m_numberOfRobots = numberOfRobots;

		isActive = true;
		m_bIsInRound = true;
	}

	void generateTotals() {
		totalSurvivalScore += survivalScore;
		totalLastSurvivorBonus += lastSurvivorBonus;
		totalBulletDamageScore += bulletDamageScore;
		totalBulletKillBonus += bulletKillBonus;
		totalRammingDamageScore += rammingDamageScore;
		totalRammingKillBonus += rammingKillBonus;

		totalScore = totalBulletDamageScore + totalRammingDamageScore + totalSurvivalScore + totalRammingKillBonus
				   + totalBulletKillBonus + totalLastSurvivorBonus;

		m_bIsInRound = false;
	}

	double getTotalScore() {
		return totalScore;
	}

	double getTotalSurvivalScore() {
		return totalSurvivalScore;
	}

	double getTotalLastSurvivorBonus() {
		return totalLastSurvivorBonus;
	}

	double getTotalBulletDamageScore() {
		return totalBulletDamageScore;
	}

	double getTotalBulletKillBonus() {
		return totalBulletKillBonus;
	}

	double getTotalRammingDamageScore() {
		return totalRammingDamageScore;
	}

	double getTotalRammingKillBonus() {
		return totalRammingKillBonus;
	}

	int getTotalFirsts() {
		return totalFirsts;
	}

	int getTotalSeconds() {
		return totalSeconds;
	}

	int getTotalThirds() {
		return totalThirds;
	}

	double getCurrentScore() {
		return (bulletDamageScore + rammingDamageScore + survivalScore + rammingKillBonus + bulletKillBonus
				+ lastSurvivorBonus);
	}

	double getCurrentSurvivalScore() {
		return survivalScore;
	}

	double getCurrentSurvivalBonus() {
		return lastSurvivorBonus;
	}

	double getCurrentBulletDamageScore() {
		return bulletDamageScore;
	}

	double getCurrentBulletKillBonus() {
		return bulletKillBonus;
	}

	double getCurrentRammingDamageScore() {
		return rammingDamageScore;
	}

	double getCurrentRammingKillBonus() {
		return rammingKillBonus;
	}

	void scoreSurvival() {
		if (isActive) {
			survivalScore += 50;
		}
	}

	void scoreLastSurvivor() {
		if (isActive) {
			int enemyCount = m_numberOfRobots - 1;

			lastSurvivorBonus += 10 * enemyCount;
			totalFirsts++;
		}
	}

	void scoreBulletDamage(std::string robot, double damage) {
		if (isActive) {
			incrementRobotDamage(robot, damage);
			bulletDamageScore += damage;
		}
	}

	double scoreBulletKill(std::string robot) {
		if (isActive) {
			double bonus = getRobotDamage(robot) * 0.20;

			bulletKillBonus += bonus;
			return bonus;
		}
		return 0;
	}

	void scoreRammingDamage(std::string robot) {
		if (isActive) {
			incrementRobotDamage(robot, Rules::ROBOT_HIT_DAMAGE);
			rammingDamageScore += Rules::ROBOT_HIT_BONUS;
		}
	}

	double scoreRammingKill(std::string robot) {
		if (isActive) {
			double bonus = getRobotDamage(robot) * 0.30;
			
			rammingKillBonus += bonus;
			return bonus;
		}
		return 0;
	}

	void scoreRobotDeath(int enemiesRemaining);

	void scoreFirsts() {
		if (isActive) {
			totalFirsts++;
		}
	}

	void setInactive() {
		resetScores();
		isActive = false;
	}

	BattleResults getFinalResults();

	bool isInRound() {
		return m_bIsInRound;
	}

	void cleanup() {// Do nothing, for now
	}
protected:
	void resetScores() {
		robotDamageMap.clear();
		survivalScore = 0;
		lastSurvivorBonus = 0;
		bulletDamageScore = 0;
		bulletKillBonus = 0;
		rammingDamageScore = 0;
		rammingKillBonus = 0;
	}

	double getRobotDamage(std::string robot) {
		double damage = robotDamageMap[ robot ];

		return damage;
	}

	void incrementRobotDamage(std::string robot, double damage) {
		double newDamage = getRobotDamage(robot) + damage;

		robotDamageMap.insert( std::make_pair( robot, newDamage ) );
	}

private:
	Robot* m_pRobot;
	int m_numberOfRobots;

	int rank;
	bool isActive;
	bool m_bIsInRound;

	double survivalScore;
	double lastSurvivorBonus;
	double bulletDamageScore;
	double bulletKillBonus;
	double rammingDamageScore;
	double rammingKillBonus;

	std::map<std::string, double> robotDamageMap;

	double totalScore;
	double totalSurvivalScore;
	double totalLastSurvivorBonus;
	double totalBulletDamageScore;
	double totalBulletKillBonus;
	double totalRammingDamageScore;
	double totalRammingKillBonus;

	int totalFirsts;
	int totalSeconds;
	int totalThirds;
};