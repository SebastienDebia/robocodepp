#include "HitRobotEvent.hpp"

HitRobotEvent::HitRobotEvent( std::string name, double bearing, double energy, bool atFault )
 : m_robotName( name ),
 m_bearing( bearing ),
 m_energy( energy ),
 m_atFault( atFault )
{
}