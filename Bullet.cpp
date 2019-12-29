#include "Bullet.hpp"

#include "World.hpp"
#include "Robot.hpp"
#include "BulletHitBulletEvent.hpp"
//#include "HitByBulletEvent.hpp"

#include "Utils.hpp"

#include <memory>

bool intersects( sf::FloatRect rect, Line2D line )
{
    double a_rectangleMinX = rect.left;
    double a_rectangleMinY = rect.top;
    double a_rectangleMaxX = rect.left + rect.width;
    double a_rectangleMaxY = rect.top + rect.height;

    double a_p1x = line.x1;
    double a_p1y = line.y1;
    double a_p2x = line.x2;
    double a_p2y = line.y2;

    // Find min and max X for the segment

    double minX = a_p1x;
    double maxX = a_p2x;

    if(a_p1x > a_p2x)
    {
        minX = a_p2x;
        maxX = a_p1x;
    }

    // Find the intersection of the segment's and rectangle's x-projections

    if(maxX > a_rectangleMaxX)
    {
        maxX = a_rectangleMaxX;
    }

    if(minX < a_rectangleMinX)
    {
        minX = a_rectangleMinX;
    }

    if(minX > maxX) // If their projections do not intersect return false
    {
        return false;
    }

    // Find corresponding min and max Y for min and max X we found before

    double minY = a_p1y;
    double maxY = a_p2y;

    double dx = a_p2x - a_p1x;

    if(std::abs(dx) > 0.0000001)
    {
        double a = (a_p2y - a_p1y) / dx;
        double b = a_p1y - a * a_p1x;
        minY = a * minX + b;
        maxY = a * maxX + b;
    }

    if(minY > maxY)
    {
        double tmp = maxY;
        maxY = minY;
        minY = tmp;
    }

    // Find the intersection of the segment's and rectangle's y-projections

    if(maxY > a_rectangleMaxY)
    {
        maxY = a_rectangleMaxY;
    }

    if(minY < a_rectangleMinY)
    {
        minY = a_rectangleMinY;
    }

    if(minY > maxY) // If Y-projections do not intersect return false
    {
        return false;
    }

    return true;
}

std::size_t Bullet::s_globalBulletId = 0;

int Bullet::getExplosionImageIndex() {
    return m_explosionImageIndex;
}
/*
std::string toString() {
    return getm_owner().getName() + " V" + getVelocity() + " *" + (int) m_power + " X" + (int) x + " Y" + (int) y + " H"
            + m_heading + " " + state.toString();
}*/

int Bullet::getBulletId() {
    return m_bulletId;
}

int Bullet::getFrame() {
    return m_frame;
}

double Bullet::getHeading() {
    return m_heading;
}

const Robot* Bullet::getOwner() {
    return m_owner;
}

double Bullet::getPower() {
    return m_power;
}

double Bullet::getVelocity() {
    return Rules::getBulletSpeed(m_power);
}

Robot* Bullet::getVictim() {
    return m_victim;
}

double Bullet::getX() {
    return m_x;
}

double Bullet::getY() {
    return m_y;
}

double Bullet::getPaintX() {
    return (m_state == HIT_VICTIM && m_victim != nullptr) ? m_victim->getX() + m_deltaX : m_x;
}

double Bullet::getPaintY() {
    return (m_state == HIT_VICTIM && m_victim != nullptr) ? m_victim->getY() + m_deltaY : m_y;
}

bool Bullet::isActive() const {
    return m_state == FIRED || m_state == MOVING;
}

Bullet::BulletState Bullet::getState() const {
    return m_state;
}

sf::Color Bullet::getColor() const {
    return m_color;
}

void Bullet::setHeading(double newHeading) {
    m_heading = newHeading;
}

void Bullet::setPower(double newPower) {
    m_power = newPower;
}

void Bullet::setm_victim(Robot* newVictim) {
    m_victim = newVictim;
}

void Bullet::setX(double newX) {
    m_x = m_lastX = newX;
}

void Bullet::setY(double newY) {
    m_y = m_lastY = newY;
}

void Bullet::setState(BulletState newState) {
    m_state = newState;
}

void Bullet::update(std::list<Robot*> robots, std::list<Bullet> bullets) {
    m_frame++;
    if (isActive()) {
        updateMovement();
        checkWallCollision();
        if (isActive()) {
            checkRobotCollision(robots);
        }
        if (isActive()) {
            checkBulletCollision(bullets);
        }
    }
    updateBulletState();
    //m_owner->addBulletStatus(createStatus());
}

Bullet::Bullet(Robot* owner)
: m_world( &owner->getWorld() ),
m_bulletId( ++s_globalBulletId )
{
    m_owner = owner;
    m_state = FIRED;
    m_color = m_owner->getBulletColor(); // Store current bullet color set on robot
}

void Bullet::checkBulletCollision(std::list<Bullet> bullets) {
    for( auto&& b : bullets )
    {
        if (b.getBulletId() != getBulletId() && b.m_owner != m_owner && b.isActive() && intersect(b.m_boundingLine)) {
            m_state = HIT_BULLET;
            m_frame = 0;
            m_x = m_lastX;
            m_y = m_lastY;

            b.m_state = HIT_BULLET;
            b.m_frame = 0;
            b.m_x = b.m_lastX;
            b.m_y = b.m_lastY;

            // Bugfix #366
            m_owner->addEvent( std::make_unique<BulletHitBulletEvent>( *this, b ) );
            b.m_owner->addEvent( std::make_unique<BulletHitBulletEvent>( b, *this ) );
            break;
        }
    }
}

