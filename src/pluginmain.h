#ifndef PLUGINMAIN_H
#define PLUGINMAIN_H

#include <memory>
#include <vector>
#include <fstream>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>


#include "siminterface/IProcessing.h"
#include "siminterface/IDataAccess.h"
#include "siminterface/IDisplay.h"
#include "siminterface/IPlanes.h"

#include "siminterface/simdata.h"
#include "cameraengine.h"
#include "cameraposition.h"
#include "effects/cameraeffect.h"

#include "pluginmain.h"

struct PersistentData {
    void*	hotKey;
};

template<
        typename TDataAccess,
        typename TProcessing,
        typename TDisplay,
        typename TPlanes>
class PluginInstance :
        public Singleton
        <PluginInstance
        <TDataAccess,
        TProcessing,
        TDisplay,
        TPlanes> >
{
    struct FlightModelDataRefs {
            void*             gForceNormal;
            void*             gearNormal;
            void*             groundSpeed;
            void*             airSpeed;
            void*             totalWeight;
            void*             refPlaneRoll;
            void*             alpha;
    };

    struct ViewDataRefs {
            void*             refRelHeadPosX;
            void*             refRelHeadPosY;
            void*             refRelHeadPosZ;
            void*             camOffsetRoll;
            void*             refHeadRoll;
            void*             viewHeading;
            void*             refViewRoll;
    };

    struct ACFDataRefs {
            void*             acfAlphaMax;
    };

    struct PersistentData {
            void*   hotKey;
    };

public:
    struct PersistentData m_PD;
public:
    bool m_registred;

    std::shared_ptr<IProcessing> m_processing;
    std::shared_ptr<IDataAccess> m_dataAccess;
    std::shared_ptr<IDisplay> m_display;
    std::shared_ptr<IPlanes> m_planes;

    CameraEngine<TDataAccess, TProcessing> *m_pCamEngine;
    SimData<TDataAccess> m_simData;

public:
    int enable()
    {
        std::cout << __func__ << "\n";

/*        m_pCamEngine = new CameraEngine<TDataAccess, TProcessing>();
        try {
        {
            std::ifstream ifs("xpcam_config.xml");
            std::cout << __func__ << " <<< loading configuration" << std::endl;
            if(ifs.good()) {
                boost::archive::xml_iarchive ia(ifs);
                // read class instance to archive
                std::cout << __func__ << " <<< serialization call" << std::endl;
                ia >> BOOST_SERIALIZATION_NVP(m_pCamEngine);
            }
            // archive and stream closed when destructors are called
        }
        }catch(std::exception& e) {
            std::cerr << "exception" << std::endl;
            std::cerr << e.what() << std::endl;
        }
        delete m_pCamEngine;
*/

        return 1;
    }

    void disable()
    {

    }

    void start()
    {
        std::cout << __func__ << "\n";
        try {
            m_PD.hotKey = m_display->registerHotKey(XPLM_VK_F8, xplm_DownFlag,
                                            "Circling External View",
                                            EnableEffectsHotKeyCallback,
                                            this);
        } catch (typename SimData<TDataAccess>::UnknownDataRefException)
        {
            std::cout << "UnknownDataRefException" << std::endl;
        }
    }

    void stop()
    {

    }

    static void EnableEffectsHotKeyCallback(void *inRefcon)
    {
        PluginInstance* ce = static_cast<PluginInstance*>(inRefcon);
        ce->onHotKey();
    }

    void onHotKey()
    {
        std::cout << __func__ << "\n";
        /* TODO: Switch view to cockpit */


        PluginInstance<TDataAccess, TProcessing, TDisplay, TPlanes>& inst = PluginInstance<TDataAccess, TProcessing, TDisplay, TPlanes>::getInstance();
        /* Now we control the camera until the view changes. */
        if(!inst.m_registred)
        {
            m_pCamEngine = new CameraEngine<TDataAccess, TProcessing>();
            try {
                {
                    std::string path;
                    std::string fname;
                    m_planes->getNthAircraftModel(0, fname, path);
                    std::ifstream ifs(path + "xpcam_config.xml");
                    std::cout << "loading configuration: " << path << "xpcam_config.xml"  << std::endl;
                    if(ifs.good()) {
                        std::cout << __func__ << "<< serializing" << std::endl;
                        boost::archive::xml_iarchive ia(ifs);
                        // read class instance to archive
                        ia >> BOOST_SERIALIZATION_NVP(m_pCamEngine);
                        // archive and stream closed when destructors are called
                        std::cout << __func__ << "<< serialized normally" << std::endl;
                }
            }
            }catch(std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            std::cout << __func__ << "!! we are about to register flightloop callback" << std::endl;
            m_pCamEngine->registerCallbacks();
            inst.m_registred = true;
        }
        else
        {
            m_pCamEngine->unregisterCallbacks();
            try {
            {
                std::string path;
                std::string fname;
                m_planes->getNthAircraftModel(0, fname, path);
                std::ofstream ofs(path + "xpcam_config.xml");
                std::cout << "saving configuration: " << path << "xpcam_config.xml" << std::endl;
                if(ofs.good()) {
                    std::cout << __func__ << ">> serializing" << std::endl;
                    boost::archive::xml_oarchive oa(ofs);
                    // write class instance to archive
                    oa << BOOST_SERIALIZATION_NVP(m_pCamEngine);
                    // archive and stream closed when destructors are called
                }
            }
            }catch(std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            delete m_pCamEngine;
            inst.m_registred = false;
        }

    }

public:

    PluginInstance(void) :
        m_processing(new TProcessing()),
        m_dataAccess(new TDataAccess()),
        m_display(new TDisplay()),
        m_planes(new TPlanes()),
        m_pCamEngine(NULL)
    {
        std::cout << __func__ << "\n";
    }

    ~PluginInstance()
    {
    }

private:

    PluginInstance(const PluginInstance &);
    PluginInstance &operator=(const PluginInstance &);
};

#endif // PLUGINMAIN_H
