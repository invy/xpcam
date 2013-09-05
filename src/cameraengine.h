#ifndef CAMERAENGINE_H
#define CAMERAENGINE_H

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/access.hpp>

#include <boost/shared_ptr.hpp>

#include "siminterface/IProcessing.h"
#include "siminterface/IDataAccess.h"
#include "siminterface/simdata.h"
#include "effects/cameraeffect.h"
#include "camerarolleffect.h"
#include "effects/vibrationeffect.h"
#include "effects/buffetingeffect.h"
#include "effects/groundshakingeffect.h"
#include "effects/gloadeffect.h"

template<typename TDataAccess, typename TProcessing, typename TDisplay, typename TPlanes>
class PluginInstance;

template<typename TD, typename TP>
class CameraEngine
{
    struct ViewDataRefs {
            void*             refRelHeadPosX;
            void*             refRelHeadPosY;
            void*             refRelHeadPosZ;
            void*             camOffsetRoll;
            void*             refHeadRoll;
            void*             viewHeading;
            void*             refViewRoll;
    };

private:

    std::vector<boost::shared_ptr<CameraEffect> > effects;

    CameraPosition g_defaultCameraPosition;
    CameraPosition g_positionDelta;

    struct ViewDataRefs gVDataRefs;

public:
    static float gameLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void *inRefcon)
    {
        CameraEngine* ce = static_cast<CameraEngine*>(inRefcon);
        return ce->flightLoopCallback(inElapsedSinceLastCall, inElapsedTimeSinceLastFlightLoop, inCounter);
    }

public:
    CameraEngine()
    {
        std::cout << __func__ << "\n";
        /* Register our hot key for the new view. */
        auto simdata = SimData<TD>::getInstance();
        auto dataAccess = simdata.m_dataAccess;

        gVDataRefs.refViewRoll      = simdata.requireDataRef(std::string("sim/graphics/view/field_of_view_roll_deg"));
        gVDataRefs.refRelHeadPosX   = simdata.requireDataRef(std::string("sim/graphics/view/pilots_head_x"));
        gVDataRefs.refRelHeadPosY   = simdata.requireDataRef(std::string("sim/graphics/view/pilots_head_y"));
        gVDataRefs.refRelHeadPosZ   = simdata.requireDataRef(std::string("sim/graphics/view/pilots_head_z"));

        dataAccess.setDataf(gVDataRefs.refViewRoll, 0);



        dataAccess.setDataf(gVDataRefs.refViewRoll, 0);

        effects.push_back(boost::shared_ptr<CameraEffect>(new CameraRollEffect<TD>(0.01f)));
        effects.push_back(boost::shared_ptr<CameraEffect>(new GroundShakingEffect<TD>(1.02f, 0.003f, 0.03f)));
        effects.push_back(boost::shared_ptr<CameraEffect>(new GLoadEffect<TD>(1.02f, 0.008f, 0.03f)));
        effects.push_back(boost::shared_ptr<CameraEffect>(new BuffetingEffect<TD>(1.02f, 0.01f, 0.03f)));

        printf("reading simulator parameters\n");
        g_defaultCameraPosition.y = dataAccess.getDataf(gVDataRefs.refRelHeadPosY);
        g_defaultCameraPosition.x = dataAccess.getDataf(gVDataRefs.refRelHeadPosX);
        g_defaultCameraPosition.z = dataAccess.getDataf(gVDataRefs.refRelHeadPosZ);

        printf("registring flight loop callback\n");
    }
    ~CameraEngine()
    {
        std::cout << __func__ << "\n";
        printf("unregistring flight loop callback\n");

        unregisterCallbacks();

        auto dataAccess = SimData<TD>::getInstance().m_dataAccess;
        dataAccess.setDataf(gVDataRefs.refRelHeadPosX, g_defaultCameraPosition.x);
        dataAccess.setDataf(gVDataRefs.refRelHeadPosY, g_defaultCameraPosition.y);
        dataAccess.setDataf(gVDataRefs.refRelHeadPosZ, g_defaultCameraPosition.z);
        dataAccess.setDataf(gVDataRefs.refViewRoll, 0);

    }

    float flightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter)
    {
        CameraPosition delta;
        auto dataAccess = SimData<TD>::getInstance().m_dataAccess;
        g_defaultCameraPosition.y = dataAccess.getDataf(gVDataRefs.refRelHeadPosY) - g_positionDelta.y;
        g_defaultCameraPosition.x = dataAccess.getDataf(gVDataRefs.refRelHeadPosX) - g_positionDelta.x;
        g_defaultCameraPosition.z = dataAccess.getDataf(gVDataRefs.refRelHeadPosZ) - g_positionDelta.z;

        for(auto it = effects.begin(); it != effects.end(); it++) {
            (*it)->apply(g_defaultCameraPosition, delta, dataAccess);
        }

        dataAccess.setDataf(gVDataRefs.refRelHeadPosX, g_defaultCameraPosition.x + delta.x);
        dataAccess.setDataf(gVDataRefs.refRelHeadPosY, g_defaultCameraPosition.y + delta.y);
        dataAccess.setDataf(gVDataRefs.refRelHeadPosZ, g_defaultCameraPosition.z + delta.z);

        g_positionDelta = delta;

        dataAccess.setDataf(gVDataRefs.refViewRoll, delta.roll);
        return 0.03f;

    }

    void onHotKey();

    void removeEffects() {
        effects.clear();
    }

    void unregisterCallbacks() {
        TP processing;
        processing.unregisterFlightLoopCallback(gameLoopCallback, this);
    }

    void registerCallbacks() {
        TP processing;
        processing.registerFlightLoopCallback(gameLoopCallback, 0.03f, this);
    }

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        std::cout << __func__ << " -- serializing" << std::endl;
        ar.register_type(static_cast<CameraRollEffect<TD> *>(NULL));
        ar.register_type(static_cast<BuffetingEffect<TD> *>(NULL));
        ar.register_type(static_cast<GroundShakingEffect<TD> *>(NULL));
        ar.register_type(static_cast<GLoadEffect<TD> *>(NULL));

        std::cout << "serialize effects parameters" << std::endl;
        ar & BOOST_SERIALIZATION_NVP(effects);
    }
};

#endif // CAMERAENGINE_H
