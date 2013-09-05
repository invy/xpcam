
#pragma once

#include "siminterface/simdata.h"

template<typename TD>
class GroundInfluence {
    void *m_gearNormal;
    void *m_totalWeight;

public:
    GroundInfluence()
    {
        std:: cout << __func__ << std::endl;
        m_gearNormal = SimData<TD>::getInstance().requireDataRef(std::string("sim/flightmodel/forces/fnrml_gear"));
        m_totalWeight = SimData<TD>::getInstance().requireDataRef(std::string("sim/flightmodel/weight/m_total"));
    }

    float getGroundInfluence(float gearNormal, float totalWeight)
    {
        // sim/flightmodel/forces/fnrml_gear
        if(gearNormal < 1e-05)
           return 0;
        else
            return 1.0f - exp(-gearNormal/totalWeight);
    }

    float operator()(void) {
        float gearNormal = SimData<TD>::getInstance().m_dataAccess.getDataf(m_gearNormal);
        float totalWeight = SimData<TD>::getInstance().m_dataAccess.getDataf(m_totalWeight);
        return getGroundInfluence(gearNormal, totalWeight);
    }

};


