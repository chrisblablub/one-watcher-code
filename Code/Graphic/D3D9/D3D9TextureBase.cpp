
#ifdef D3D9

#include <Graphic/D3D9/D3D9TextureBase.h>
#include <Graphic/RenderSystems/RenderSystem.h>


namespace env
{
		CD3D9TextureBase::CD3D9TextureBase(IDirect3DDevice9* pDevice)
		: m_pDevice(pDevice),
		m_pTexture(NULL),
		m_width(-1),
		m_height(-1)
	{
	}

	void CD3D9TextureBase::Bind(unsigned int unit)
	{
		m_pDevice->SetTexture(unit, m_pTexture);
	}

	CD3D9TextureBase::~CD3D9TextureBase()
	{
		ENV_RELEASE(m_pTexture);
		//Unload();
	}

	/*bool CD3D9TextureBase::Unload()
	{
		//ENV_RELEASE(m_pTexture);
		return true;
	}

	const std::string& CD3D9TextureBase::GetFilename()
	{
		return m_strFilename;
	}

	const std::string& CD3D9TextureBase::GetName()
	{
		return m_strName;
	}

	


	void CD3D9TextureBase::SetAlphaChannel(bool bSet)
	{
		m_bAlpha = bSet;
	}

	bool CD3D9TextureBase::HasAlphaChannel()
	{
		return m_bAlpha;
	}

	bool CD3D9TextureBase::InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt)
	{
		m_strFilename = strName;
		m_strName = str;

		if (!LoadResource())
			return false;

		
		(m_strFilename, pFt);

		return true;
	}*/

	
} // env
#endif // D3D9