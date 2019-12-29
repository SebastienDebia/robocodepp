#pragma once

/**
 * Copyright (c) 2001-2019 Mathew A. Nelson and Robocode contributors
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://robocode.sourceforge.io/license/epl-v10.html
 */
#include "../Robot.hpp"
#include "../Utils.hpp"

/// <summary>
///   This advanced robot type allows you to set a rate for each of the robot's movements.
///   <p />
///   You can set the rate for:
///   <ul>
///     <li>velocity - pixels per turn</li>
///     <li>robot turn - radians per turn</li>
///     <li>gun rotation - radians per turn</li>
///     <li>radar rotation - radians per turn</li>
///   </ul>
///   When you set a rate for one of the above movements, the movement will continue the move by
///   specified rate for ever, until the rate is changed. In order to move ahead or right, the
///   rate must be set to a positive value. If a negative value is used instead, the movement
///   will go back or to the left. In order to stop the movement, the rate must be
///   set to 0.
///   <para />
///   Note: When calling <see cref="m_velocityRate" />, <see cref="m_turnRate" />,
///   <see cref="m_gunRotationRate" />, <see cref="m_radarRotationRate" /> and variants,
///   any previous calls to "movement" functions outside of <see cref="RateControlRobot" />,
///   such as <see cref="AdvancedRobot.SetAhead(double)" />,
///   <see cref="AdvancedRobot.SetTurnLeft(double)" />,
///   <see cref="AdvancedRobot.SetTurnRadarRightRadians(double)" />  and similar will be
///   overridden when calling the <see cref="Execute()" /> on this robot class.
///   <p />
///   Look into the source code for the samplecs.VelociRobot in order to see how to use this
///   robot type.
/// <seealso cref="JuniorRobot"/>
/// <seealso cref="Robot"/>
/// <seealso cref="AdvancedRobot"/>
/// <seealso cref="TeamRobot"/>
/// <seealso cref="IDroid"/>
/// <seealso cref="IBorderSentry"/>
/// </summary>
class RateControlRobot : public Robot
{
public:
    RateControlRobot( World& world, const std::string& name, int x = 400, unsigned y = 300 )
	: Robot( world, name, x, y )
	{}

	/**
	 * Returns the speed the robot will move, in pixels per turn.
	 * Positive values means that the robot will move forward.
	 * Negative values means that the robot will move backwards.
	 * If the value is 0, the robot will stand still.
	 *
	 * @return The speed of the robot in pixels per turn
	 *
	 * @see #setVelocityRate(double)
	 */
    double getVelocityRate() const
    {
        return m_velocityRate;
    }

	/**
	 * Sets the speed the robot will move (forward), in pixels per turn.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the robot will move backwards
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the robot to move forward 2 pixels per turn
	 *   setVelocityRate(2);
	 *
	 *   // Set the robot to move backwards 8 pixels per turn
	 *   // (overrides the previous order)
	 *   setVelocityRate(-8);
	 *
	 *   ...
	 *   // Executes the last setVelocityRate()
	 *   execute();
	 * </pre>
	 *
	 * Note: This method overrules {@link robocode.AdvancedRobot#setAhead(double)} and
	 * {@link robocode.AdvancedRobot#setBack(double)}.
	 *
	 * @param velocityRate pixels per turn the robot will move.
	 * 
	 * @see #getVelocityRate()
	 * @see #setTurnRate(double)
	 * @see #setGunRotationRate(double)
	 * @see #setRadarRotationRate(double)
	 * @see AdvancedRobot#setAhead(double)
	 * @see AdvancedRobot#setBack(double)
	 */
    void setVelocityRate( double velocityRate )
    {
        m_velocityRate = velocityRate;
    }

	/**
	 * Gets the robot's clockwise rotation per turn, in degrees.
	 * Positive values means that the robot will turn to the right.
	 * Negative values means that the robot will turn to the left.
	 * If the value is 0, the robot will not turn.
	 *
	 * @return Angle of the clockwise rotation, in degrees.
	 *
	 * @see #setTurnRate(double)
	 */
	double getTurnRate()
	{
		return Utils::toRadians * m_turnRate;
	}

	/**
	 * Sets the robot's clockwise (right) rotation per turn, in degrees.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the robot turns counterclockwise (left)
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the robot to turn right 10 degrees per turn
	 *   setTurnRate(10);
	 *
	 *   // Set the robot to turn left 4 degrees per turn
	 *   // (overrides the previous order)
	 *   setTurnRate(-5);
	 *
	 *   ...
	 *   // Executes the last setTurnRate()
	 *   execute();
	 * </pre>
	 *
	 * @param turnRate angle of the clockwise rotation, in degrees.
	 *
	 * @see #getTurnRate()
	 * @see #setVelocityRate(double)
	 * @see #setGunRotationRate(double)
	 * @see #setRadarRotationRate(double)
	 * @see AdvancedRobot#setTurnRight(double)
	 * @see AdvancedRobot#setTurnLeft(double)
	 */
	void setTurnRate( double value )
	{
		m_turnRate = Utils::toRadians * value;
	}

