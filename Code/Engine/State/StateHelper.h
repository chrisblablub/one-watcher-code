/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef STATE_HELPER_H
#define STATE_HELPER_H

#include <Engine/EngineCommon.h>

namespace env
{ 
    Vec4 FADE_COLOR_TOP = Vec4(0, 0, 0, 1);
    Vec4 FADE_COLOR_BOT = Vec4(0, 0, 0, 1);

    inline void GetFadeTopColor(Vec4& topColor, Vec4& botColor)
    {
        CSingletonPlayerMgr playerMgr;
        unsigned int chapter = playerMgr->GetCurrentChapterIndex() + 1;

        std::stringstream str;
        str << "../Stripes/tmpl_chap" << chapter << "_orig_01.stp";

        CScript* pStripeScript = gScriptMgr->AddGetPtr(str.str());

        if (pStripeScript)
        {
            if (pStripeScript->Exists("startTopColor"))
                topColor = (*pStripeScript)["startTopColor"];

            if (pStripeScript->Exists("startBotColor"))
                botColor = (*pStripeScript)["startBotColor"];

            gScriptMgr->Release(str.str());
        }
    }


} // env

#endif // STATE_HELPER_H
