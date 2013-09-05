#ifndef GLOADEFFECT_H
#define GLOADEFFECT_H

#include "siminterface/simdata.h"
#include "headcamera_helpers.h"
#include "groundshakingeffect.h"

#include "cameraeffect.h"

template<typename TD>
class GLoadEffect : public CameraEffect {
    float m_tolerance;
    float m_k;
    float m_dmax;

    void *m_gForceNormal;

    GroundInfluence<TD> m_groundInfluence;

public:
    GLoadEffect() : GLoadEffect(1, 1, 1) {}

    GLoadEffect(float tolerance, float k, float dmax)
     : m_tolerance(tolerance), m_k(k), m_dmax(dmax),
       m_groundInfluence()
    {
        std::cout << __func__ << "\n";
        m_gForceNormal = SimData<TD>::getInstance().requireDataRef(std::string("sim/flightmodel2/misc/gforce_normal"));
    }
    virtual void apply(CameraPosition &/*pos*/, CameraPosition &deltaPos, IDataAccess& pda)
    {
        float gNormal = pda.getDataf(m_gForceNormal);

        deltaPos.y += m_groundInfluence() * cameraGYDeflection(gNormal);
    }

protected:
    /*
     * calculate deflection for the Gy acceleration
     * tolerance, describes the minimum threshold, at which the effect should take place
     * k is the rigidness coefficient
     * dmax is the maximum available deflection
     */
    float cameraGYDeflection(float Gy) {
        if(Gy < m_tolerance)
            return 0;
        float def = (1-Gy) * m_k;
        if(def > m_dmax)
            return m_dmax;
        else if(def < -m_dmax)
            return -m_dmax;
        return def;
    }
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        std::cout << "g-load effect" << " -- serializing" << std::endl;
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CameraEffect);
        ar & BOOST_SERIALIZATION_NVP(m_tolerance);
        ar & BOOST_SERIALIZATION_NVP(m_k);
        ar & BOOST_SERIALIZATION_NVP(m_dmax);
    }
};


#endif // GLOADEFFECT_H
