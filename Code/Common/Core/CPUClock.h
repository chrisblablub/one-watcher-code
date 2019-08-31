/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CPU_CLOCK_H
#define CPU_CLOCK_H

#include <Common/Common.h>

namespace env
{
    typedef envS64 TIME_STAMP;

    struct CPUClockFrequency
    {
        CPUClockFrequency()
        {
#ifdef ENV_PLATFORM_WINDOWS
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);

            m_freqS  = double(freq.QuadPart);
            m_freqMS = m_freqS  / 1000.0;
            m_freqUS = m_freqMS / 1000.0;
#endif // POP_PLATFORM_WINDOWS
        }

        double m_freqS;
        double m_freqMS;
        double m_freqUS;
    };

    class CPUClock
    {
    public:
        
        ~CPUClock();

        inline static TIME_STAMP GetTimestamp()
        {
#ifdef ENV_PLATFORM_WINDOWS
            LARGE_INTEGER ts;
            QueryPerformanceCounter(&ts);
            
            return ts.QuadPart;
#else
            return TIME_STAMP(0);
#endif // POP_PLATFORM_WINDOWS
        }

        inline static TIME_STAMP GetFrequencyInMS()
        {
            return static_cast<TIME_STAMP>(m_frequency.m_freqS);
        }

        inline static double ConvertToSecs(TIME_STAMP begin)
        {
            return double(GetTimestamp() - begin) / m_frequency.m_freqS;
        }

        inline static double ConvertToMilliSecs(TIME_STAMP begin)
        {
            return double(GetTimestamp() - begin) / m_frequency.m_freqMS;
        }

        inline static double ConvertToMikroSecs(TIME_STAMP begin)
        {
            return double(GetTimestamp() - begin) / m_frequency.m_freqUS;
        }

        inline static double ConvertToSecs(TIME_STAMP begin, TIME_STAMP end)
        {
            return double(end - begin) / m_frequency.m_freqS;
        }

        inline static double ConvertToMilliSecs(TIME_STAMP begin, TIME_STAMP end)
        {
            return double(end - begin) / m_frequency.m_freqMS;
        }

        inline static double ConvertToMikroSecs(TIME_STAMP begin, TIME_STAMP end)
        {
            return double(end - begin) / m_frequency.m_freqUS;
        }

    private:

        CPUClock();
        CPUClock(const CPUClock&);
        CPUClock operator=(const CPUClock&);

    private:

        static CPUClockFrequency m_frequency;
    };

} // env

#endif // CPU_CLOCK_H
