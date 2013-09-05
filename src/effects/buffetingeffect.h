#ifndef BUFFETINGEFFECT_H
#define BUFFETINGEFFECT_H

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "siminterface/IDataAccess.h"

#include "siminterface/simdata.h"
#include "headcamera_helpers.h"

#include "vibrationeffect.h"

template<typename TD>
class BuffetingEffect : public VibrationEffect {
    float m_alphaMax;
    float m_minBuffetSpeed;
    float m_buffetRange;

    void *m_alpha;
    void *m_airSpeed;

    GroundInfluence<TD> m_gndInfluence;
public:
    BuffetingEffect() : BuffetingEffect(1,1,1) {}
    BuffetingEffect(float tolerance, float k, float dmax)
        : VibrationEffect(tolerance, k, dmax), m_alphaMax(0),
          m_gndInfluence()
    {
        std::cout << __func__ << "\n";
        auto simData = SimData<TD>::getInstance();
        m_airSpeed = simData.requireDataRef(std::string("sim/flightmodel/position/groundspeed"));
        m_alpha = simData.requireDataRef(std::string("sim/flightmodel/position/alpha"));


        m_alphaMax = simData.m_dataAccess.getDataf(simData.requireDataRef(std::string("sim/aircraft/overflow/acf_stall_warn_alpha")));
        m_minBuffetSpeed = 50.0f;
        m_buffetRange = 2.0f;
        printf("got acf stall warning alpha: %.02f\n", m_alphaMax);
    }



    void apply(CameraPosition & /*pos*/, CameraPosition &deltaPos, IDataAccess& pda) {
        float airSpeed = pda.getDataf(m_airSpeed);
        float alpha = pda.getDataf(m_alpha);

        double kGround = m_gndInfluence();

        deltaPos.y += (1 - kGround) * buffetShaking(airSpeed, alpha);
    }

    void updateSimData();




private:
    float buffetShaking(float speed, float alpha) {
        if(speed > m_minBuffetSpeed && alpha + m_buffetRange > m_alphaMax) {
            if(alpha > m_alphaMax)
                alpha = m_alphaMax;
            float kAlpha = 1.0f/(1.0f + 5 * fabs(alpha-m_alphaMax));
    //		printf("kAlpha = %.02f, alpha = %.02f, alphaMax= %.02f\n", kAlpha, alpha, alphaMax);

            //, 1.02f, 0.01f, 0.03f);
            return kAlpha * fvib(speed);
        }
        return 0.0f;
    }
public:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        std::cout << "buffeting effect" << " -- serializing" << std::endl;
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VibrationEffect);
        ar & BOOST_SERIALIZATION_NVP(m_alphaMax);
        ar & BOOST_SERIALIZATION_NVP(m_minBuffetSpeed);
        ar & BOOST_SERIALIZATION_NVP(m_buffetRange);
        printf("got acf stall warning alpha: %.02f\n", m_alphaMax);
    }
};

#endif // BUFFETINGEFFECT_H
