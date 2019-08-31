/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_FONT_H
#define D3D11_FONT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>


// namespace env
// {
//     struct Font
//     {
//         Font()
//             : m_initialSize(10),
//             m_initialSizeMin(10),
//             m_initialSizeMax(10) {}
// 
//         int m_initialSize;
//         int m_initialSizeMin;
//         int m_initialSizeMax;
//     };
// 
//     class CD3D11Font
//     {
//     public:
//         CD3D11Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax)
//         : m_pDevice(pDevice)
//         , m_pDeviceContext(pDeviceContext) {}
// 
//         ~CD3D11Font() {}
// 
//         void SetSize(int height) {}
// 
//         const Font& GetFont() const { return m_font; }
// 
//         void Draw(int posX, int posY, const VECTOR4& color, const std::string& strText);
// 
//         void OnLostDevice() {}
// 
//         void OnResetDevice() {}
// 
//     private:
// 
//         Font m_font;
// 
//         //ID3DXFont* m_pFont;
// 
//         ID3D11Device* m_pDevice;
//         ID3D11DeviceContext* m_pDeviceContext;
//     };
// 
//     typedef CD3D11Font CFont;
// } // env

#endif // D3D11_FONT_H
#endif // ENV_GRAPHIC_API_D3D11
