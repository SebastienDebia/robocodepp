#pragma once

#include "Rules.hpp"
#include "Event.hpp"
#include "ExecCommands.hpp"
#include "Arc2D.hpp"
#include "RobotStatistics.hpp"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>

#include <memory>
#include <list>
#include <algorithm>
#include <cmath>

class World;

class Event;
class BulletHitBulletEvent;
class DeathEvent;
class HitRobotEvent;
class HitWallEvent;
class ScannedRobotEvent;

class RoundEndedEvent;
class BattleEndedEvent;

class Robot
{
public:
    struct RobotState
    {
        enum EState : unsigned int
        {
            /** The robot is active on the battlefield and has not hit the wall or a robot at this turn. */
            ACTIVE = 0,

            /** The robot has hit a wall, i.e. one of the four borders, at this turn. This state only last one turn. */
            HIT_WALL = 1,

            /** The robot has hit another robot at this turn. This state only last one turn. */
            HIT_ROBOT = 2,

            /** The robot is dead. */
            DEAD = 3
        };
    };

    static constexpr int
			WIDTH = 36,
			HEIGHT = 36;

	static constexpr int
			HALF_WIDTH_OFFSET = WIDTH / 2,
			HALF_HEIGHT_OFFSET = HEIGHT / 2;

    Robot( World& world, const std::string& name, int x = 400, unsigned y = 300 );

    void setPosition( int x, unsigned y );
    void reset();

    const std::string& getName() const;

    void setBulletColor( sf::Color color );
    void setBodyColor( sf::Color color );
    void setRadarColor( sf::Color color );
    void setGunColor( sf::Color color );
    void setScanColor( sf::Color color );
    sf::Color getBulletColor() const;
    sf::Color getBodyColor() const;
    sf::Color getRadarColor() const;
    sf::Color getGunColor() const;
    sf::Color getScanColor() const;

    Arc2D getScanArc();

    void setAdjustGunForRobotTurn( bool value );
    void setAdjustRadarForRobotTurn( bool value );

    int getX();
    int getY();

    sf::FloatRect getBoundingBox() { return m_boundingBox; }

    World& getWorld() { return m_world; }

    RobotState::EState getState()
    {
        return m_state;
    }
	void setState( RobotState::EState state )
    {
		m_state = state;
	}
    bool isDead() const
    {
		return m_state == RobotState::DEAD;
	}
    bool isAlive() const
    { 
		return m_state != RobotState::DEAD;
    }
    bool isWinner() const 
    {
        return m_isWinner;
    }
	void setWinner( bool newWinner ) {
		m_isWinner = newWinner;
	}

    void kill();

    float getAngle();
    float getBodyHeading();
    float getTurretAngle();
    float getTurretHeading();
    float getRadarAngle();
    float getRadarHeading();
    double getVelocity();
    double getEnergy();
    double getGunHeat();
    
    double getDistanceRemaining() {
		return m_currentCommands.getDistanceRemaining();
	}

	double getRadarTurnRemaining() {
		return m_currentCommands.getRadarTurnRemaining();
	}

	double getBodyTurnRemaining() {
		return m_currentCommands.getBodyTurnRemaining();
	}

	double getGunTurnRemaining() {
		return m_currentCommands.getGunTurnRemaining();
	}

    void setTurnGun( float angle );
    void setTurnGunRadians( float angle );
    void setTurnBody( float angle );
    void setTurnBodyRadians( float angle );
    void setTurnRadar( float radians );
    void setTurnRadarRadians( float radians );

    void setMaxVelocity( double newVelocity );
    void setMove( double distance );
    void setAhead( double distance );
    void setBack( double distance );

    void setFire( double power );

    void drainEnergy();
    void setEnergy( double newEnergy, bool resetInactiveTurnCount );

    void updateGunHeat();
    void updateEnergy( double delta );
    void updateHeading();
    void updateRadarHeading();
    void updateGunHeading();
    void updateMovement();

    std::string getNameForEvent( Robot* otherRobot );
    bool isCollidingRobot( std::list<Robot*> robots );

    void checkRobotCollision( std::list<Robot*> robots );
    void checkWallCollision();

    double getDistanceTraveledUntilStop(double velocity);
    double getNewVelocity(double velocity, double distance);
    double getMaxVelocity(double distance);
    double maxDecel(double speed);

    void updateBoundingBox();

    void tick();
    void processEvents();
    void performMove();
    void performScan();

    void addEvent( std::unique_ptr<Event> evt );

    virtual void run() = 0;
    virtual void onBulletHitBullet( BulletHitBulletEvent* e ) {};
    virtual void onDeath( DeathEvent* e ) {};
    virtual void onHitRobot( HitRobotEvent* e ) {};
    virtual void onHitWall( HitWallEvent* e ) {};
    virtual void onScannedRobot( ScannedRobotEvent* e ) {};

	virtual void onRoundEnded( RoundEndedEvent* e ) {};
	virtual void onBattleEnded( BattleEndedEvent* e ) {};

    RobotStatistics getRobotStatistics() { return m_statistics; }

protected:

    float rotate( float angle );
    float rotateTurret( float angle );
    float rotateRadar( float angle );

    void scan( double lastRadarHeading, std::list<Robot*> robots );
    bool intersects( Arc2D arc, sf::FloatRect rect );

    void zap( double zapAmount );

private:
    World& m_world;

    std::string m_name;

    ExecCommands m_currentCommands;

    sf::Transformable m_bodyPosition;
    sf::Transformable m_turretPosition;
    sf::Transformable m_radarPosition;

    sf::FloatRect m_boundingBox;

    double m_lastHeading;
    double m_lastGunHeading;
    double m_lastRadarHeading;

    double m_energy;
    double m_gunHeat;
    double m_velocity;

	bool m_scan;
	bool m_turnedRadarWithGun; // last round

    bool m_isExecFinishedAndDisabled;
	bool m_isEnergyDrained;
	bool m_isWinner;
	bool m_inCollision;
	bool m_isOverDriving;

    std::list<std::unique_ptr<Event>> m_events;

    sf::Color m_bulletColor;
    sf::Color m_bodyColor;
    sf::Color m_radarColor;
    sf::Color m_gunColor;
    sf::Color m_scanColor;

    bool m_adjustGunForRobotTurn;
    bool m_adjustRadarForRobotTurn;

    RobotState::EState m_state;
    Arc2D m_scanArc;
    std::size_t m_inactiveTurnCount;

    RobotStatistics m_statistics;
};