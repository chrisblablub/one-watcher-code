#ifdef D3D9
#include <Graphic/D3D9/D3D9Font.h>

namespace env
{
	CD3D9Font::CD3D9Font(IDirect3DDevice9* pDevice, const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax)
		: m_pDevice(pDevice), m_pFont(0)
	{
		m_font.m_initialSize = size;
		m_font.m_initialSizeMin = sizeMin;
		m_font.m_initialSizeMax = sizeMax;

		D3DXFONT_DESC  FontDesc = { 0 };
		FontDesc.Height = size;
		FontDesc.MipLevels = D3DX_DEFAULT;
		FontDesc.Italic = bItalic;
		strcpy_s(FontDesc.FaceName, strFontFamily.length() + 1, strFontFamily.c_str());
		FontDesc.Weight = weight;
		FontDesc.OutputPrecision = 255;
		FontDesc.Quality = 255;

		D3DXCreateFontIndirect(pDevice, &FontDesc, &m_pFont);
	}

	CD3D9Font::~CD3D9Font()
	{
		ENV_RELEASE(m_pFont);
	}

	void CD3D9Font::SetSize(int height)
	{
		D3DXFONT_DESC  FontDesc = { 0 };
		m_pFont->GetDesc(&FontDesc);

		FontDesc.Height = height;

		ENV_RELEASE(m_pFont);

		D3DXCreateFontIndirect(m_pDevice, &FontDesc, &m_pFont);
	}

	const Font& CD3D9Font::GetFont() const { return m_font; }

	void CD3D9Font::Draw(int posX, int posY, const Vec4& color, const std::string& strText)
	{
		RECT r = { posX, posY, 0, 0 };

		unsigned long d3d9Color = COLOR_ARGB32((int)(color.w * 255.0f), (int)(color.x * 255.0f), (int)(color.y * 255.0f), (int)(color.z * 255.0f));

		// Grösse des Rechtecks berechnen
		m_pFont->DrawText(NULL, strText.c_str(), -1, &r, DT_CALCRECT, d3d9Color);

		int width = r.right - r.left;
		int height = r.bottom - r.top;
		r.left -= width * 0.5f;
		r.right -= width * 0.5f;
		r.top -= height * 0.5f;
		r.bottom -= height * 0.5f;
		// Text ausgeben
		m_pFont->DrawText(0, strText.c_str(), -1, &r, DT_LEFT, d3d9Color);
	}

	void CD3D9Font::OnLostDevice()
	{
		m_pFont->OnLostDevice();
	}

	void CD3D9Font::OnResetDevice()
	{
		m_pFont->OnResetDevice();
	}
} // env

#endif // D3D9