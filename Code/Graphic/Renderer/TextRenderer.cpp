
#include <Graphic/Renderer/TextRenderer.h>
#include <Graphic/Renderer/SpriteRenderer.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/RenderObjects/TextRenderObject.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    CTextRenderer::CTextRenderer(CSpriteRenderer* pSpriteRenderer)
        : m_pSpriteRenderer(pSpriteRenderer)
        , m_screenWidth(-1)
        , m_screenHeight(-1) {}

    //-----------------------------------------------------------------------------------
    CTextRenderer::~CTextRenderer()
    {

    }

    //-----------------------------------------------------------------------------------
    bool CTextRenderer::Initialize()
    {
        gFontMgr->SetAllocPolicy(CFont::BitmapFontAllocationPolicy(gRenderSystem->GetRenderDevice()->GetDevice(), gRenderSystem->GetRenderDevice()->GetDeviceContext()));

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CTextRenderer::Render(RenderQueue& queue, RenderSet& renderSet)
    {
        for (mapFontTextRenderObjects::iterator it = queue.m_textRenderObjects.begin(); it != queue.m_textRenderObjects.end(); ++it)
        {
            CFont* pFont = gFontMgr->AddGetPtr(it->first.GetKey());

            if (!pFont)
                continue;

//             mapFonts::iterator jt = m_fonts.find(it->first);
//             if (jt == m_fonts.end())
//                 continue;
// 
            for (listTextRenderObjects::iterator itText = it->second.begin(); itText != it->second.end(); ++itText)
            {
//                 int posX = itText->m_position.x * m_screenWidth;
//                 int posY = itText->m_position.y * m_screenHeight;

                if (itText->m_bScreenSpace)
                {
//                     Vec3 spriteResolutionScaling(1.0f / (float)(queue.m_pCameraRenderObject->m_screenWidth * 0.5f),
//                                                  1.0f / (float)(queue.m_pCameraRenderObject->m_screenHeight * 0.5f),
//                                                  1.0f);
                    itText->m_mtxTransform = /*glm::scale(Matrix4x4(1.0f), spriteResolutionScaling) **/  itText->m_mtxTransform;
                }

                listSpriteRenderObjects textSprites;
                pFont->DrawString(queue, textSprites, itText->m_strText, *itText);

//                 for (listSpriteRenderObjects::iterator it = textSprites.begin(); it != textSprites.end(); ++it)
//                 {
//                     it->m_scale = VECTOR3(1.0f / m_screenWidth, 1.0f / m_screenHeight, 1.0f);
//                 }

                m_pSpriteRenderer->RenderTextSprites(queue, renderSet, textSprites);
            }

            gFontMgr->Release(it->first.GetKey());
        }
    }

    //-----------------------------------------------------------------------------------
    void CTextRenderer::OnLostDevice()
    {

    }

    //-----------------------------------------------------------------------------------
    void CTextRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
//         if (m_screenWidth != screenWidth || m_screenHeight != screenHeight)
//         {
//             for (mapFonts::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
//             {
//                 const Font& font = it->second->GetFont();
// 
//                 if (font.m_initialSize == font.m_initialSizeMin &&
//                     font.m_initialSizeMax == font.m_initialSizeMax)
//                 {
//                     continue;
//                 }
// 
// 
//                 VECTOR2 refernceSize(1920.0f, 1080.0f);
//                 VECTOR2 scaling((float)screenWidth / refernceSize.x, (float)screenHeight / refernceSize.y);
// 
//                 if (scaling.x < scaling.y)
//                 {
//                     // Use smaller scaling factor & clamp to [0.5, 1.0f]
//                     scaling.x = std::min(scaling.x, 1.0f);
//                     scaling.x = std::max(scaling.x, 0.5f);
//                     scaling.y = scaling.x;
//                 }
//                 else
//                 {
//                     // Use smaller scaling factor & clamp to [0.5, 1.0f]
//                     scaling.y = std::min(scaling.y, 1.0f);
//                     scaling.y = std::max(scaling.y, 0.5f);
//                     scaling.x = scaling.y;
//                 }
// 
//                 it->second->SetSize((int)((float)font.m_initialSize * scaling.x));
//             }
//         }

        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;
    }


} // env