#include "../Robot.hpp"

#include "../ScannedRobotEvent.hpp"

#include <cmath>
#include <iostream>

/**
 * SuperTracker - a Super Sample Robot by CrazyBassoonist based on the robot Tracker by Mathew Nelson and maintained by Flemming N. Larsen
 * <p/>
 * Locks onto a robot, moves close, fires when close.
 */
class SuperTracker: public Robot
{
public:
    SuperTracker( World& world, int x = 400, unsigned y = 300 )
    : Robot( world, "SuperTracker", x, y )
    {
    }

    /**
     * run:  Tracker's main run function
     */
    void run()
    {
        //std::cout << getRadarTurnRemaining() << std::endl;
        setAdjustRadarForRobotTurn(true);//keep the radar still while we turn
        setBodyColor(sf::Color(128, 128, 50));
        setGunColor(sf::Color(50, 50, 20));
        setRadarColor(sf::Color(200, 200, 70));
        setScanColor(sf::Color::White);
        setBulletColor(sf::Color::Blue);
        setAdjustGunForRobotTurn(true); // Keep the gun still when we turn
        if( getRadarTurnRemaining() <= 0.01 && getRadarTurnRemaining() >= -0.01 )
            setTurnRadar( std::numeric_limits<double>::infinity() );//keep turning radar right
    }

    /**
     * onScannedRobot:  Here's the good stuff
     */
    void onScannedRobot( ScannedRobotEvent* e )
    {
        //std::cout << "scan!\n";
        double velocity = getVelocity() > 0 ? getVelocity() : .01;
        double evtVelocity = e->getVelocity() > 0 ? e->getVelocity() : .01;
        double absBearing = e->getBearingRadians() + getBodyHeading(); //enemies absolute bearing
        double latVel = evtVelocity * std::sin( e->getHeadingRadians() - absBearing ); //enemies later velocity
        double gunTurnAmt; //amount to turn our gun

        // Absolute angle towards target
        double angleToEnemy = getBodyHeading() + e->getBearingRadians();
    
        // Subtract current radar heading to get the turn required to face the enemy, be sure it is normalized
        double radarTurn = Utils::normalRelativeAngle( angleToEnemy - getRadarHeading() );
    
        // Distance we want to scan from middle of enemy to either side
        // The 36.0 is how many units from the center of the enemy robot it scans.
        double extraTurn = std::min( std::atan( 36.0 / e->getDistance() ), Rules::RADAR_TURN_RATE_RADIANS );
    
        // Adjust the radar turn so it goes that much further in the direction it is going to turn
        // Basically if we were going to turn it left, turn it even more left, if right, turn more right.
        // This allows us to overshoot our enemy so that we get a good sweep that will not slip.
        if (radarTurn < 0)
            radarTurn -= extraTurn;
        else
            radarTurn += extraTurn;
    
        //Turn the radar
        setTurnRadarRadians(radarTurn);

        //if(Math.random()>.9)
        {
            //setMaxVelocity( ( 12 * Math.random() ) + 12 );//randomly change speed
            setMaxVelocity( 24 );
        }
        if( e->getDistance() > 200 )
        {
            //if distance is greater than 150
            gunTurnAmt = Utils::normalRelativeAngle( absBearing - getTurretHeading() + latVel/22 ); //amount to turn our gun, lead just a little bit
            setTurnGunRadians( gunTurnAmt ); //turn our gun
            setTurnBodyRadians( Utils::normalRelativeAngle(absBearing - getBodyHeading() + latVel/velocity ) ); //drive towards the enemies predicted future location
            setAhead( ( e->getDistance() - 140 ) * moveDirection ); //move forward
            setFire(3); //fire
        }
        else
        {
            //if we are close enough...
            gunTurnAmt = Utils::normalRelativeAngle( absBearing - getTurretHeading() + latVel/15 ); //amount to turn our gun, lead just a little bit
            setTurnGunRadians( gunTurnAmt ); //turn our gun
            setTurnBodyRadians( -1 * ( -Utils::PI/2 - e->getBearingRadians() ) ); //turn perpendicular to the enemy
            setAhead( ( e->getDistance() - 140 ) * moveDirection ); //move forward
            setFire(3); //fire
        }
    }

    void onHitWall( HitWallEvent* )
    {
        moveDirection = -moveDirection; //reverse direction upon hitting a wall
    }

    void onHitRobot( HitRobotEvent* )
    {
        moveDirection = -moveDirection; //reverse direction upon hitting a robot
    }

    /**
     * onWin:  Do a victory dance
     *//*
    void onWin(WinEvent e)
    {
        for (int i = 0; i < 50; i++) {
            turnRight(30);
            turnLeft(30);
        }
    }*/

private:
    int moveDirection=1;//which way to move
};