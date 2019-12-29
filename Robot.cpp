#include "Robot.hpp"

#include "World.hpp"
#include "Bullet.hpp"
#include "Rules.hpp"
#include "Utils.hpp"

#include "Event.hpp"
#include "BulletHitBulletEvent.hpp"
#include "DeathEvent.hpp"
#include "HitRobotEvent.hpp"
#include "HitWallEvent.hpp"
#include "ScannedRobotEvent.hpp"

#include "BattleEndedEvent.hpp"
#include "RoundEndedEvent.hpp"

#include <iostream>
#include <memory>

Robot::Robot( World& world, const std::string& name, int x /*= 400*/, unsigned y /*= 400*/ )
 : m_world( world ),
 m_name( name ),
 m_energy( 100 ),
 m_gunHeat( 0 ),
 m_velocity( 0 ),
 m_isExecFinishedAndDisabled( false ),
 m_isEnergyDrained( false ),
 m_isWinner( false ),
 m_inCollision( false ),
 m_isOverDriving( false ),
 m_bulletColor( sf::Color::Red ),
 m_bodyColor( sf::Color::Black ),
 m_radarColor( sf::Color::Red ),
 m_gunColor( sf::Color::Red ),
 m_scanColor( sf::Color::Blue ),
 m_adjustGunForRobotTurn( false ),
 m_adjustRadarForRobotTurn( false ),
 m_state( RobotState::ACTIVE ),
 m_scanArc( x, y, Rules::RADAR_SCAN_RADIUS, 0, 0 ),
 m_statistics( this )
{
    setPosition( x, y );
}

void Robot::setPosition( int x, unsigned y )
{
    m_bodyPosition.setPosition( x, y );
    updateBoundingBox();
}

void Robot::reset()
{
    m_energy = 100;
    m_gunHeat = 0;
    m_velocity = 0;
    m_isExecFinishedAndDisabled = false;
    m_isEnergyDrained = false;
    m_isWinner = false;
    m_inCollision = false;
    m_isOverDriving = false;
    m_state = RobotState::ACTIVE;
    m_scanArc = Arc2D( m_bodyPosition.getPosition().x, m_bodyPosition.getPosition().y, Rules::RADAR_SCAN_RADIUS, 0, 0 );

    m_statistics.reset( m_world.getRobots().size() );
}

const std::string& Robot::getName() const
{
    return m_name;
}

void Robot::setBulletColor( sf::Color color )
{
    m_bulletColor = color;
}

void Robot::setBodyColor( sf::Color color )
{
    m_bodyColor = color;
}

void Robot::setRadarColor( sf::Color color )
{
    m_radarColor = color;
}

void Robot::setGunColor( sf::Color color )
{
    m_gunColor = color;
}

void Robot::setScanColor( sf::Color color )
{
    m_scanColor = color;
}

sf::Color Robot::getBulletColor() const
{
    return m_bulletColor;
}

sf::Color Robot::getBodyColor() const
{
    return m_bodyColor;
}

sf::Color Robot::getRadarColor() const
{
    return m_radarColor;
}

sf::Color Robot::getGunColor() const
{
    return m_gunColor;
}

sf::Color Robot::getScanColor() const
{
    return m_scanColor;
}

Arc2D Robot::getScanArc()
{
    return m_scanArc;
}

void Robot::setAdjustGunForRobotTurn( bool value )
{
    m_currentCommands.setAdjustGunForBodyTurn( value );
}

void Robot::setAdjustRadarForRobotTurn( bool value )
{
    m_currentCommands.setAdjustRadarForGunTurn( value );
}

int Robot::getX()
{
    return m_bodyPosition.getPosition().x;
}

int Robot::getY()
{
    return m_bodyPosition.getPosition().y;
}

float Robot::getAngle()
{
    return m_bodyPosition.getRotation();
}

float Robot::getBodyHeading()
{
    return m_bodyPosition.getRotation() * Utils::toRadians;
}

float Robot::getTurretAngle()
{
    return m_turretPosition.getRotation();
}

float Robot::getTurretHeading()
{
    return m_turretPosition.getRotation() * Utils::toRadians;
}

float Robot::getRadarAngle()
{
    return m_radarPosition.getRotation();
}

float Robot::getRadarHeading()
{
    return m_radarPosition.getRotation() * Utils::toRadians;
}

double Robot::getVelocity()
{
    return m_velocity;
}

