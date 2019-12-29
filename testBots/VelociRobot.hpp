#pragma once

/**
 * Copyright (c) 2001-2018 Mathew A. Nelson and Robocode contributors
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse License v1.0
 * which accompanies this distribution, and is available at
 * http://robocode.sourceforge.net/license/epl-v10.html
 */
#include "RateControlRobot.hpp"

//#include "../HitByBulletEvent.hpp"
#include "../HitWallEvent.hpp"
//#include "../ScannedRobotEvent.hpp"


/**
 * This is a sample of a robot using the RateControlRobot class
 * 
 * @author Joshua Galecki (original)
 */
class VelociRobot : public RateControlRobot
{
public:
    VelociRobot( World& world, int x = 400, unsigned y = 300 )
    : RateControlRobot( world, "VelociRobot", x, y ),
    m_turnCounter( 0 )
    {
    }

	void run()
    {
		setGunRotationRate(15);
		
		//while( true )
        {
			if( m_turnCounter % 64 == 0 )
            {
				// Straighten out, if we were hit by a bullet and are turning
				setTurnRate(0);
				// Go forward with a velocity of 4
				setVelocityRate(4);
			}
			if( m_turnCounter % 64 == 32 )
            {
				// Go backwards, faster
				setVelocityRate(-6);
			}
			m_turnCounter++;
			execute();
		}
	}
/*
	void onScannedRobot( ScannedRobotEvent e )
    {
		fire(1);
	}

	void onHitByBullet( HitByBulletEvent e )
    {
		// Turn to confuse the other robot
		setTurnRate(5);
	}
*/
	void onHitWall( HitWallEvent* e )
    {
		// Move away from the wall
		setVelocityRate( -1 * getVelocityRate() );
	}

private:
	int m_turnCounter;
};