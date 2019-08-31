/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CSPRITE_H
#define CSPRITE_H

#include <Engine/EngineCommon.h>
#include <Graphic/Core/Material.h>
#include <Graphic/RenderObjects/SpriteRenderObject.h>

namespace env
{
    class CSceneNode;

    class CSprite
    {
    public:

        CSprite();
        CSprite(const CSprite& other);
        ~CSprite();

        void InitializeAtLoad();

        void SetMaterialFilename(const std::string& strTexture, bool bCreateNewMtrlInstance = false);

        void Reset();

        void SetAlphaBlending(bool bAlphaBlending);
        bool GetAlphaBlending() { return m_bAlphaBlending; }

        void SetBillboard(bool bBillboard);
        bool GetBillboard() { return m_bBillboard; }

        void SetScreenSpace(bool bSet);
        bool GetScreenSpace() const { return m_bScreenSpace; }

        inline void SetDepthTest(bool bSet) { m_bDepthTest = bSet; }
        inline bool GetDepthTest() const { return m_bDepthTest; }

        void SetAlpha(float alpha);
        void SetColor(const Vec3& clr);
        void SetColor(const Vec4& clr);

        const Vec4& GetColor() { return m_vColor; }

        void SetTextPosition(const Vec2& pos) { m_vTextPosition = pos; }
        const Vec2& GetTextPosition() { return m_vTextPosition; }

        void SetText(const std::string& strText) { m_strText = strText; }
        const std::string& GetText() { return m_strText; }

        CManagedMaterial* GetMaterial() { return m_pMaterial; }

        void GetAbsoluteRect(TextureRect& absRect);
        void GetLogicalRect(float& left, float& right, float& top, float& bottom);
        void GetLogicalRect(TextureRect& logicalRect);
        void SetLogicalRect(const TextureRect& spriteRect) { m_logicalRect = spriteRect; }

        const Vec3& GetCenter();
        void SetCenter(const Vec3& vCenter) { m_vCenter = vCenter; }

        void SetUpdateRectByAnimation(bool bUpdate) { m_bUpdateRect = bUpdate; }
        bool GetUpdateRectByAnimation() const { return m_bUpdateRect; }

        void AttachNode(CSceneNode* pNode) { m_pNode = pNode; }
        CSceneNode* GetNode() const { return m_pNode; }

//         void SetApplyFade(bool bApply) { m_bApplyFade = bApply; }
//         bool GetApplyFade() const { return m_bApplyFade; }
// 
//         void SetApplyPostProcessing(bool bApply) { m_bApplyPostProcessing = bApply; }
//         bool GetApplyPostProcessing() const { return m_bApplyPostProcessing; }

        SpriteRenderObject& GetRenderObject();

        inline void SetSpritePipeline(SPRITE_PIPELINE pipe) { m_pipeline = pipe; }
        inline SPRITE_PIPELINE GetSpritePipeline() const { return m_pipeline; }

    private:

        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_strMaterial;
            ar & m_bAlphaBlending;
            ar & m_bBillboard;
            ar & m_bScreenSpace;
            ar & m_vPosition;
            ar & m_flags;
            ar & m_strText;
            ar & m_vTextPosition;
            ar & m_color;
            ar & m_vColor;

            if (version > 0)
            {
                ar & m_bApplyFade;
                ar & m_bApplyPostProcessing;
            }

            if (version > 1)
                ar & m_pipeline;

            if (version > 2)
                ar & m_bDepthTest;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_strMaterial;
            ar & m_bAlphaBlending;
            ar & m_bBillboard;
            ar & m_bScreenSpace;
            ar & m_vPosition;
            ar & m_flags;
            ar & m_strText;
            ar & m_vTextPosition;
            ar & m_color;
            ar & m_vColor;

            if (version > 0)
            {
                ar & m_bApplyFade;
                ar & m_bApplyPostProcessing;
            }

            if (version > 1)
                ar & m_pipeline;

            if (version > 2)
                ar & m_bDepthTest;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    protected:
        CManagedMaterial* m_pMaterial;
        std::string m_strMaterial;

        bool m_bAlphaBlending;
        bool m_bBillboard;
        bool m_bScreenSpace;
        bool m_bMaterialInstance;
        bool m_bUpdateRect;
        bool m_bApplyFade;
        bool m_bApplyPostProcessing;
        bool m_bDepthTest;

        Vec3 m_vPosition;
        unsigned long m_flags;
        Matrix4x4 m_mtxTransform;
        std::string m_strText;
        Vec2 m_vTextPosition;
        unsigned long m_color;
        Vec4 m_vColor;
        Vec3 m_vCenter;
        TextureRect m_logicalRect;

        CSceneNode* m_pNode;

        SpriteRenderObject m_spriteRenderObject;

        SPRITE_PIPELINE m_pipeline;
    };
} // env

#endif // CSPRITE_H