double Robot::getEnergy()
{
    return m_energy;
}

double Robot::getGunHeat()
{
    return m_gunHeat;
}

void Robot::setTurnGun( float angle )
{
    m_currentCommands.setGunTurnRemaining( angle );
}

void Robot::setTurnGunRadians( float angle )
{
    setTurnGun( angle * Utils::toDegrees );
}

void Robot::setTurnBody( float angle )
{
    if( getEnergy() > 0 )
    {
        m_currentCommands.setBodyTurnRemaining( angle );
    }
}

void Robot::setTurnBodyRadians( float angle )
{
    setTurnBody( angle * Utils::toDegrees );
}

void Robot::setTurnRadar( float angle )
{
    m_currentCommands.setRadarTurnRemaining( angle );
}

void Robot::setTurnRadarRadians( float angle )
{
    setTurnRadar( angle * Utils::toDegrees );
}

float Robot::rotate( float angle )
{
    float max_angle = Rules::getTurnRate(m_velocity);
    if( std::abs(angle) > max_angle )
        angle = max_angle * std::abs(angle) / angle;

    if( !m_currentCommands.isAdjustGunForBodyTurn() )
    {
        rotateTurret( angle );
    }
    
    m_bodyPosition.rotate( angle );

    return angle;
}

float Robot::rotateTurret( float angle )
{
    if( std::abs(angle) > Rules::GUN_TURN_RATE )
        angle = Rules::GUN_TURN_RATE * std::abs(angle) / angle;

    if( !m_currentCommands.isAdjustRadarForGunTurn() )
    {
        rotateRadar( angle );
    }
    
    m_turretPosition.rotate( angle );

    return angle;
}

float Robot::rotateRadar( float angle )
{
    if( std::abs(angle) > Rules::RADAR_TURN_RATE )
        angle = Rules::RADAR_TURN_RATE * ( (angle > 0) - (angle < 0) );
    
    m_radarPosition.rotate( angle );

    return angle;
}

void Robot::setMove( double distance )
{
    if( getEnergy() == 0 )
        return;

    m_currentCommands.setDistanceRemaining(distance);
    m_currentCommands.setMoved(true);
}

void Robot::setMaxVelocity( double newVelocity )
{
    m_currentCommands.setMaxVelocity( newVelocity );
}

void Robot::setAhead( double distance )
{
    setMove( distance );
}

void Robot::setBack( double distance )
{
    setMove( -distance );
}

void Robot::setFire(double power)
{
    if( std::isnan(power) )
    {
        std::cerr << "SYSTEM: You cannot call fire(NaN)" << std::endl;
        return;
    }
    
    if( m_gunHeat > 0 || m_energy == 0)
        return;

    double firePower = std::min( m_energy,
            std::min( std::max( power, Rules::MIN_BULLET_POWER), Rules::MAX_BULLET_POWER ) );

    updateEnergy( -firePower );

    m_gunHeat += Rules::getGunHeat(firePower);

    Bullet newBullet( this );

    newBullet.setPower(firePower);
    newBullet.setHeading(getTurretAngle());
    newBullet.setX(getX());
    newBullet.setY(getY());

    m_world.addBullet( newBullet );
}

void Robot::drainEnergy()
{
    setEnergy(0, true);
    m_isEnergyDrained = true;
}

void Robot::setEnergy( double newEnergy, bool resetInactiveTurnCount )
{
    if( resetInactiveTurnCount && ( m_energy != newEnergy ) )
    {
		m_inactiveTurnCount = 0;
	}

    m_energy = newEnergy;
    if (m_energy < .01)
    {
        m_energy = 0;
        m_currentCommands.setDistanceRemaining(0);
        m_currentCommands.setBodyTurnRemaining(0);
    }
}

void Robot::updateGunHeat()
{
    m_gunHeat -= 0.1;
    if (m_gunHeat < 0) {
        m_gunHeat = 0;
    }
}

void Robot::updateEnergy(double delta)
{
    if ((!m_isExecFinishedAndDisabled && !m_isEnergyDrained) || delta < 0)
    {
        setEnergy(m_energy + delta, true);
    }
}

void Robot::updateHeading()
{
    auto rotatedAngle = rotate( m_currentCommands.getBodyTurnRemaining() );

    m_currentCommands.setBodyTurnRemaining( m_currentCommands.getBodyTurnRemaining() - rotatedAngle );

    if( m_currentCommands.getBodyTurnRemaining() < 0.01 ) // yuk
        m_currentCommands.setBodyTurnRemaining( 0 );
}