	/**
	 * Gets the robot's clockwise rotation per turn, in radians.
	 * Positive values means that the robot will turn to the right.
	 * Negative values means that the robot will turn to the left.
	 * If the value is 0, the robot will not turn.
	 * 
	 * @return Angle of the clockwise rotation, in radians.
	 * 
	 * @see #getTurnRateRadians()
	 */
	double getTurnRateRadians()
	{
		return m_turnRate;
	}

	/**
	 * Sets the robot's clockwise (right) rotation per turn, in radians.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the robot turns counterclockwise (left)
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the robot to turn right pi / 32 radians per turn
	 *   setTurnRateRadians(Math.PI / 32);
	 *
	 *   // Set the robot to turn left pi / 20 radians per turn
	 *   // (overrides the previous order)
	 *   setTurnRateRadians(-Math.PI / 20);
	 *
	 *   ...
	 *   // Executes the last setTurnRateRadians()
	 *   execute();
	 * </pre>
	 * 
	 * @param turnRate angle of the clockwise rotation, in radians.
	 *
	 * @see #getTurnRateRadians()()
	 * @see #setVelocityRate(double)
	 * @see #setGunRotationRateRadians(double)
	 * @see #setRadarRotationRateRadians(double)
	 * @see AdvancedRobot#setTurnRightRadians(double)
	 * @see AdvancedRobot#setTurnLeftRadians(double)
	 */
	void setTurnRateRadians( double value )
	{
		m_turnRate = value;
	}

	/**
	 * Gets the gun's clockwise rotation per turn, in degrees.
	 * Positive values means that the gun will turn to the right.
	 * Negative values means that the gun will turn to the left.
	 * If the value is 0, the gun will not turn.
	 * 
	 * @return Angle of the clockwise rotation, in degrees.
	 * 
	 * @see #setGunRotationRate(double)
	 */
	double getGunRotationRate()
	{
		return Utils::toDegrees * m_gunRotationRate;
	}
    
	/**
	 * Sets the gun's clockwise (right) rotation per turn, in degrees.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the gun turns counterclockwise (left)
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the gun to turn right 15 degrees per turn
	 *   setGunRotationRate(15);
	 *
	 *   // Set the gun to turn left 9 degrees per turn
	 *   // (overrides the previous order)
	 *   setGunRotationRate(-9);
	 *
	 *   ...
	 *   // Executes the last setGunRotationRate()
	 *   execute();
	 * </pre>
	 * 
	 * @param gunRotationRate angle of the clockwise rotation, in degrees.
	 *
	 * @see #getGunRotationRate()
	 * @see #setVelocityRate(double)
	 * @see #setTurnRate(double)
	 * @see #setRadarRotationRate(double)
	 * @see AdvancedRobot#setTurnGunRight(double)
	 * @see AdvancedRobot#setTurnGunLeft(double)
	 */
	void setGunRotationRate( double value )
	{
		m_gunRotationRate = Utils::toRadians * value;
	}

	/**
	 * Gets the gun's clockwise rotation per turn, in radians.
	 * Positive values means that the gun will turn to the right.
	 * Negative values means that the gun will turn to the left.
	 * If the value is 0, the gun will not turn.
	 * 
	 * @return Angle of the clockwise rotation, in radians.
	 * 
	 * @see #setGunRotationRateRadians(double)
	 */
	double getGunRotationRateRadians()
	{
		return m_gunRotationRate;
	}

	/**
	 * Sets the gun's clockwise (right) rotation per turn, in radians.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the gun turns counterclockwise (left)
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the gun to turn right pi / 16 radians per turn
	 *   setGunRotationRateRadians(Math.PI / 16);
	 *
	 *   // Set the gun to turn left pi / 12 radians per turn
	 *   // (overrides the previous order)
	 *   setGunRotationRateRadians(-Math.PI / 12);
	 *
	 *   ...
	 *   // Executes the last setGunRotationRateRadians()
	 *   execute();
	 * </pre>
	 * 
	 * @param gunRotationRate angle of the clockwise rotation, in radians.
	 *
	 * @see #getGunRotationRateRadians()
	 * @see #setVelocityRate(double)
	 * @see #setTurnRateRadians(double)
	 * @see #setRadarRotationRateRadians(double)
	 * @see AdvancedRobot#setTurnGunRightRadians(double)
	 * @see AdvancedRobot#setTurnGunLeftRadians(double)
	 */
	void setGunRotationRateRadians( double value )
	{
		m_gunRotationRate = value;
	}
	
