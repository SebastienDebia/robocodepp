#include "SpinRobot.hpp"

#include "../World.hpp"

void SpinRobot::run()
{
    setTurnBody( m_bodySpin );
    setTurnGun( m_gunSpin );
    setTurnRadar( m_radarSpin );
    setAhead(10);

    if( getWorld().getTurn() % 2 == 0 )
        setFire( 2 );
}