/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Core/APITypeConversion.h>
#include GRAPHIC_API_SHADER


namespace env
{
    typedef CShader::CShaderMgr CShaderMgr;

    extern CShaderMgr* gShaderMgr;
} // env

#endif // SHADER_H