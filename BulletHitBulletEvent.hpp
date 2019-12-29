#pragma once

#include "Event.hpp"
#include "Bullet.hpp"

class BulletHitBulletEvent : public Event
{
public:
    BulletHitBulletEvent( Bullet bullet, Bullet hitBullet );

	/**
	 * Returns your bullet that hit another bullet.
	 *
	 * @return your bullet
	 */
	Bullet getBullet()
    {
		return m_bullet;
	}

	/**
	 * Returns the bullet that was hit by your bullet.
	 *
	 * @return the bullet that was hit
	 */
	Bullet getHitBullet()
    {
		return m_hitBullet;
	}

private:
    Bullet m_bullet;
    Bullet m_hitBullet;
};