#pragma once

#include "Rules.hpp"

#include <limits>
#include <algorithm>

#include <SFML/Graphics/Color.hpp>

struct ExecCommands
{
    ExecCommands()
    : m_bodyColor( 0x29298CFF ),
    m_radarColor( 0x29298CFF ),
    m_gunColor( 0x29298CFF ),
    m_bulletColor( sf::Color::White ),
    m_scanColor( sf::Color::Blue ),
    m_bodyTurnRemaining( 0 ),
    m_radarTurnRemaining( 0 ),
    m_gunTurnRemaining( 0 ),
	m_isAdjustGunForBodyTurn( false ),
	m_isAdjustRadarForGunTurn( false ),
    m_moved( false ),
    m_scan( false ),
    m_distanceRemaining( 0 )
    {
        setMaxVelocity( std::numeric_limits<double>::infinity() );
        setMaxTurnRate( std::numeric_limits<double>::infinity() );
    }

    sf::Color getBodyColor() {
		return m_bodyColor;
	}

	sf::Color getRadarColor() {
		return m_radarColor;
	}

	sf::Color getGunColor() {
		return m_gunColor;
	}

	sf::Color getBulletColor() {
		return m_bulletColor;
	}

	sf::Color getScanColor() {
		return m_scanColor;
	}

	void setBodyColor( sf::Color color ) {
		m_bodyColor = color;
	}

    void setRadarColor( sf::Color color ) {
		m_radarColor = color;
	}

	void setGunColor( sf::Color color ) {
		m_gunColor = color;
	}

	void setBulletColor( sf::Color color ) {
		m_bulletColor = color;
	}

	void setScanColor( sf::Color color ) {
		m_scanColor = color;
	}

	double getBodyTurnRemaining() {
		return m_bodyTurnRemaining;
	}

	void setBodyTurnRemaining(double bodyTurnRemaining) {
		m_bodyTurnRemaining = bodyTurnRemaining;
	}

	double getRadarTurnRemaining() {
		return m_radarTurnRemaining;
	}

	void setRadarTurnRemaining(double radarTurnRemaining) {
		m_radarTurnRemaining = radarTurnRemaining;
	}

	double getGunTurnRemaining() {
		return m_gunTurnRemaining;
	}

	void setGunTurnRemaining(double gunTurnRemaining) {
		m_gunTurnRemaining = gunTurnRemaining;
	}

    double getDistanceRemaining()
    {
        return m_distanceRemaining;
    }

    void setDistanceRemaining(double distanceRemaining)
    {
        m_distanceRemaining = distanceRemaining;
    }

	bool isAdjustGunForBodyTurn()
    {
		return m_isAdjustGunForBodyTurn;
	}

	void setAdjustGunForBodyTurn( bool adjustGunForBodyTurn )
    {
		m_isAdjustGunForBodyTurn = adjustGunForBodyTurn;
	}

	bool isAdjustRadarForGunTurn()
    {
		return m_isAdjustRadarForGunTurn;
	}

	void setAdjustRadarForGunTurn( bool adjustRadarForGunTurn )
    {
		m_isAdjustRadarForGunTurn = adjustRadarForGunTurn;
	}
    
    double getMaxTurnRate()
    {
        return m_maxTurnRate;
    }

    void setMaxTurnRate(double maxTurnRate)
    {
        m_maxTurnRate = std::min(std::abs(maxTurnRate), Rules::MAX_TURN_RATE_RADIANS);
    }

    double getMaxVelocity()
    {
        return m_maxVelocity;
    }

    void setMaxVelocity(double maxVelocity)
    {
        m_maxVelocity = std::min(std::abs(maxVelocity), Rules::MAX_VELOCITY);
    }

	void setMoved(bool moved) {
		m_moved = moved;
	}

	bool isScan() {
		return m_scan;
	}

	void setScan(bool scan) {
		m_scan = scan;
	}

private:
    sf::Color m_bodyColor;
    sf::Color m_radarColor;
    sf::Color m_gunColor;
    sf::Color m_bulletColor;
    sf::Color m_scanColor;

    double m_bodyTurnRemaining;
    double m_radarTurnRemaining;
    double m_gunTurnRemaining;

	bool m_isAdjustGunForBodyTurn;
	bool m_isAdjustRadarForGunTurn;

    bool m_moved;
    bool m_scan;

    double m_distanceRemaining;
    double m_maxVelocity;
    double m_maxTurnRate;
};