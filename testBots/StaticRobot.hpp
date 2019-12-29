#pragma once

#include "../Robot.hpp"

class StaticRobot : public Robot
{
public:
    StaticRobot( World& world,
        int x,
        unsigned y,
        double bodySpin = 0,
        double gunSpin = 0,
        double radarSpin = 0 )
     : Robot( world, "StaticRobot", x, y ),
        m_bodySpin( bodySpin ),
        m_gunSpin( gunSpin ),
        m_radarSpin( radarSpin )
    {
    }

    void run();

private:
    double m_bodySpin;
    double m_gunSpin;
    double m_radarSpin;
};