void Robot::updateGunHeading()
{
    auto rotatedAngle = rotateTurret( m_currentCommands.getGunTurnRemaining() );

    m_currentCommands.setGunTurnRemaining( m_currentCommands.getGunTurnRemaining() - rotatedAngle );

    if( m_currentCommands.getGunTurnRemaining() < 0.01 ) // yuk
        m_currentCommands.setGunTurnRemaining( 0 );
}

void Robot::updateRadarHeading()
{
    auto rotatedAngle = rotateRadar( m_currentCommands.getRadarTurnRemaining() );

    m_currentCommands.setRadarTurnRemaining( m_currentCommands.getRadarTurnRemaining() - rotatedAngle );

    if( m_currentCommands.getRadarTurnRemaining() < 0.01 ) // yuk
        m_currentCommands.setRadarTurnRemaining( 0 );
}

/**
 * Updates the robots movement.
 *
 * This is Nat Pavasants method described here:
 *   http://robowiki.net/wiki/User:Positive/Optimal_Velocity#Nat.27s_updateMovement
 */
void Robot::updateMovement()
{
    double distance = m_currentCommands.getDistanceRemaining();

    if( std::isnan(distance) )
        distance = 0;

    m_velocity = getNewVelocity( m_velocity, distance );

    // If we are over-driving our distance and we are now at velocity=0
    // then we stopped.
    if( Utils::isNear(m_velocity, 0) && m_isOverDriving )
    {
        m_currentCommands.setDistanceRemaining(0);
        distance = 0;
        m_isOverDriving = false;
    }

    // If we are moving normally and the breaking distance is more
    // than remaining distance, enabled the overdrive flag.
    if (Utils::signum(distance * m_velocity) != -1) {
        if (getDistanceTraveledUntilStop(m_velocity) > std::abs(distance)) {
            m_isOverDriving = true;
        } else {
            m_isOverDriving = false;
        }
    }

    m_currentCommands.setDistanceRemaining(distance - m_velocity);

    if (m_velocity != 0) {
        double x = m_velocity * std::sin(m_bodyPosition.getRotation() * Utils::toRadians);
        double y = m_velocity * std::cos(m_bodyPosition.getRotation() * Utils::toRadians);
        m_bodyPosition.move( x, y );
        updateBoundingBox();
    }
}

std::string Robot::getNameForEvent( Robot* otherRobot )
{/*
    if( Rules::getHideEnemyNames() && !isTeamMate( otherRobot ) ) {
        return otherRobot->getAnnonymousName();
    }*/
    return otherRobot->getName();
}		

bool Robot::isCollidingRobot( std::list<Robot*> robots )
{
    for( Robot* otherRobot : robots )
    {
        if( !( otherRobot == nullptr || otherRobot == this || otherRobot->isDead() )
                && m_boundingBox.intersects( otherRobot->m_boundingBox ) )
        {
            return true;
        }
    }

    return false;
}

