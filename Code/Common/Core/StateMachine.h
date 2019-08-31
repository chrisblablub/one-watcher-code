/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Common/Math/MathUtility.h>
#include <vector>

namespace env
{
    class StateEnterLeaveInfo
    {
    public:
        StateEnterLeaveInfo()
            : m_stateTime(0.0f) {}

        StateEnterLeaveInfo(float stateTime)
            : m_stateTime(stateTime) {}

        inline float GetTotalTime() const { return m_stateTime; }

    protected:
        float m_stateTime;
    };

    class StateUpdateInfo : public StateEnterLeaveInfo
    {
    public:

        StateUpdateInfo()
            : m_currentTime(0.0f) {}

        StateUpdateInfo(float currentTime, float stateTime)
            : StateEnterLeaveInfo(stateTime)
            , m_currentTime(currentTime) {}

        inline float GetElapsedTime() const { return m_currentTime; }

        // 0 -> 1
        inline float GetLinearInterpolation() const
        {
            return Clamp(m_currentTime / m_stateTime, 0.0f, 1.0f);
        }

        // 1 -> 0
        inline float GetLinearInterpolationInverse() const
        {
            return 1.0f - Clamp(m_currentTime / m_stateTime, 0.0f, 1.0f);
        }

        // 0 -> 1
        inline float GetExpInterpolation(float exponent) const
        {
            float tmp = GetLinearInterpolationInverse();
            tmp = std::pow(tmp, exponent);
            return 1.0f - tmp;
        }

        // 1 -> 0
        inline float GetExpInterpolationInverse(float exponent) const
        {
            float tmp = GetLinearInterpolationInverse();
            tmp = std::pow(tmp, exponent);
            return tmp;
        }

    protected:
        float m_currentTime;
    };

    template<class Type, class FrameParams>
    class CStateMachine
    {
    public:

        typedef FrameParams FrameParamsType;

        enum STATE
        {
            STATE_ENTER = 0,
            STATE_UPDATE,
            STATE_LEAVE,
            STATE_IDLE
        };

        typedef void (Type::*StateUpdateCallback)(const FrameParamsType& frameParams, const StateUpdateInfo&);
        typedef void (Type::*StateEnterLeaveCallback)(const FrameParamsType& frameParams, const StateEnterLeaveInfo&);

        struct StateSetupInfo
        {
            StateSetupInfo()
                : m_p(nullptr) {}

            StateSetupInfo(Type* p)
                : m_p(p) {}

            Type* m_p;
        };

        struct StateUpdateSetupInfo : StateSetupInfo
        {
            StateUpdateSetupInfo()
                : m_time(0.0f) {}

            StateUpdateSetupInfo(Type* p, StateUpdateCallback cb)
                : StateSetupInfo(p)
                , m_updateCallback(cb)
                , m_time(0.0f) {}

            StateUpdateSetupInfo(float time, Type* p, StateUpdateCallback cb)
                : StateSetupInfo(p)
                , m_updateCallback(cb)
                , m_time(time) {}

            StateUpdateCallback m_updateCallback;

            float m_time;
        };

        struct StateEnterLeaveSetupInfo : StateSetupInfo
        {
            StateEnterLeaveSetupInfo()
                : m_updateCallback(nullptr) {}

            StateEnterLeaveSetupInfo(Type* p, StateEnterLeaveCallback cb)
                : StateSetupInfo(p)
                , m_updateCallback(cb) {}

            StateEnterLeaveCallback m_updateCallback;
        };


        struct StateFullInfo
        {
            StateEnterLeaveSetupInfo m_enter;
            StateUpdateSetupInfo m_update;
            StateEnterLeaveSetupInfo m_leave;
        };

        CStateMachine()
        {
            Reset();
        }

        inline void SetIdle(bool bIdle) { m_bIdle = true; }
        inline void SetPaused(bool bPaused) { m_bPaused = bPaused; }
        inline void SetLooped(bool bLooped) { m_bLooped = bLooped; }
        inline void SetStateID(unsigned int stateID) { m_currentStateID = stateID; m_currentStateTime = 0.0f; }
        inline void SetNextStateID(unsigned int stateID) { m_nextStateID = stateID; }
        inline unsigned int GetNextStateID() const { return m_nextStateID; }
        inline unsigned int GetStateID() const { return m_currentStateID; }

        //-----------------------------------------------------------------------------------
        void RegisterStateEnterCallback(unsigned int stateID, const StateEnterLeaveSetupInfo& setupInfo)
        {
            if (m_states.size() <= stateID)
                m_states.resize(stateID + 1);

            m_states[stateID].m_enter = setupInfo;
        }

