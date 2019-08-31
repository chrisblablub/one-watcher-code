/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <Graphic/GraphicConfig.h>
#include GRAPHIC_API_TEXTURE


namespace env
{
    typedef CTextureManaged::CTextureMgr CTextureMgr;

    extern CTextureMgr* gTextureMgr;
}
