#pragma once

#include "Rules.hpp"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include <string>
#include <list>
#include <algorithm>

class Robot;
class World;

struct Line2D
{
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

    void setLine( double nx1, double ny1, double nx2, double ny2 )
    {
        x1 = nx1;
        x2 = nx2;
        y1 = ny1;
        y2 = ny2;
    }
}; 

bool intersects( sf::FloatRect rect, Line2D line );

class Bullet
{
public:
    static std::size_t s_globalBulletId;   

    enum BulletState
    {
	    /** The bullet has just been fired this turn and hence just been created. This state only last one turn. */
        FIRED = 0,

        /** The bullet is now moving across the battlefield, but has not hit anything yet. */
        MOVING = 1,

        /** The bullet has hit a robot victim. */
        HIT_VICTIM = 2,

        /** The bullet has hit another bullet. */
        HIT_BULLET = 3,

        /** The bullet has hit the wall, i.e. one of the four borders of the battlefield. */
        HIT_WALL = 4,

        /** The bullet currently represents a robot explosion, i.e. a robot death. */
        EXPLODED = 5,

        /** The bullet is currently inactive. Hence, it is not active or visible on the battlefield. */
        INACTIVE = 6
    };


	Bullet(Robot* owner);

	int getExplosionImageIndex();
/*
	std::string toString() {
		return getm_owner().getName() + " V" + getVelocity() + " *" + (int) m_power + " X" + (int) x + " Y" + (int) y + " H"
				+ m_heading + " " + state.toString();
	}*/

	int getBulletId();

	int getFrame();

	double getHeading();

	const Robot* getOwner();

	double getPower();

	double getVelocity();

	Robot* getVictim();

	double getX();

	double getY();

	double getPaintX();

	double getPaintY();

	bool isActive() const;

	BulletState getState() const;

	sf::Color getColor() const;

	void setHeading(double newHeading);

	void setPower(double newPower);

	void setm_victim(Robot* newVictim);

	void setX(double newX);

	void setY(double newY);

	void setState(BulletState newState);

	void update(std::list<Robot*> robots, std::list<Bullet> bullets);

private:

	static constexpr int EXPLOSION_LENGTH = 17;

	static constexpr int RADIUS = 3;

	Robot* m_owner;
    World* m_world;

	int m_bulletId;

	Robot* m_victim;

	BulletState m_state;

	double m_heading;

	double m_x;
	double m_y;

	double m_lastX;
	double m_lastY;

	double m_power;

	double m_deltaX;
	double m_deltaY;

	Line2D m_boundingLine;

	int m_frame; // Do not set to -1

	sf::Color m_color;

	int m_explosionImageIndex; // Do not set to -1

	void checkBulletCollision(std::list<Bullet> bullets);

/*
	Bullet createBullet(bool hidem_ownerName) {
		std::string m_ownerName = (m_owner == nullptr) ? nullptr : (hidem_ownerName ? getNameForEvent(m_owner) : m_owner->getName());
		std::string m_victimName = (m_victim == nullptr) ? nullptr : (hidem_ownerName ? m_victim.getName() : getNameForEvent(m_victim));

		return new Bullet(m_heading, m_x, m_y, m_power, m_ownerName, m_victimName, isActive(), m_bulletId);
	}

	BulletStatus createStatus();

	std::string getNameForEvent(Robot& otherRobot) {
		if (battleRules.getHideEnemyNames() && !m_owner->isTeamMate(otherRobot)) {
			return otherRobot->getAnnonymousName();
		}
		return otherRobot->getName();
	}
*/
	// Workaround for http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6457965
	bool intersect(Line2D line);

	void checkRobotCollision(std::list<Robot*> robots);

	void checkWallCollision();

	void updateBulletState();

	void updateMovement();

	int getExplosionLength();
};
