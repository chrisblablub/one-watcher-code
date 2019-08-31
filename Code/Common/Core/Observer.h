/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

#include <Common/Common.h>

namespace env
{
    class CObserver
    {
    public:
        virtual void OnNotified(unsigned long id, unsigned long Event)
        {
            m_BufferedData.push_back(std::make_pair(id, Event));
        }

    protected:
        std::list<std::pair<unsigned long, unsigned long>> m_BufferedData;

    };

    class CObservable
    {
    public:
        CObservable()
            : m_bIsLocked(false) {}

        void AddObserver(unsigned long Event, CObserver* pObserver, unsigned long id)
        {
            listEventObservers& eventObservers = m_Observers[Event];
            for (listEventObservers::iterator it = eventObservers.begin(); it != eventObservers.end(); ++it)
            {
                //ENV_ASSERT_RVOID(it->first != pObserver);
                if (it->first == pObserver)
                    return;
            }

            eventObservers.push_back(std::make_pair(pObserver, id));
        }

        void RemoveObservers()
        {
            m_Observers.clear();
        }

        void RemoveObserver(unsigned long Event, CObserver* pObserver)
        {
            if (m_Observers.find(Event) == m_Observers.end())
                return;

            listEventObservers& eventObservers = m_Observers[Event];
            for (listEventObservers::iterator it = eventObservers.begin(); it != eventObservers.end(); )
            {
                if (it->first == pObserver)
                {
                    if (m_bIsLocked && m_lockedEvent == Event)
                    {
                        m_toBeDeleted.push_back(pObserver);
                    }
                    else
                    {
                        it = eventObservers.erase(it);
                        if (eventObservers.size() == 0)
                            m_Observers.erase(Event);
                    }
                    break;
                }
                else
                    ++it;
            }
        }

        void NotifyObservers(unsigned long Event)
        {
            if (m_Observers.find(Event) == m_Observers.end())
                return;

            listEventObservers& eventObservers = m_Observers[Event];

            m_bIsLocked = true;
            m_lockedEvent = Event;
            for (listEventObservers::iterator it = eventObservers.begin(); it != eventObservers.end(); ++it)
            {
                it->first->OnNotified(it->second, Event);
                if (m_Observers.find(Event) == m_Observers.end())
                    return;
            }
            m_bIsLocked = false;

            for (std::list< CObserver* >::iterator itTBD = m_toBeDeleted.begin(); itTBD != m_toBeDeleted.end(); ++itTBD)
            {
                for (listEventObservers::iterator it = eventObservers.begin(); it != eventObservers.end(); )
                {
                    if (it->first == *itTBD)
                    {
                        eventObservers.erase(it);
                        if (eventObservers.size() == 0)
                            m_Observers.erase(Event);
                        break;
                    }
                }

            }
            m_toBeDeleted.clear();
        }

    protected:
        typedef std::list<std::pair<CObserver*, unsigned long>> listEventObservers;
        typedef std::map<unsigned long, listEventObservers> mapObservers;
        mapObservers m_Observers;

        bool m_bIsLocked;
        unsigned long m_lockedEvent;

        std::list< CObserver* > m_toBeDeleted;
    };

    void Test();
} // env
#endif // OBSERVER_H