void Robot::checkRobotCollision( std::list<Robot*> robots )
{
    m_inCollision = false;

    for( Robot* otherRobot : robots )
    {
        if( !( otherRobot == nullptr || otherRobot == this || otherRobot->isDead() )
                && m_boundingBox.intersects( otherRobot->m_boundingBox ) )
        {
            // Bounce back
            double angle = std::atan2( otherRobot->m_bodyPosition.getPosition().x - m_bodyPosition.getPosition().x,
                                       otherRobot->m_bodyPosition.getPosition().y - m_bodyPosition.getPosition().y);

            double movedx = m_velocity * std::sin( getBodyHeading() );
            double movedy = m_velocity * std::cos( getBodyHeading() );

            bool atFault;
            double bearing = Utils::normalRelativeAngle( angle - getBodyHeading() );

            if( ( m_velocity > 0 && bearing > -Utils::PI / 2 && bearing < Utils::PI / 2 )
                    || ( m_velocity < 0 && ( bearing < -Utils::PI / 2 || bearing > Utils::PI / 2 ) ) )
            {

                m_inCollision = true;
                atFault = true;
                m_velocity = 0;
                m_currentCommands.setDistanceRemaining( 0 );
                m_bodyPosition.move( -movedx, -movedy );

                m_statistics.scoreRammingDamage( otherRobot->getName() );

                updateEnergy( -Rules::ROBOT_HIT_DAMAGE );
                otherRobot->updateEnergy( -Rules::ROBOT_HIT_DAMAGE );

                if( otherRobot->m_energy == 0 )
                {
                    if( otherRobot->isAlive() )
                    {
                        otherRobot->kill();
                        double bonus = m_statistics.scoreRammingKill( otherRobot->getName() );

                        if( bonus > 0 )
                        {
                            std::cout << 
                                    "SYSTEM: Ram bonus for killing " << getNameForEvent( otherRobot ) << ": "
                                    << (int) (bonus + .5) << std::endl;
                        }
                    }
                }
                addEvent(
                        std::make_unique<HitRobotEvent>( getNameForEvent( otherRobot ),
                        Utils::normalRelativeAngle( angle - getBodyHeading() ),
                        otherRobot->m_energy, atFault ) );

                otherRobot->addEvent(
                        std::make_unique<HitRobotEvent>( getNameForEvent(this),
                        Utils::normalRelativeAngle( Utils::PI + angle - otherRobot->getBodyHeading() ),
                        m_energy, false ) );
            }
        }
    }
    if( m_inCollision )
    {
        setState( RobotState::HIT_ROBOT );
    }
}

void Robot::checkWallCollision()
{
    int minX = 0 + HALF_WIDTH_OFFSET;
    int minY = 0 + HALF_HEIGHT_OFFSET;
    int maxX = (int) m_world.getWidth() - HALF_WIDTH_OFFSET;
    int maxY = (int) m_world.getHeight() - HALF_HEIGHT_OFFSET;

    bool hitWall = false;
    double adjustX = 0, adjustY = 0;
    double angle = 0;

    if( m_bodyPosition.getPosition().x < minX)
    {
        hitWall = true;
        adjustX = minX - m_bodyPosition.getPosition().x;
        angle = Utils::normalRelativeAngle( 3 * Utils::PI / 2 - getBodyHeading() );

    }
    else if ( m_bodyPosition.getPosition().x > maxX )
    {
        hitWall = true;
        adjustX = maxX - m_bodyPosition.getPosition().x;
        angle = Utils::normalRelativeAngle( Utils::PI / 2 - getBodyHeading() );

    }
    else if ( m_bodyPosition.getPosition().y < minY )
    {
        hitWall = true;
        adjustY = minY - m_bodyPosition.getPosition().y;
        angle = Utils::normalRelativeAngle( Utils::PI - getBodyHeading() );

    }
    else if ( m_bodyPosition.getPosition().y > maxY )
    {
        hitWall = true;
        adjustY = maxY - m_bodyPosition.getPosition().y;
        angle = Utils::normalRelativeAngle( -getBodyHeading() );
    }

    if (hitWall)
    {
        addEvent( std::make_unique<HitWallEvent>( angle ) );

        // only fix both x and y values if hitting wall at an angle
        if( ( int(getBodyHeading()) % int( Utils::PI / 2 ) ) != 0)
        {
            double tanHeading = tan( getBodyHeading() );

            // if it hits bottom or top wall
            if (adjustX == 0)
            {
                adjustX = adjustY * tanHeading;
            } // if it hits a side wall
            else if (adjustY == 0)
            {
                adjustY = adjustX / tanHeading;
            } // if the robot hits 2 walls at the same time (rare, but just in case)
            else if (std::abs(adjustX / tanHeading) > std::abs(adjustY))
            {
                adjustY = adjustX / tanHeading;
            } else if (std::abs(adjustY * tanHeading) > std::abs(adjustX))
            {
                adjustX = adjustY * tanHeading;
            }
        }
        m_bodyPosition.move( adjustX, adjustY );

        if( m_bodyPosition.getPosition().x < minX )
        {
            m_bodyPosition.setPosition( minX, m_bodyPosition.getPosition().y );
        }
        else if( m_bodyPosition.getPosition().x > maxX )
        {
            m_bodyPosition.setPosition( maxX, m_bodyPosition.getPosition().y );
        }
        if ( m_bodyPosition.getPosition().y < minY )
        {
            m_bodyPosition.setPosition( m_bodyPosition.getPosition().x, minY );
        }
        else if( m_bodyPosition.getPosition().y > maxY )
        {
            m_bodyPosition.setPosition( m_bodyPosition.getPosition().x, maxY );
        }

        // Update energy, but do not reset inactiveTurnCount
        setEnergy( m_energy - Rules::getWallHitDamage(m_velocity), false );

        updateBoundingBox();

        m_currentCommands.setDistanceRemaining(0);
        m_velocity = 0;

        setState( RobotState::HIT_WALL );
    }
}

