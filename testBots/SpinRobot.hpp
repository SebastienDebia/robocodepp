#pragma once

#include "../Robot.hpp"

class SpinRobot : public Robot
{
public:
    SpinRobot( World& world,
        int x,
        unsigned y,
        double bodySpin = 360 * 1/180,
        double gunSpin = 360 * -1./120,
        double radarSpin = 360 * 1./30 )
        : Robot( world, "SpinRobot", x, y ),
        m_bodySpin( bodySpin ),
        m_gunSpin( gunSpin ),
        m_radarSpin( radarSpin )
    {}

    void run();

private:
    double m_bodySpin;
    double m_gunSpin;
    double m_radarSpin;
};