/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <map>

namespace env
{
// 	struct StateResult
// 	{
// 		StateResult()
// 			: bChangeState(false),
// 			newStateId(0),
// 			stateTime(0.0f) {}
// 
// 		bool bChangeState;
// 		unsigned long newStateId;
// 		float stateTime;
// 	};
// 
// 
// 
// 	class CStateMachineState
// 	{
// 	public:
// 		CStateMachineState(unsigned long stateId);
// 
// 		virtual void OnStateEntered(unsigned long prevStateId);
// 
// 		virtual void OnStateLeft(unsigned long nextStateId);
// 
// 		virtual StateResult Update(float dt);
// 
// 		float GetCurrentStateTime() const { return m_currentStateTime; }
// 
// 		unsigned long GetStateId() const { return m_stateId; }
// 		//float GetStateTime() const { return m_stateTime; }
// 
// 	private:
// 		unsigned long m_stateId;
// 
// 		float m_currentStateTime;
// 		//float m_stateTime;
// 	};
// 
// 	class CStateMachine
// 	{
// 	public:
// 		CStateMachine();
// 
// 		void Update(float dt);
// 
// 		void SetState(CStateMachineState* pState);
// 
// 		void RegisterState(CStateMachineState* pState);
// 
// 	private:
// 		std::map< unsigned long, CStateMachineState* > m_states;
// 
// 		CStateMachineState* m_pCurrentState;
// 	};
} // env

#endif // STATE_MACHINE_H