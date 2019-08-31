/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_FONT_H
#define D3D9_FONT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>

namespace env
{
	struct Font
	{
		Font()
			: m_initialSize(10),
			m_initialSizeMin(10),
			m_initialSizeMax(10) {}

		int m_initialSize;
		int m_initialSizeMin;
		int m_initialSizeMax;
	};

	class CD3D9Font
	{
	public:
		CD3D9Font(IDirect3DDevice9* pDevice, const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax);

		~CD3D9Font();

		void SetSize(int height);

		const Font& GetFont() const;

		void Draw(int posX, int posY, const Vec4& color, const std::string& strText);

		void OnLostDevice();

		void OnResetDevice();
			
	private:

		Font m_font;

		ID3DXFont* m_pFont;

		IDirect3DDevice9* m_pDevice;
	};

	typedef CD3D9Font CFont;
} // env

#endif // INDEX_BUFFER_H
#endif // D3D9