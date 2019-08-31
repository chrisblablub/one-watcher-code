/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma  once

#include <Engine/EngineCommon.h>
#include <Common/Core/Observer.h>

namespace env
{

    template< class T > class CInputListener
    {
    public:
        virtual ~CInputListener() {}

        virtual void OnNotified(unsigned long id, const T& data)
        {
            m_BufferedData.push_back(std::make_pair(id, data));
        }

        virtual void OnRemoved(unsigned long id)
        {
        }

        virtual void OnAdded(unsigned long id)
        {
        }

    protected:
        std::list<std::pair< unsigned long, T > > m_BufferedData;

    };

    template< class T > class CInputListenable
    {
    public:
        CInputListenable()
            : m_pListener(0)
            /*: m_bIsLocked(false)*/ {}


        void SetListener(CInputListener<T>* pListener, unsigned long id)
        {
            bool bNotifyRemoved = m_pListener && ((m_pListener != pListener) || m_id != id);
            bool bNotifyAdded = m_pListener != pListener || m_id != id;
            if (bNotifyRemoved)
                m_pListener->OnRemoved(m_id);

            m_id = id;
            m_pListener = pListener;

            if (bNotifyAdded)
                m_pListener->OnAdded(m_id);
        }

        void UnsetListener()
        {
            m_pListener = NULL;
        }

        CInputListener<T>* GetListener()
        {
            return m_pListener;
        }
        /*void PushListener(unsigned long id, CInputListener<T>* pListener)
        {
            if(!m_queueListeners.empty() && m_queueListeners.back().first == pListener)
                return;

            m_queueListeners.push_back(std::make_pair(pListener, id));
        }

        void PopListener()
        {
            m_queueListeners.pop_back();
        }*/

        void NotifyListeners(const T& data)
        {
            if (!m_pListener)
                return;

            m_pListener->OnNotified(m_id, data);


            /*if(m_listeners.find(Event) == m_listeners.end())
                return;

            listEventObservers& eventObservers = m_listeners[Event];

            m_bIsLocked = true;
            m_lockedEvent = Event;
            for(listEventObservers::iterator it = eventObservers.begin(); it != eventObservers.end(); ++it)
            {
                it->first->OnNotified(it->second, Event, data);
                if(m_listeners.find(Event) == m_listeners.end())
                    return;
            }
            m_bIsLocked = false;

            for(std::list< CObserver* >::iterator itTBD = m_toBeDeleted.begin(); itTBD != m_toBeDeleted.end(); ++itTBD)
            {
                for(listEventObservers::iterator it = eventObservers.begin(); it != eventObservers.end(); )
                {
                    if(it->first == *itTBD)
                    {
                        eventObservers.erase(it);
                        if(eventObservers.size() == 0)
                            m_listeners.erase(Event);
                        break;
                    }
                }

            }
            m_toBeDeleted.clear();*/
        }

    protected:

        CInputListener<T>* m_pListener;
        unsigned long m_id;

        std::list< std::pair< CInputListener<T>*, unsigned long > > m_queueListeners;

        /*typedef std::list<std::pair<CObserver*, unsigned long>> listEventListeners;
        typedef std::map<unsigned long, listEventListeners> mapListeners;
        mapListeners m_listeners;

        bool m_bIsLocked;
        unsigned long m_lockedEvent;

        std::list< CObserver* > m_toBeDeleted;´*/
    };
}