	/**
	 * Gets the radar's clockwise rotation per turn, in degrees.
	 * Positive values means that the radar will turn to the right.
	 * Negative values means that the radar will turn to the left.
	 * If the value is 0, the radar will not turn.
	 * 
	 * @return Angle of the clockwise rotation, in degrees.
	 * 
	 * @see #setRadarRotationRate(double)
	 */
	double getRadarRotationRate()
	{
		return Utils::toDegrees * m_radarRotationRate;
	}
	
	/**
	 * Sets the radar's clockwise (right) rotation per turn, in degrees.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the radar turns counterclockwise (left)
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the radar to turn right 45 degrees per turn
	 *   setRadarRotationRate(45);
	 *
	 *   // Set the radar to turn left 15 degrees per turn
	 *   // (overrides the previous order)
	 *   setRadarRotationRate(-15);
	 *
	 *   ...
	 *   // Executes the last setRadarRotationRate()
	 *   execute();
	 * </pre>
	 * 
	 * @param radarRotationRate angle of the clockwise rotation, in degrees.
	 *
	 * @see #getRadarRotationRate()
	 * @see #setVelocityRate(double)
	 * @see #setTurnRate(double)
	 * @see #setGunRotationRate(double)
	 * @see AdvancedRobot#setTurnRadarRight(double)
	 * @see AdvancedRobot#setTurnRadarLeft(double)
	 */
	void setRadarRotationRate( double value )
	{
		m_radarRotationRate = Utils::toRadians * value;
	}
	/**
	 * Gets the radar's clockwise rotation per turn, in radians.
	 * Positive values means that the radar will turn to the right.
	 * Negative values means that the radar will turn to the left.
	 * If the value is 0, the radar will not turn.
	 * 
	 * @return Angle of the clockwise rotation, in radians.
	 * 
	 * @see #setRadarRotationRateRadians(double)
	 */
	double getRadarRotationRateRadians()
	{
		return m_radarRotationRate;
	}

	/**
	 * Sets the radar's clockwise (right) rotation per turn, in radians.
	 * <p>
	 * This call returns immediately, and will not execute until you call
	 * execute() or take an action that executes.
	 * <p>
	 * Note that both positive and negative values can be given as input,
	 * where negative values means that the radar turns counterclockwise (left)
	 * <p>
	 * Example:
	 * <pre>
	 *   // Set the radar to turn right pi / 4 radians per turn
	 *   setRadarRotationRateRadians(Math.PI / 4);
	 *
	 *   // Set the radar to turn left pi / 8 radians per turn
	 *   // (overrides the previous order)
	 *   setRadarRotationRateRadians(-Math.PI / 8);
	 *
	 *   ...
	 *   // Executes the last setRadarRotationRateRadians()
	 *   execute();
	 * </pre>
	 * 
	 * @param radarRotationRate angle of the clockwise rotation, in radians.
	 *
	 * @see #getRadarRotationRateRadians()
	 * @see #setVelocityRate(double)
	 * @see #setTurnRateRadians(double)
	 * @see #setGunRotationRateRadians(double)
	 * @see AdvancedRobot#setTurnRadarRightRadians(double)
	 * @see AdvancedRobot#setTurnRadarLeftRadians(double)
	 */
	void setRadarRotationRateRadians( double value )
	{
		m_radarRotationRate = value;
	}

	/**
	 * Executes any pending actions, or continues executing actions that are
	 * in process. This call returns after the actions have been started.
	 * <p>
	 * Note that advanced robots <em>must</em> call this function in order to
	 * execute pending set* calls like e.g. {@code setVelocityRate()}, {@code setFire()},
	 * {@code setTurnRate()} etc. Otherwise, these calls will never get executed.
	 * <p>
	 * Any previous calls to "movement" functions outside of {@code RateControlRobot},
	 * such as {@code setAhead()}, {@code setTurnLeft()}, {@code setTurnRadarLeftRadians()}
	 * etc. will be overridden when this method is called on this robot class.
	 * <p>
	 * In this example the robot will move while turning:
	 * <pre>
	 *   setVelocityRate(6);
	 *   setTurnRate(7);
	 *
	 *   while (true) {
	 *       execute();
	 *   }
	 * </pre>
	 */
    void execute()
    {
        setMaxVelocity( m_velocityRate );
        if( m_velocityRate > 0 )
        {
            setAhead( std::numeric_limits<double>::infinity() );
        }
        else if( m_velocityRate < 0 )
        {
            setBack( std::numeric_limits<double>::infinity() );
        }
        else
        {
            setAhead( 0 );
        }

        setTurnGun( getGunRotationRate() );
        setTurnRadar( getRadarRotationRate() );
        setTurnBody( getTurnRate() );
    }

private:
    double m_velocityRate; // Pixels per turn
    double m_turnRate; // Radians per turn
    double m_gunRotationRate; // Radians per turn
    double m_radarRotationRate; // Radians per turn
};