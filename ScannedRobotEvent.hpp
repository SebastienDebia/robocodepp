/**
 * Copyright (c) 2001-2019 Mathew A. Nelson and Robocode contributors
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse License v1.0
 * which accompanies this distribution, and is available at
 * https://robocode.sourceforge.io/license/epl-v10.html
 */
#pragma once

#include "Event.hpp"

#include "Utils.hpp"

#include <string>

/**
 * A ScannedRobotEvent is sent to {@link Robot#onScannedRobot(ScannedRobotEvent)
 * onScannedRobot()} when you scan a robot.
 * You can use the information contained in this event to determine what to do.
 * <p>
 * <b>Note</b>: You should not inherit from this class in your own event class!
 * The internal logic of this event class might change. Hence, your robot might
 * not work in future Robocode versions, if you choose to inherit from this class.
 *
 * @author Mathew A. Nelson (original)
 * @author Flemming N. Larsen (contributor)
 */
class ScannedRobotEvent : public Event
{
private:

	const std::string m_name;
	const double m_energy;
	const double m_heading;
	const double m_bearing;
	const double m_distance;
	const double m_velocity;
	const bool m_isSentryRobot;

public:

	/**
	 * Called by the game to create a new ScannedRobotEvent.
	 *
	 * @param name	 the name of the scanned robot
	 * @param energy   the energy of the scanned robot
	 * @param bearing  the bearing of the scanned robot, in radians
	 * @param distance the distance from your robot to the scanned robot
	 * @param heading  the heading of the scanned robot
	 * @param velocity the velocity of the scanned robot
	 * @param isSentryRobot flag specifying if the scanned robot is a sentry robot
	 * 
	 * @since 1.9.0.0
	 */
	ScannedRobotEvent( std::string name, double energy, double bearing, double distance, double heading, double velocity, bool isSentryRobot = false )
	 : m_name( name ),
		m_energy( energy ),
		m_heading( heading ),
		m_bearing( bearing ),
		m_distance( distance ),
		m_velocity( velocity ),
		m_isSentryRobot( isSentryRobot )
	{
	}

	/**
	 * Returns the bearing to the robot you scanned, relative to your robot's
	 * heading, in degrees (-180 <= getBearing() < 180)
	 *
	 * @return the bearing to the robot you scanned, in degrees
	 */
	double getBearing() {
		return m_bearing * 180.0 / Utils::PI;
	}

	/**
	 * Returns the bearing to the robot you scanned, relative to your robot's
	 * heading, in radians (-PI <= getBearingRadians() < PI)
	 *
	 * @return the bearing to the robot you scanned, in radians
	 */
	double getBearingRadians() {
		return m_bearing;
	}

	/**
	 * Returns the distance to the robot (your center to his center).
	 *
	 * @return the distance to the robot.
	 */
	double getDistance() const {
		return m_distance;
	}

	/**
	 * Returns the energy of the robot.
	 *
	 * @return the energy of the robot
	 */
	double getEnergy() {
		return m_energy;
	}

	/**
	 * Returns the heading of the robot, in degrees (0 <= getHeading() < 360)
	 *
	 * @return the heading of the robot, in degrees
	 */
	double getHeading() {
		return m_heading * 180.0 / Utils::PI;
	}

	/**
	 * Returns the heading of the robot, in radians (0 <= getHeading() < 2 * PI)
	 *
	 * @return the heading of the robot, in radians
	 */
	double getHeadingRadians() {
		return m_heading;
	}

	/**
	 * @return the energy of the robot
	 * @deprecated Use {@link #getEnergy()} instead.
	 */
	double getLife() {
		return m_energy;
	}

	/**
	 * Returns the name of the robot.
	 *
	 * @return the name of the robot
	 */
	std::string getName() {
		return m_name;
	}

	/**
	 * @return the bearing to the robot you scanned, in degrees
	 * @deprecated Use {@link #getBearing()} instead.
	 */
	double getRobotBearing() {
		return getBearing();
	}

	/**
	 * @return the robot bearing in degrees
	 * @deprecated Use {@link #getBearing()} instead.
	 */
	double getRobotBearingDegrees() {
		return getBearing();
	}

	/**
	 * @return the bearing to the robot you scanned, in radians
	 * @deprecated Use {@link #getBearingRadians()} instead.
	 */
	double getRobotBearingRadians() {
		return getBearingRadians();
	}

	/**
	 * @return the distance to the robot.
	 * @deprecated Use {@link #getDistance()} instead.
	 */
	double getRobotDistance() {
		return getDistance();
	}

	/**
	 * @return the heading of the robot, in degrees
	 * @deprecated Use {@link #getHeading()} instead.
	 */
	double getRobotHeading() {
		return getHeading();
	}

	/**
	 * @return the heading of the robot, in degrees
	 * @deprecated Use {@link #getHeading()} instead.
	 */
	double getRobotHeadingDegrees() {
		return getHeading();
	}

	/**
	 * @return the heading of the robot, in radians
	 * @deprecated Use {@link #getHeadingRadians()} instead.
	 */
	double getRobotHeadingRadians() {
		return getHeadingRadians();
	}

	/**
	 * @return the energy of the robot
	 * @deprecated Use {@link #getEnergy()} instead.
	 */
	double getRobotLife() {
		return getEnergy();
	}

	/**
	 * @return the name of the robot
	 * @deprecated Use {@link #getName()} instead.
	 */
	std::string getRobotName() {
		return getName();
	}

	/**
	 * @return the velocity of the robot
	 * @deprecated Use {@link #getVelocity()} instead.
	 */
	double getRobotVelocity() {
		return getVelocity();
	}

	/**
	 * Returns the velocity of the robot.
	 *
	 * @return the velocity of the robot
	 */
	double getVelocity() {
		return m_velocity;
	}
	
	/**
	 * {@inheritDoc}
	 */
	int compareTo( Event* event ) const
	{
		int res = 0; //super.compareTo(event);
		if (res != 0) {
			return res;
		}
		// Compare the distance, if the events are ScannedRobotEvents
		// The shorter distance to the robot, the higher priority
		auto pSre = dynamic_cast<ScannedRobotEvent*>( event );
		if( pSre != nullptr )
		{
			return (int) (getDistance() - pSre->getDistance());
		}
		// No difference found
		return 0;
	}
};
