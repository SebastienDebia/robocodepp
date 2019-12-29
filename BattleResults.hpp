#pragma once

#include <string>

/**
 * Contains the battle results returned by {@link BattleEndedEvent#getResults()}
 * when a battle has ended.
 *
 * @see BattleEndedEvent#getResults()
 * @see Robot#onBattleEnded(BattleEndedEvent)
 *
 * @author Pavel Savara (original)
 * @author Flemming N. Larsen (contributor)
 *
 * @since 1.6.1
 */
class BattleResults
{
public:
	/**
	 * Constructs this BattleResults object.
	 *
	 * @param teamLeaderName    the name of the team leader.
	 * @param rank              the rank of the robot in the battle.
	 * @param score             the total score for the robot in the battle.
	 * @param survival          the survival score for the robot in the battle.
	 * @param lastSurvivorBonus the last survivor bonus for the robot in the battle.
	 * @param bulletDamage      the bullet damage score for the robot in the battle.
	 * @param bulletDamageBonus the bullet damage bonus for the robot in the battle.
	 * @param ramDamage         the ramming damage for the robot in the battle.
	 * @param ramDamageBonus    the ramming damage bonus for the robot in the battle.
	 * @param firsts            the number of rounds this robot placed first.
	 * @param seconds           the number of rounds this robot placed second.
	 * @param thirds            the number of rounds this robot placed third.
	 */
	BattleResults(
			std::string teamLeaderName,
			int rank,
			double score,
			double survival,
			double lastSurvivorBonus,
			double bulletDamage,
			double bulletDamageBonus,
			double ramDamage,
			double ramDamageBonus,
			int firsts,
			int seconds,
			int thirds
			)
    : teamLeaderName( teamLeaderName ),
		rank( rank ),
		score( score ),
		survival( survival ),
		lastSurvivorBonus( lastSurvivorBonus ),
		bulletDamage( bulletDamage ),
		bulletDamageBonus( bulletDamageBonus ),
		ramDamage( ramDamage ),
		ramDamageBonus( ramDamageBonus ),
		firsts( firsts ),
		seconds( seconds ),
		thirds( thirds )
    {
	}

	/**
	 * Returns the name of the team leader in the team or the name of the
	 * robot if the robot is not participating in a team.
	 *
	 * @return the name of the team leader in the team or the name of the robot.
	 */
	std::string getTeamLeaderName() {
		return teamLeaderName;
	}

	/**
	 * Returns the rank of this robot in the battle results.
	 *
	 * @return the rank of this robot in the battle results.
	 */
	int getRank() {
		return rank;
	}

	/**
	 * Returns the total score of this robot in the battle.
	 *
	 * @return the total score of this robot in the battle.
	 */
	int getScore() {
		return (int) (score + 0.5);
	}

	/**
	 * Returns the survival score of this robot in the battle.
	 *
	 * @return the survival score of this robot in the battle.
	 */
	int getSurvival() {
		return (int) (survival + 0.5);
	}

	/**
	 * Returns the last survivor score of this robot in the battle.
	 *
	 * @return the last survivor score of this robot in the battle.
	 */
	int getLastSurvivorBonus() {
		return (int) (lastSurvivorBonus + 0.5);
	}

	/**
	 * Returns the bullet damage score of this robot in the battle.
	 *
	 * @return the bullet damage score of this robot in the battle.
	 */
	int getBulletDamage() {
		return (int) (bulletDamage + 0.5);
	}

	/**
	 * Returns the bullet damage bonus of this robot in the battle.
	 *
	 * @return the bullet damage bonus of this robot in the battle.
	 */
	int getBulletDamageBonus() {
		return (int) (bulletDamageBonus + 0.5);
	}

	/**
	 * Returns the ram damage score of this robot in the battle.
	 *
	 * @return the ram damage score of this robot in the battle.
	 */
	int getRamDamage() {
		return (int) (ramDamage + 0.5);
	}

	/**
	 * Returns the ram damage bonus of this robot in the battle.
	 *
	 * @return the ram damage bonus of this robot in the battle.
	 */
	int getRamDamageBonus() {
		return (int) (ramDamageBonus + 0.5);
	}

	/**
	 * Returns the number of rounds this robot placed first in the battle.
	 *
	 * @return the number of rounds this robot placed first in the battle.
	 */
	int getFirsts() {
		return firsts;
	}

	/**
	 * Returns the number of rounds this robot placed second in the battle.
	 *
	 * @return the number of rounds this robot placed second in the battle.
	 */
	int getSeconds() {
		return seconds;
	}

	/**
	 * Returns the number of rounds this robot placed third in the battle.
	 *
	 * @return the number of rounds this robot placed third in the battle.
	 */
	int getThirds() {
		return thirds;
	}

	/**
	 * {@inheritDoc}
	 */
	bool operator<( const BattleResults& other ) {
		return score < other.score;
	}
private:
	std::string teamLeaderName;
	int rank;
	double score;
	double survival;
	double lastSurvivorBonus;
	double bulletDamage;
	double bulletDamageBonus;
	double ramDamage;
	double ramDamageBonus;
	int firsts;
	int seconds;
	int thirds;
};