/*
Bullet Bullet::createBullet(bool hidem_ownerName) {
    std::string m_ownerName = (m_owner == nullptr) ? nullptr : (hidem_ownerName ? getNameForEvent(m_owner) : m_owner->getName());
    std::string m_victimName = (m_victim == nullptr) ? nullptr : (hidem_ownerName ? m_victim.getName() : getNameForEvent(m_victim));

    return new Bullet(m_heading, m_x, m_y, m_power, m_ownerName, m_victimName, isActive(), m_bulletId);
}

BulletStatus Bullet::createStatus() {
    return new BulletStatus(m_bulletId, m_x, m_y, m_victim == nullptr ? nullptr : getNameForEvent(m_victim), isActive());
}

std::string getNameForEvent(Robot& otherRobot) {
    if (battleRules.getHideEnemyNames() && !m_owner->isTeamMate(otherRobot)) {
        return otherRobot->getAnnonymousName();
    }
    return otherRobot->getName();
}
*/
// Workaround for http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6457965
bool Bullet::intersect(Line2D line) {
    double x1 = line.x1, x2 = line.x2, x3 = m_boundingLine.x1, x4 = m_boundingLine.x2;
    double y1 = line.y1, y2 = line.y2, y3 = m_boundingLine.y1, y4 = m_boundingLine.y2;

    double dx13 = (x1 - x3), dx21 = (x2 - x1), dx43 = (x4 - x3);
    double dy13 = (y1 - y3), dy21 = (y2 - y1), dy43 = (y4 - y3);

    double dn = dy43 * dx21 - dx43 * dy21;

    double ua = (dx43 * dy13 - dy43 * dx13) / dn;
    double ub = (dx21 * dy13 - dy21 * dx13) / dn;

    return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}

void Bullet::checkRobotCollision(std::list<Robot*> robots) {
    for (Robot* otherRobot : robots) {
        if (!(otherRobot == nullptr || otherRobot == m_owner || otherRobot->isDead())
                && intersects( otherRobot->getBoundingBox(), m_boundingLine )) {

            m_state = HIT_VICTIM;
            m_frame = 0;
            m_victim = otherRobot;

            double damage = Rules::getBulletDamage(m_power);

            double score = damage;
            if (score > otherRobot->getEnergy()) {
                score = otherRobot->getEnergy();
            }
            otherRobot->updateEnergy(-damage);

            if (otherRobot->getEnergy() <= 0 && otherRobot->isAlive()) {
                otherRobot->kill();
            }

            m_owner->updateEnergy( Rules::getBulletHitBonus( m_power ) );

            /*
            otherRobot->addEvent(
                    std::make_unique<HitByBulletEvent>(
                            robocode.util.Utils.normalRelativeAngle(m_heading + Math.PI - otherRobot->getBodyHeading()),
                            createBullet(true))); // Bugfix #366

            m_owner->addEvent(
                    new BulletHitEvent(m_owner->getNameForEvent(otherRobot), otherRobot->getEnergy(), createBullet(false))); // Bugfix #366
            */

            double newX, newY;

            if (otherRobot->getBoundingBox().contains(m_lastX, m_lastY)) {
                newX = m_lastX;
                newY = m_lastY;

                setX(newX);
                setY(newY);
            } else {
                newX = m_x;
                newY = m_y;
            }

            m_deltaX = newX - otherRobot->getX();
            m_deltaY = newY - otherRobot->getY();

            break;
        }
    }
}

void Bullet::checkWallCollision() {
    if ((m_x - RADIUS <= 0) || (m_y - RADIUS <= 0) || (m_x + RADIUS >= m_world->getWidth())
            || (m_y + RADIUS >= m_world->getHeight())) {
        m_state = HIT_WALL;
        m_frame = 0;
        //m_owner->addEvent(new BulletMissedEvent(createBullet(false))); // Bugfix #366
    }
}

void Bullet::updateBulletState() {
    switch (m_state) {
    case FIRED:
        // Note that the bullet must be in the FIRED state before it goes to the MOVING state
        if (m_frame > 0) {
            m_state = MOVING;
        }
        break;

    case HIT_BULLET:
    case HIT_VICTIM:
    case HIT_WALL:
    case EXPLODED:
        // Note that the bullet explosion must be ended before it goes into the INACTIVE state
        if (m_frame >= getExplosionLength()) {
            m_state = INACTIVE;
        }
        break;
    default:
        break;
    }
}

void Bullet::updateMovement() {
    m_lastX = m_x;
    m_lastY = m_y;

    double v = getVelocity();

    m_x += v * sin(m_heading * Utils::toRadians);
    m_y += v * cos(m_heading * Utils::toRadians);

    m_boundingLine.setLine(m_lastX, m_lastY, m_x, m_y);
}

int Bullet::getExplosionLength() {
    return EXPLOSION_LENGTH;
}