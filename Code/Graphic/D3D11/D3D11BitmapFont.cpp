
#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11BitmapFont.h>
#include <Graphic/D3D11/D3D11RenderDevice.h>
#include <Graphic/D3D11/D3D11TextureBase.h>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <vector>


#include <Graphic/Core/RenderQueue.h>

//#include "SpriteFont.h"
#include <Graphic/DirectXTK/pch.h>
#include <Graphic/DirectXTK/DirectXHelpers.h>
#include <Graphic/DirectXTK/BinaryReader.h>
#include <Graphic/RenderObjects/SpriteRenderObject.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace env
{
    // Internal SpriteFont implementation class.
    class CFont::Impl
    {
    public:
        Impl(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BinaryReader* reader);
        ~Impl()
        {
            ENV_DELETE(m_pTex);
        }

        Glyph const* FindGlyph(wchar_t character) const;

        void SetDefaultCharacter(wchar_t character);

        template<typename TAction>
        void ForEachGlyph(_In_z_ wchar_t const* text, CFont* p, TAction action);


        CTextureBase* m_pTex;

        unsigned int width;
        unsigned int height;

        std::vector<Glyph> glyphs;
        Glyph const* defaultGlyph;
        float lineSpacing;
    };

    //-----------------------------------------------------------------------------------
    static const char spriteFontMagic[] = "DXTKfont";

    //-----------------------------------------------------------------------------------
    // Comparison operators make our sorted glyph vector work with std::binary_search and lower_bound.
    static inline bool operator< (CFont::Glyph const& left, CFont::Glyph const& right)
    {
        return left.Character < right.Character;
    }

    //-----------------------------------------------------------------------------------
    static inline bool operator< (wchar_t left, CFont::Glyph const& right)
    {
        return left < right.Character;
    }

    //-----------------------------------------------------------------------------------
    static inline bool operator< (CFont::Glyph const& left, wchar_t right)
    {
        return left.Character < right;
    }

    //-----------------------------------------------------------------------------------
    // Reads a SpriteFont from the binary format created by the MakeSpriteFont utility.
    CFont::Impl::Impl(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BinaryReader* reader)
        : m_pTex(nullptr)
    {
        // Validate the header.
        for (char const* magic = spriteFontMagic; *magic; magic++)
        {
            if (reader->Read<uint8_t>() != *magic)
            {
                DebugTrace("SpriteFont provided with an invalid .spritefont file\n");
                throw std::exception("Not a MakeSpriteFont output binary");
            }
        }

        // Read the glyph data.
        auto glyphCount = reader->Read<uint32_t>();
        auto glyphData = reader->ReadArray<Glyph>(glyphCount);

        glyphs.assign(glyphData, glyphData + glyphCount);

        // Read font properties.
        lineSpacing = reader->Read<float>();

        SetDefaultCharacter((wchar_t)reader->Read<uint32_t>());

        // Read the texture data.
        auto textureWidth = reader->Read<uint32_t>();
        auto textureHeight = reader->Read<uint32_t>();
        auto textureFormat = reader->Read<DXGI_FORMAT>();
        auto textureStride = reader->Read<uint32_t>();
        auto textureRows = reader->Read<uint32_t>();
        auto textureData = reader->ReadArray<uint8_t>(textureStride * textureRows);

        // Create the D3D texture.
        CD3D11_TEXTURE2D_DESC textureDesc(textureFormat, textureWidth, textureHeight, 1, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);
        CD3D11_SHADER_RESOURCE_VIEW_DESC viewDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureFormat);
        D3D11_SUBRESOURCE_DATA initData = { textureData, textureStride };

        ID3D11Texture2D* pTex = nullptr;
        ID3D11ShaderResourceView* pSRV = nullptr;

        if (device)
        {
            ENV_DXCALL(device->CreateTexture2D(&textureDesc, &initData, &pTex));
            ENV_DXCALL(device->CreateShaderResourceView(pTex, &viewDesc, &pSRV));

            m_pTex = new CTextureBase(device, deviceContext, pTex, pSRV, textureWidth, textureHeight);
            //deviceContext->GenerateMips(pSRV);

            m_pTex->SetTextureSampler(CDefaultTextureSampler::GetLinearMipClampSampler());
        }

        

        //SetDebugObjectName(srv.Get(), "DirectXTK:SpriteFont");
        //SetDebugObjectName(texture.Get(), "DirectXTK:SpriteFont");
    }

    //-----------------------------------------------------------------------------------
    // Looks up the requested glyph, falling back to the default character if it is not in the font.
    CFont::Glyph const* CFont::Impl::FindGlyph(wchar_t character) const
    {
        auto glyph = std::lower_bound(glyphs.begin(), glyphs.end(), character);

        if (glyph != glyphs.end() && glyph->Character == character)
        {
            return &*glyph;
        }

        if (defaultGlyph)
        {
            return defaultGlyph;
        }

        DebugTrace("SpriteFont encountered a character not in the font (%u, %C), and no default glyph was provided\n", character, character);
        throw std::exception("Character not in font");
    }


    //-----------------------------------------------------------------------------------
    // Sets the missing-character fallback glyph.
    void CFont::Impl::SetDefaultCharacter(wchar_t character)
    {
        defaultGlyph = nullptr;

        if (character)
        {
            defaultGlyph = FindGlyph(character);
        }
    }


    //-----------------------------------------------------------------------------------
    // The core glyph layout algorithm, shared between DrawString and MeasureString.
    template<typename TAction>
    void CFont::Impl::ForEachGlyph(_In_z_ wchar_t const* text, CFont* p, TAction action)
    {
        float x = 0;
        float y = 0;

        for (; *text; text++)
        {
            wchar_t character = *text;

            switch (character)
            {
            case '\r':
                // Skip carriage returns.
                continue;

            case '\n':
                // New line.
                x = 0;
                y += lineSpacing;
                break;

            default:
                // Output this character.
                auto glyph = FindGlyph(character);

                x += glyph->XOffset;

                if (x < 0)
                    x = 0;

                if (!iswspace(character)
                    || ((glyph->Subrect.right - glyph->Subrect.left) > 1)
                    || ((glyph->Subrect.bottom - glyph->Subrect.top) > 1))
                {
                    action(glyph, x, y, p);
                }

                x += glyph->Subrect.right - glyph->Subrect.left + glyph->XAdvance;
                break;
            }
        }
    }

    CFontMgr* gFontMgr = NULL;

    //-----------------------------------------------------------------------------------
    CFont::CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
        : m_pDevice(pDevice)
        , m_pDeviceContext(pDeviceContext)
    {

    }

    // Move constructor.
    CFont::CFont(CFont&& moveFrom)
        : pImpl(std::move(moveFrom.pImpl))
    {
    }


    // Move assignment.
    CFont& CFont::operator= (CFont&& moveFrom)
    {
        pImpl = std::move(moveFrom.pImpl);
        return *this;
    }

    //-----------------------------------------------------------------------------------
    CFont::~CFont()
    {
        //ENV_DELETE(m_pSpriteFont);
    }

    //-----------------------------------------------------------------------------------
    bool CFont::InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt)
    {
        m_strFilename = strFilename;
        m_strName = strName;

        if (!GetFileExists(m_strFilename))
            return false;

        return LoadResource();

    }

    //-----------------------------------------------------------------------------------
    bool CFont::LoadResource()
    {
        //ENV_DELETE(m_pSpriteFont);

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        const std::wstring strInputFilenameUnicode = converter.from_bytes(m_strFilename);

        BinaryReader reader(strInputFilenameUnicode.c_str());

        pImpl.reset(new Impl(m_pDevice, m_pDeviceContext, &reader));

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CFont::DrawString(RenderQueue& renderQueue, std::list< SpriteRenderObject >& textSprites, const std::wstring& strText, const TextRenderObject& textRenderObject)
    {
        unsigned int i = 0;

        Vec2 textSize = MeasureString(strText);

        // Draw each character in turn.
        pImpl->ForEachGlyph(strText.c_str(), this, [&](Glyph const* glyph, float x, float y, CFont* p)
        {
            Vec3 offset(x - glyph->XAdvance, y, 0.0f);
            Vec3 scale(glyph->Subrect.right - glyph->Subrect.left, glyph->Subrect.bottom - glyph->Subrect.top, 1.0f);
            scale *= 0.5f;
            scale.y *= -1.0f;

            if (textRenderObject.m_alignment == TEXT_ALIGNMENT_CENTER)
                offset += Vec3(-textSize.x * 0.5f, 0.0f, 0.0f);
            else if (textRenderObject.m_alignment == TEXT_ALIGNMENT_RIGHT)
                offset += Vec3(-textSize.x, 0.0f, 0.0f);

            Matrix4x4 mtxInternalTransform = glm::scale(Matrix4x4(1.0f), scale) * glm::transpose(glm::translate(Matrix4x4(1.0f), offset));// *textRenderObject.m_mtxTransform;
            
//             TODO: Port to Sprite object and then get SpriteRenderObject from Sprite
            SpriteRenderObject spriteRenderObject;
            spriteRenderObject.m_bAlphaBlending = true;
            spriteRenderObject.m_bBillboard = textRenderObject.m_bBillboard;
            spriteRenderObject.m_bScreenSpace = textRenderObject.m_bScreenSpace;
            spriteRenderObject.m_color = textRenderObject.m_color;
            spriteRenderObject.m_mtxRSTPre = mtxInternalTransform;
            spriteRenderObject.m_mtxRST = textRenderObject.m_mtxTransform;
            spriteRenderObject.m_pTexture = pImpl->m_pTex;
            spriteRenderObject.m_texRect.x = static_cast<float>(glyph->Subrect.left) / spriteRenderObject.m_pTexture->GetWidth();
            spriteRenderObject.m_texRect.y = static_cast<float>(glyph->Subrect.bottom) / spriteRenderObject.m_pTexture->GetHeight();
            spriteRenderObject.m_texRect.z = static_cast<float>(glyph->Subrect.right - glyph->Subrect.left) / spriteRenderObject.m_pTexture->GetWidth();
            spriteRenderObject.m_texRect.w = static_cast<float>(glyph->Subrect.bottom - glyph->Subrect.top) / spriteRenderObject.m_pTexture->GetHeight();
            spriteRenderObject.m_texRect.y -= spriteRenderObject.m_texRect.w;
            spriteRenderObject.m_pipeline = SPRITE_PIPELINE_AFTER_FADE;

            textSprites.push_back(spriteRenderObject);
        });
    }

    //-----------------------------------------------------------------------------------
    Vec2 CFont::MeasureString(const std::wstring& strText)
    {
        Vec2 result(0.0f, 0.0f);

        pImpl->ForEachGlyph(strText.c_str(), this, [&](Glyph const* glyph, float x, float y, CFont* p)
        {
            float w = (float)(glyph->Subrect.right - glyph->Subrect.left);
            float h = (float)(glyph->Subrect.bottom - glyph->Subrect.top) + glyph->YOffset;

            h = std::max(h, pImpl->lineSpacing);

            result.x = std::max(x + w, 0.0f);
            result.y = std::max(y + h, 0.0f);
            //result = XMVectorMax(result, XMVectorSet(x + w, y + h, 0, 0));
        });

        return result;
    }

    //-----------------------------------------------------------------------------------
    void CFont::DrawText(const std::string& strText)
    {

    }
} // env
#endif // ENV_GRAPHIC_API_D3D11