double Robot::getDistanceTraveledUntilStop(double velocity)
{
    double distance = 0;

    velocity = std::abs(velocity);
    while (velocity > 0) {
        distance += (velocity = getNewVelocity(velocity, 0));
    }
    return distance;
}

double Robot::getNewVelocity( double velocity, double distance )
{
    if( distance < 0 )
    {
        // If the distance is negative, then change it to be positive
        // and change the sign of the input velocity and the result
        return -getNewVelocity( -velocity, -distance );
    }

    double goalVel;

    if( distance == std::numeric_limits<double>::infinity() )
        goalVel = m_currentCommands.getMaxVelocity();
    else
        goalVel = std::min( getMaxVelocity(distance), m_currentCommands.getMaxVelocity() );

    if( velocity >= 0 )
        return std::max( velocity - Rules::DECELERATION, std::min(goalVel, velocity + Rules::ACCELERATION) );
    
    // else
    return std::max( velocity - Rules::ACCELERATION, std::min(goalVel, velocity + maxDecel(-velocity)) );
}

double Robot::getMaxVelocity(double distance)
{
	const double decelTime = std::max(1., std::ceil(// sum of 0... decelTime, solving for decelTime using quadratic formula
            (std::sqrt((4 * 2 / Rules::DECELERATION) * distance + 1) - 1) / 2));

    if (decelTime == std::numeric_limits<double>::infinity())
        return Rules::MAX_VELOCITY;

    const double decelDist = (decelTime / 2.0) * (decelTime - 1) // sum of 0..(decelTime-1)
            * Rules::DECELERATION;

    return ((decelTime - 1) * Rules::DECELERATION) + ((distance - decelDist) / decelTime);
}

double Robot::maxDecel(double speed)
{
    double decelTime = speed / Rules::DECELERATION;
    double accelTime = (1 - decelTime);

    return std::min(1., decelTime) * Rules::DECELERATION + std::max(0., accelTime) * Rules::ACCELERATION;
}

void Robot::updateBoundingBox()
{
    m_boundingBox = sf::FloatRect( getX() - HALF_WIDTH_OFFSET, getY() - HALF_HEIGHT_OFFSET, WIDTH, HEIGHT);
}

void Robot::tick()
{
    processEvents();
    
    run();

    performMove();

    performScan();

	m_inactiveTurnCount++;
}

void Robot::processEvents()
{
    for( auto& event : m_events )
    {
        if( auto ptr = dynamic_cast<BulletHitBulletEvent*>( event.get() ) )
        {
            onBulletHitBullet( ptr );
        }
        if( auto ptr = dynamic_cast<DeathEvent*>( event.get() ) )
        {
            onDeath( ptr );
        }
        if( auto ptr = dynamic_cast<HitRobotEvent*>( event.get() ) )
        {
            onHitRobot( ptr );
        }
        if( auto ptr = dynamic_cast<HitWallEvent*>( event.get() ) )
        {
            onHitWall( ptr );
        }
        if( auto ptr = dynamic_cast<ScannedRobotEvent*>( event.get() ) )
        {
            onScannedRobot( ptr );
        }
        if( auto ptr = dynamic_cast<RoundEndedEvent*>( event.get() ) )
        {
            onRoundEnded( ptr );
        }
        if( auto ptr = dynamic_cast<BattleEndedEvent*>( event.get() ) )
        {
            onBattleEnded( ptr );
        }
    }

    m_events.clear();
}