        //-----------------------------------------------------------------------------------
        void RegisterStateUpdateCallback(unsigned int stateID, const StateUpdateSetupInfo& setupInfo)
        {
            if (m_states.size() <= stateID)
                m_states.resize(stateID + 1);

            m_states[stateID].m_update = setupInfo;
        }

        //-----------------------------------------------------------------------------------
        void RegisterStateLeaveCallback(unsigned int stateID, const StateEnterLeaveSetupInfo& setupInfo)
        {
            if (m_states.size() <= stateID)
                m_states.resize(stateID + 1);

            m_states[stateID].m_leave = setupInfo;
        }

        //-----------------------------------------------------------------------------------
        void Reset()
        {
            m_currentStateTime = 0.0f;
            m_currentStateID = 0;
            m_currentStateInfo = StateUpdateInfo(0.0f, 0.0f);
            m_nextStateID = 0;
            m_currentState = STATE_ENTER;
            m_bIdle = false;
            m_bPaused = false;
            m_bLooped = false;
        }

        //-----------------------------------------------------------------------------------
        void ResetCurrentState()
        {
            m_currentStateTime = 0.0f;
        }

        //-----------------------------------------------------------------------------------
        void Update(const FrameParamsType& frameParams)
        {
            if (m_bPaused)
                return;

            m_currentStateTime += frameParams.m_dt;

            if (m_currentStateID >= m_states.size())
                return;

            if (m_bIdle)
            {
                m_currentState = STATE_IDLE;
                m_currentStateTime = 0.0f;
            }

            const StateFullInfo& stateFullInfo = m_states[m_currentStateID];

            if (m_currentState == STATE_ENTER)
            {
                const StateEnterLeaveSetupInfo& stateSetupInfo = stateFullInfo.m_enter;

                if (stateSetupInfo.m_updateCallback != nullptr)
                    (stateSetupInfo.m_p->*stateSetupInfo.m_updateCallback)(frameParams, StateEnterLeaveInfo(stateFullInfo.m_update.m_time));

                m_currentState = STATE_UPDATE;
                //m_currentStateTime = 0.0f;
            }

            if (m_currentState == STATE_UPDATE)
            {
                const StateUpdateSetupInfo& stateSetupInfo = stateFullInfo.m_update;

                if (stateSetupInfo.m_updateCallback != nullptr)
                    (stateSetupInfo.m_p->*stateSetupInfo.m_updateCallback)(frameParams, StateUpdateInfo(m_currentStateTime, stateSetupInfo.m_time));

                if (stateSetupInfo.m_time <= m_currentStateTime ||
                    stateSetupInfo.m_updateCallback == nullptr)
                {                    
                    m_currentState = STATE_LEAVE;
                }
            }

            if (m_currentState == STATE_LEAVE)
            {
                const StateEnterLeaveSetupInfo& stateSetupInfo = stateFullInfo.m_leave;

                m_currentState = STATE_ENTER;
                
                if (stateSetupInfo.m_updateCallback != nullptr)
                    (stateSetupInfo.m_p->*stateSetupInfo.m_updateCallback)(frameParams, StateEnterLeaveInfo(stateFullInfo.m_update.m_time));

                // If nextStateID equals currentStateID and loop is false,
                // just go to the successor state
                if (m_currentStateID != m_nextStateID)
                {
                    m_currentStateTime = 0.0f;

                    m_currentStateID = m_nextStateID;
                }
                else 
                {
//                    m_currentStateTime = 0.0f;
 //                   SetIdle(true);
                }

                
            }
        }

    private:

        std::vector< StateFullInfo > m_states;

        unsigned int m_currentStateID;
        unsigned int m_nextStateID;

        float m_currentStateTime;

        bool m_bPaused;
        bool m_bIdle;
        bool m_bLooped;

        StateUpdateInfo m_currentStateInfo;

        STATE m_currentState;
    };


    struct RenderQueue;

    struct UpdateFrameParams
    {
        UpdateFrameParams(float dt)
            : m_dt(dt) {}

        float m_dt;
    };

    struct RenderFrameParams
    {
        RenderFrameParams(float dt, RenderQueue& q)
            : m_dt(dt)
            , m_renderQueue(q) {}

        RenderQueue& m_renderQueue;
        float m_dt;
    };


    //-----------------------------------------------------------------------------------
    template<class Type>
    class CStateMachineRender : public CStateMachine<Type, RenderFrameParams> { public: };


    //-----------------------------------------------------------------------------------
    template<class Type>
    class CStateMachineUpdate : public CStateMachine<Type, UpdateFrameParams> { public: };

} // env

#endif // STATE_MACHINE_H