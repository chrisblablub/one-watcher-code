/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TIMER_H
#define TIMER_H

namespace env
{
    class CTimer
    {
    public:

        CTimer()
            : m_bStopped(false)
            , m_time(0.0) {}

        inline bool GetStopped() const
        {
            return m_bStopped;
        }

        inline float GetTime() const
        {
            return m_time;
        }

        inline void Start()
        {
            m_bStopped = false;
        }

        inline void Stop()
        {
            m_bStopped = true;
            m_time = 0.0f;
        }

        inline void Pause()
        {
            m_bStopped = true;
        }

        inline void SetTime(float time)
        {
            m_time = time;
        }

        inline void Reset()
        {
            m_time = 0.0f;
        }

        inline void Update(float dt)
        {
            if (!m_bStopped)
                m_time += dt;
        }

    private:
        bool m_bStopped;
        float m_time;
    };
} // env

#endif // TIMER_H