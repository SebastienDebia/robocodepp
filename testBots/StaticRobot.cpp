#include "StaticRobot.hpp"

void StaticRobot::run()
{
    static bool start = true;
    
    if( start )
    {
        rotate( 42 );
        start = false;
    }
    setTurnBody( m_bodySpin );
    setTurnGun( m_gunSpin );
    setTurnRadar( m_radarSpin );
    setFire( 3 );
    //setEnergy( 100, true ); // never dies (for debugging)
}