/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STATE_BLOCK_H
#define STATE_BLOCK_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/APITypes.h>
#include <tuple>

namespace env
{
    class CRenderDevice;

    class CStateBlock
    {
    public:
        typedef std::pair< RENDERSTATE_TYPE, unsigned long > pairRSValue;
        typedef std::tuple< unsigned long, SAMPLERSTATE_TYPE, unsigned long > tupleSSValue;

        CStateBlock();

        ~CStateBlock() {}

        void SetNumStates(CRenderDevice* pDevice, int numRS, int numSS = 0, int numTS = 0);

        void AddRenderState(RENDERSTATE_TYPE type, unsigned long value)
        {
            m_renderStates.push_back(pairRSValue(type, value));
        }

        void AddSamplerState(unsigned long sampler, SAMPLERSTATE_TYPE type, unsigned long value)
        {
            m_samplerStates.push_back(tupleSSValue(sampler, type, value));
        }

        void Apply();

        void OnLostDevice() {}

        void OnResetDevice();

    protected:
        CRenderDevice* m_pDevice;

        std::vector< pairRSValue > m_renderStates;
        std::vector< tupleSSValue > m_samplerStates;
    };


} //

#endif // STATE_BLOCK_H