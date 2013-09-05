#ifndef CAMERAROLLEFFECT_H
#define CAMERAROLLEFFECT_H

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <functional>
#include <memory>

#include "siminterface/simdata.h"

#include "effects/cameraeffect.h"

template<typename TD>
class CameraRollEffect : public CameraEffect {
    float m_sideDisplacement;
    std::function<float(float)> m_rollApproxFunc;

    /*
     * roll dataref handle
     */
    void* m_refPlaneRoll;
    void* m_refPlanePitch;

public:
    virtual void apply(CameraPosition &/*pos*/, CameraPosition &deltaPos, IDataAccess &pda) {
        auto roll = pda.getDataf(m_refPlaneRoll);
        auto pitch = pda.getDataf(m_refPlanePitch);
        auto headroll = cameraRoll(roll);

        float additionalRoll = -(roll - headroll);
        float kpitch = pitchInfluence(pitch);
        std::cout << "pitch: " << pitch << "kpitch: " << kpitch << std::endl;
        deltaPos.roll += kpitch * additionalRoll;
        deltaPos.x += kpitch * m_sideDisplacement*additionalRoll;
    }

public:
    CameraRollEffect() : CameraRollEffect(0)
    {
    }

    CameraRollEffect(float sideDisplacement)
        : m_sideDisplacement(sideDisplacement)
    {
        std::cout << __func__ << "\n";
        m_rollApproxFunc = [](float roll) -> float {
            /*
             * camera roll:
             * for 0 <= roll <= 90: good approximation for head roll angle: 0.01115*roll^(2)
             * for 90 <= roll <= 180: the head will not be aligned with horizon anymore
             * for -90 <= roll <= 0: use the approximation and add - sign
             */
            if(0 <= roll && roll <= 90)
                return 0.2594*pow(roll, 1.3);
            else if(-90 <= roll && roll <= 0)
                return -0.2594*pow(-roll, 1.3);
            else
                return roll;
        };

        /*
         * require datarefs which will be used
         */
        m_refPlaneRoll = SimData<TD>::getInstance().requireDataRef(std::string("sim/flightmodel/position/phi"));
        m_refPlanePitch = SimData<TD>::getInstance().requireDataRef(std::string("sim/flightmodel/position/theta"));
    }
private:
    inline float pitchInfluence(float pitch) {
        float kPitch = 0;
        if(pitch >= 0 && pitch < 80) {
            kPitch = 1 - 0.0125f /* 1/80 */ * fabs(pitch);
        }
        else if(pitch < 0 && pitch > -40) {
            kPitch = 1 - 0.025f /* 1/40 */ * fabs(pitch);
        }
        return kPitch;
    }

    inline float cameraRoll(float roll)
    {
        return m_rollApproxFunc(roll);
    }
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        std::cout << "camera roll effect" << " -- serializing" << std::endl;
        std::cout << "loading effects parameters" << std::endl;
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CameraEffect);
        ar & BOOST_SERIALIZATION_NVP(m_sideDisplacement);
    }

};

#endif // CAMERAROLLEFFECT_H
