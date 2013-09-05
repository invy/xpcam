#ifndef GROUNDSHAKINGEFFECT_H
#define GROUNDSHAKINGEFFECT_H

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "siminterface/simdata.h"
#include "headcamera_helpers.h"

#include "vibrationeffect.h"

template<typename TD>
class GroundShakingEffect : public VibrationEffect {
    void *m_groundSpeed;
    GroundInfluence<TD> m_gndInfluence;
public:
    GroundShakingEffect() : GroundShakingEffect(1, 1, 1) {}
    GroundShakingEffect(float tolerance, float k, float dmax)
        : VibrationEffect(tolerance, k, dmax),
        m_gndInfluence()
    {
        std:: cout << __func__ << std::endl;
        m_groundSpeed = SimData<TD>::getInstance().requireDataRef(std::string("sim/flightmodel/position/groundspeed"));
    }

    virtual void apply(CameraPosition &/*pos*/, CameraPosition &deltaPos, IDataAccess& pda) {
        float speed = pda.getDataf(m_groundSpeed);
        deltaPos.y += m_gndInfluence() * cameraGYGround(speed);
    }


protected:
    float cameraGYGround(float speed) {
        return fvib(speed);
    }
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        std::cout << "ground shaking effect" << " -- serializing" << std::endl;
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VibrationEffect);
    }
};

#endif // GROUNDSHAKINGEFFECT_H
