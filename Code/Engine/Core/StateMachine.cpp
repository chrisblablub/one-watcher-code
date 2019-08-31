// #include "StateMachine.h"
// 
// namespace env
// {
//     CStateMachineState::CStateMachineState(unsigned long stateId)
//         : m_stateId(stateId),
//         m_currentStateTime(0.0f)
//     {
//     }
// 
//     void CStateMachineState::OnStateEntered(unsigned long prevStateId)
//     {
//         m_currentStateTime = 0.0f;
//     }
// 
//     void CStateMachineState::OnStateLeft(unsigned long nextStateId)
//     {
//     }
// 
//     StateResult CStateMachineState::Update(float dt)
//     {
//         m_currentStateTime += dt;
// 
//         return StateResult();
//     }
// 
// 
// 
// 
// 
// 
//     CStateMachine::CStateMachine()
//         : m_pCurrentState(NULL)
//     {
//     }
// 
//     void CStateMachine::Update(float dt)
//     {
//         if (m_pCurrentState)
//         {
//             StateResult res = m_pCurrentState->Update(dt);
//             if (res.bChangeState)
//             {
//                 unsigned long curStateId = m_pCurrentState->GetStateId();
//                 m_pCurrentState->OnStateLeft(res.newStateId);
//                 m_pCurrentState = m_states[res.newStateId];
//                 m_pCurrentState->OnStateEntered(curStateId);
//             }
//         }
//     }
// 
//     void CStateMachine::SetState(CStateMachineState* pState)
//     {
//         m_pCurrentState = pState;
//     }
// 
//     void CStateMachine::RegisterState(CStateMachineState* pState)
//     {
//         m_states[pState->GetStateId()] = pState;
//     }
// 
// } // env