/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STATE_H
#define STATE_H

#include <Engine/EngineCommon.h>

namespace env
{
	struct RenderQueue;

	/*!
	 * This class represents a State.
	 * You may change from one state into another (similar to a
	 * finite state machine). Each state will be Update()d and 
	 * Render()ed every frame until it is Unload()ed or changed.
	 * You must call Load() to load the State.
	 *
	 * \author Christian Hamm
	 */
	class CState
	{
	public:
		//CState() {}
		CState(const std::string& strName);
		virtual ~CState();

		virtual bool Load(const std::string& strPreviousState);
		virtual bool Unload();

		virtual void Update(float time, float dt, RenderQueue& renderQueue);

		virtual void OnLostDevice() {}
		virtual void OnResetDevice(int screenWidth, int screenHeight) {}

		//int GetStateId()	{ return m_StateId; }
		std::string& GetStateName() { return m_strName; }

		/*void ChangeState(const std::string& strNextState);

		bool GetFinished() { return m_bIsFinished; }
		std::string& GetNextStateName() { return m_strNextStateName; }*/

		void SetUsedExclusively( bool bExclusively = true );
		bool IsUsedExclusively() { return m_bUseExclusively; }

	
	private:
		//bool		m_bIsFinished;
		//std::string m_strNextStateName;
	protected:
		std::string m_strName;
		
		//int m_StateId;
		bool m_bUseExclusively;
	};
} /* End namespace */

#endif /* STATE_H */