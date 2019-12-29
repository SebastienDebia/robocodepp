#pragma once

#include "Event.hpp"
#include "Utils.hpp"

class HitRobotEvent : public Event
{
public:
	/**
	 * Called by the game to create a new HitRobotEvent.
	 *
	 * @param name	the name of the robot you hit
	 * @param bearing the bearing to the robot that your robot hit, in radians
	 * @param energy  the amount of energy of the robot you hit
	 * @param atFault {@code true} if your robot was moving toward the other
	 *                robot; {@code false} otherwise
	 */
    HitRobotEvent( std::string name, double bearing, double energy, bool atFault );

	/**
	 * Returns the bearing to the robot you hit, relative to your robot's
	 * heading, in degrees (-180 <= getBearing() < 180)
	 *
	 * @return the bearing to the robot you hit, in degrees
	 */
	double getBearing()
	{
		return m_bearing * 180.0 / Utils::PI;
	}

	/**
	 * @return the bearing to the robot you hit, in degrees
	 * @deprecated Use {@link #getBearing()} instead.
	 */
	double getBearingDegrees()
	{
		return getBearing();
	}

	/**
	 * Returns the bearing to the robot you hit, relative to your robot's
	 * heading, in radians (-PI <= getBearingRadians() < PI)
	 *
	 * @return the bearing to the robot you hit, in radians
	 */
	double getBearingRadians()
	{
		return m_bearing;
	}

	/**
	 * Returns the amount of energy of the robot you hit.
	 *
	 * @return the amount of energy of the robot you hit
	 */
	double getEnergy()
	{
		return m_energy;
	}

	/**
	 * Returns the name of the robot you hit.
	 *
	 * @return the name of the robot you hit
	 */
	std::string getName()
	{
		return m_robotName;
	}

	/**
	 * @return the name of the robot you hit
	 * @deprecated Use {@link #getName()} instead.
	 */
	std::string getRobotName()
	{
		return m_robotName;
	}

	/**
	 * Checks if your robot was moving towards the robot that was hit.
	 * <p>
	 * If {@link #isMyFault()} returns {@code true} then your robot's movement
	 * (including turning) will have stopped and been marked complete.
	 * <p>
	 * Note: If two robots are moving toward each other and collide, they will
	 * each receive two HitRobotEvents. The first will be the one if
	 * {@link #isMyFault()} returns {@code true}.
	 *
	 * @return {@code true} if your robot was moving towards the robot that was
	 *         hit; {@code false} otherwise.
	 */
	bool isMyFault()
	{
		return m_atFault;
	}

	/**
	 * {@inheritDoc}
	 */
	int compareTo( Event* event )
	{
		int res = 0; //Event::compareTo(event);

		if( res != 0 )
		{
			return res;
		}

		// Compare the isMyFault, if the events are HitRobotEvents
		// The isMyFault has higher priority when it is set compared to when it is not set
		auto pHre = dynamic_cast<HitRobotEvent*>( event );
		if( pHre != nullptr )
		{
			int compare1 = isMyFault() ? -1 : 0;
			int compare2 = pHre->isMyFault() ? -1 : 0;

			return compare1 - compare2;
		}

		// No difference found
		return 0;
	}

private:
	std::string m_robotName;
	double m_bearing;
	double m_energy;
	bool m_atFault;
};