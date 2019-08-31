#include <Engine/Entity/PolygonEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/PolygonMousePicker.h>
#include <Engine/Core/Engine.h>


namespace env
{
    ENTITY_TYPEID CPolygonEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_POLYGON;

    CPolygonEntity::CPolygonEntity()
        : m_color(0.0f, 0.0f, 0.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_vertices.SetPrimitiveType(PRIMITIVE_TYPE_TRIANGLELIST);
        //m_vertices.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
        assert(false);
        //m_vertices.SetDeclaration(VertexPolygon::pDecl);
    }

    CPolygonEntity::~CPolygonEntity()
    {
        for (vecPoints::iterator it = m_newPoints.begin(); it != m_newPoints.end(); ++it)
        {
            gEngine->GetEntityMgr()->RemoveEntity((*it)->GetName());
        }
        m_newPoints.clear();
    }

    void CPolygonEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        AddPoint(Vec3(-100.0f, -100.0f, 0.0f));
        AddPoint(Vec3(-100.0f, 100.0f, 0.0f));
        AddPoint(Vec3(100.0f, 100.0f, 0.0f));
        AddPoint(Vec3(100.0f, -100.0f, 0.0f));
    }

    void CPolygonEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        m_points.clear();
    }

    void CPolygonEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        m_vertices.Reset();
        m_vertices.Reserve(m_newPoints.size() - 2);
        for (int i = 0; i < m_newPoints.size(); ++i)
        {
            if (i <= 2)
            {
                m_vertices.Add(PolygonVertex(m_newPoints[i]->GetNode()->GetDerivedPosition(), m_color));
            }
            else
            {
                m_vertices.Add(PolygonVertex(m_newPoints[0]->GetNode()->GetDerivedPosition(), m_color));
                m_vertices.Add(PolygonVertex(m_newPoints[i - 1]->GetNode()->GetDerivedPosition(), m_color));
                m_vertices.Add(PolygonVertex(m_newPoints[i]->GetNode()->GetDerivedPosition(), m_color));
            }
        }

        //queue.polygons.push_back(&m_vertices);
    }

    void CPolygonEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

#ifndef ENV_FINAL
        for (vecPoints::iterator it = m_newPoints.begin(); it != m_newPoints.end();)
        {
            if (!gEngine->GetEntityMgr()->IsValid(*it))
                it = m_newPoints.erase(it);
            else
                ++it;
        }
#endif // ENV_NOT_FINAL
    }


    void CPolygonEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }

    void CPolygonEntity::AddPoint(const Vec3& point)
    {
        CSpriteEntity* pPoint = static_cast<CSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(CSpriteEntity::ENTITY_META_TYPEID));
        pPoint->GetNode()->SetPosition(point);
        pPoint->SetRenderGame(false);
        pPoint->SetAutoDelete(false);
        pPoint->SetMaterialFilename("Sprites/vertexRing.png");
        m_pNode->AttachChildNode(pPoint->GetNode());
        gEngine->GetEntityMgr()->SetActiveEntity(pPoint);
        m_newPoints.push_back(pPoint);
    }
} // env