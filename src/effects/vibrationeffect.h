#ifndef VIBRATIONEFFECT_H
#define VIBRATIONEFFECT_H

#include <random>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>


#include "cameraeffect.h"
#include "cameraposition.h"

class VibrationEffect : public CameraEffect {
protected:
    float tolerance;
    float k;
    float dmax;
public:
    VibrationEffect() {}
    VibrationEffect(float tolerance, float k, float dmax)
        : tolerance(tolerance), k(k), dmax(dmax), distribution(0.0,1.0)
    {
        std::cout << __func__ << "\n";
    }

private:
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
public:
    virtual void apply(CameraPosition &pos, CameraPosition &deltaPos, IDataAccess& pda) = 0;
public:

    float randomVibratoin() {
        float rnd = distribution(generator);
        return (k * rnd - k/2.0f);
    }
    float fvib(float speed) {
        // kSpeed defines the speed influence
        float kSpeed = speed/100.0f;
        if(kSpeed > 1)
            kSpeed = 1;
        float ret = kSpeed * randomVibratoin();

        if(ret > dmax)
            return dmax;
        else if(ret < -dmax)
            return -dmax;
        return ret;
    }
    /*
     * periodic vibration simulation
     * in: T is a period value
     *     omega is the current angle
     *     phi is the phase shift
     */
    float periodicVibration(float /*T*/, float /*omega*/, float /*phi*/) {
        return 0;
    }


private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /* version */)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CameraEffect);
        ar & BOOST_SERIALIZATION_NVP(tolerance);
        ar & BOOST_SERIALIZATION_NVP(k);
        ar & BOOST_SERIALIZATION_NVP(dmax);
    }

};

#endif // VIBRATIONEFFECT_H