void Robot::performMove()
{
    if( isDead() )
		return;

	setState( RobotState::ACTIVE );

    // public final void performMove(List<RobotPeer> robots, double zapEnergy) {
	updateGunHeat();

    m_lastHeading = getBodyHeading();
    m_lastGunHeading = getTurretHeading();
    m_lastRadarHeading = getRadarHeading();
    double lastX = getX();
    double lastY = getY();

    if( !m_inCollision )
        updateHeading();

    updateGunHeading();
    updateRadarHeading();
    updateMovement();

    // At this point, robot has turned then moved.
    // We could be touching a wall or another bot...

    // First and foremost, we can never go through a wall:
    checkWallCollision();

    // Now check for robot collision
    checkRobotCollision( m_world.getRobots() );

    // Scan false means robot did not call scan() manually.
    // But if we're moving, scan
    if( !m_scan )
    {
        m_scan = ( m_lastHeading != getBodyHeading() || m_lastGunHeading != getTurretHeading() || m_lastRadarHeading != getRadarHeading()
                || lastX != getX() || lastY != getY() );
    }

    if (isDead()) {
        return;
    }

    // zap
    bool bZap = ( m_inactiveTurnCount > 200 );
    const double zapEnergy = bZap ? .1 : 0;
    if( zapEnergy != 0 )
    {
        zap( zapEnergy );
    }
}

void Robot::performScan()
{
    if( isDead() )
    {
        return;
    }

    m_turnedRadarWithGun = false;
 
    // scan
    if( m_scan )
    {
        scan( m_lastRadarHeading, m_world.getRobots() );
        m_turnedRadarWithGun = ( m_lastGunHeading == m_lastRadarHeading ) && ( getTurretHeading() == getRadarHeading() );
        m_scan = false;
    }

    //m_currentCommands = ExecCommands();
    m_lastHeading = -1;
    m_lastGunHeading = -1;
    m_lastRadarHeading = -1;
}

void Robot::addEvent( std::unique_ptr<Event> evt )
{
    m_events.push_back( std::move( evt ) );
}

void Robot::scan( double lastRadarHeading, std::list<Robot*> robots )
{
    double startAngle = lastRadarHeading;
    double scanRadians = getRadarHeading() - startAngle;

    // Check if we passed through 360
    if (scanRadians < -Utils::PI) {
        scanRadians = 2 * Utils::PI + scanRadians;
    } else if (scanRadians > Utils::PI) {
        scanRadians = scanRadians - 2 * Utils::PI;
    }

    // In our coords, we are scanning clockwise, with +y up
    // In java coords, we are scanning counterclockwise, with +y down
    // All we need to do is adjust our angle by -90 for this to work.
    //startAngle -= Utils::PI / 2;

    startAngle = Utils::normalAbsoluteAngle( startAngle );

    m_scanArc = Arc2D( getX(), getY(), Rules::RADAR_SCAN_RADIUS, startAngle, scanRadians );

    for( Robot* otherRobot : robots)
    {
        if ( !(otherRobot == nullptr || otherRobot == this || otherRobot->isDead())
                && intersects( m_scanArc, otherRobot->m_boundingBox ) )
        {
            double dx = otherRobot->getX() - getX();
            double dy = otherRobot->getY() - getY();
            double angle = atan2(dx, dy);
            double dist = std::hypot(dx, dy);

            addEvent(
                    std::make_unique<ScannedRobotEvent>( getNameForEvent( otherRobot ),
                    otherRobot->m_energy,
                    Utils::normalRelativeAngle( angle - getBodyHeading() ), dist, otherRobot->getBodyHeading(),
                    otherRobot->getVelocity()) );
        }
    }
}

bool Robot::intersects( Arc2D arc, sf::FloatRect rect )
{
    return /*(rect.intersectsLine(arc.getCenterX(), arc.getCenterY(), arc.getStartPoint().getX(),
            arc.getStartPoint().getY()))
            || */ arc.intersects( rect );
}

void Robot::zap( double zapAmount )
{
    if( m_energy == 0 )
    {
        kill();
        return;
    }

    m_energy -= std::abs( zapAmount );
    if( m_energy < .1 )
    {
        m_energy = 0;
        m_currentCommands.setDistanceRemaining( 0 );
        m_currentCommands.setBodyTurnRemaining( 0 );
    }
}

void Robot::kill()
{
    //battle.resetInactiveTurnCount(10.0);
    if( isAlive() )
    {
        addEvent( std::make_unique<DeathEvent>() );

        //battle.registerDeathRobot(this);

        // 'fake' bullet for explosion on self
        Bullet fakeBullet( this );

        fakeBullet.setPower( 1 );
        fakeBullet.setHeading(0);
        fakeBullet.setX(getX());
        fakeBullet.setY(getY());

        m_world.addBullet( fakeBullet );
    }
    updateEnergy(-m_energy);

    setState( RobotState::DEAD );
}