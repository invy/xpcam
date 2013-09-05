#ifndef CAMERAEFFECT_H
#define CAMERAEFFECT_H

#include <memory>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "siminterface/IDataAccess.h"
#include "cameraposition.h"

class CameraEffect {
public:
    /*
     * function applies an affect
     * sets the camera position, and changes, which were made by the function
     */
    virtual void apply(CameraPosition &pos, CameraPosition &deltaPos, IDataAccess &pda) = 0;

    CameraEffect() {}
    virtual ~CameraEffect() {}

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & /* ar */, const unsigned int /* version */) {}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(CameraEffect)

#endif // CAMERAEFFECT_H
