#pragma once

#include "Event.hpp"
#include "Utils.hpp"

class HitWallEvent : public Event
{
public:
    HitWallEvent( double bearing );

	/**
	 * Returns the bearing to the wall you hit, relative to your robot's
	 * heading, in degrees (-180 <= getBearing() < 180)
	 *
	 * @return the bearing to the wall you hit, in degrees
	 */
	double getBearing()
	{
		return m_bearing * 180.0 / Utils::PI;
	}

	/**
	 * @return the bearing to the wall you hit, in degrees
	 * @deprecated Use {@link #getBearing()} instead.
	 */
	double getBearingDegrees()
	{
		return getBearing();
	}

	/**
	 * Returns the bearing to the wall you hit, relative to your robot's
	 * heading, in radians (-PI <= getBearingRadians() < PI)
	 *
	 * @return the bearing to the wall you hit, in radians
	 */
	double getBearingRadians()
	{
		return m_bearing;
	}

private:
    double m_bearing;
};