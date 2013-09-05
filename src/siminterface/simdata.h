#ifndef SIMDATA_H
#define SIMDATA_H

#include <unordered_map>
#include <string>

#include "Singleton.h"

#include "IDataAccess.h"

template<typename TD>
class SimData : public Singleton<SimData<TD> >
{
    std::unordered_map<std::string, void *> m_dataRefs;
public:
    /*
     * requests dataref handle
     */
    void* getDataRef(const std::string &s)
    {
        auto it = m_dataRefs.find(s);
        if(it == m_dataRefs.end()) {
            return it->second;
        }
        else
            return nullptr;
    }
    /*
     * requires dataref to be tracked
     */
    void* requireDataRef(const std::string &s)
    {
        auto it = m_dataRefs.find(s);
        if(it == m_dataRefs.end())
        {
            auto dataref = m_dataAccess.findDataRef(s);
            if(dataref == NULL)
                throw UnknownDataRefException();
            m_dataRefs[s] = dataref;
            return dataref;
        }
        else
            return it->second;
    }

public:
    TD m_dataAccess;

public:
    class UnknownDataRefException { };
};

#endif // SIMDATA_H
