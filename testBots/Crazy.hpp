/**
 * Copyright (c) 2001-2019 Mathew A. Nelson and Robocode contributors
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://robocode.sourceforge.io/license/epl-v10.html
 */
#include "Robot.hpp"


/**
 * Crazy - a sample robot by Mathew Nelson.
 * <p>
 * This robot moves around in a crazy pattern.
 *
 * @author Mathew A. Nelson (original)
 * @author Flemming N. Larsen (contributor)
 */
class Crazy : public Robot
{
public:
	/**
	 * run: Crazy's main run function
	 */
	public void run()
    {
		// Set colors
		//setBodyColor(new Color(0, 200, 0));
		//setGunColor(new Color(0, 150, 50));
		//setRadarColor(new Color(0, 100, 100));
		//setBulletColor(new Color(255, 255, 100));
		//setScanColor(new Color(255, 200, 200));

		// Loop forever
		//while (true)
        {
			// Tell the game we will want to move ahead 40000 -- some large number
			setAhead(40000);
			m_movingForward = true;
			// Tell the game we will want to turn right 90
			setTurnBody(90);
			// At this point, we have indicated to the game that *when we do something*,
			// we will want to move ahead and turn right.  That's what "set" means.
			// It is important to realize we have not done anything yet!
			// In order to actually move, we'll want to call a method that
			// takes real time, such as waitFor.
			// waitFor actually starts the action -- we start moving and turning.
			// It will not return until we have finished turning.
			waitFor(new TurnCompleteCondition(this));
			// Note:  We are still moving ahead now, but the turn is complete.
			// Now we'll turn the other way...
			setTurnLeft(180);
			// ... and wait for the turn to finish ...
			waitFor(new TurnCompleteCondition(this));
			// ... then the other way ...
			setTurnRight(180);
			// .. and wait for that turn to finish.
			waitFor(new TurnCompleteCondition(this));
			// then back to the top to do it all again
		}
	}

	/**
	 * onHitWall:  Handle collision with wall.
	 */
	void onHitWall(HitWallEvent* e)
    {
		// Bounce off!
		reverseDirection();
	}

	/**
	 * reverseDirection:  Switch from ahead to back & vice versa
	 */
	void reverseDirection()
    {
		if (m_movingForward) {
			setBack(40000);
			m_movingForward = false;
		} else {
			setAhead(40000);
			m_movingForward = true;
		}
	}

	/**
	 * onScannedRobot:  Fire!
	 */
	/*void onScannedRobot( ScannedRobotEvent e )
    {
		setFire( 1 );
    }*/

	/**
	 * onHitRobot:  Back up!
	 */
	void onHitRobot( HitRobotEvent* e )
    {
		// If we're moving the other robot, reverse!
		if (e->isMyFault()) {
			reverseDirection();
		}
	}

private:
	bool m_movingForward = false;
};