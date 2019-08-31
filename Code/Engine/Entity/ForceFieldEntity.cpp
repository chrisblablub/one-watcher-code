#include <Engine/Entity/ForceFieldEntity.h>
#include <Graphic/Renderer/ForceFieldRenderer.h>
#include <Graphic/RenderSystems/RenderSystem.h>

namespace env
{
    ENTITY_TYPEID CForceFieldEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_FORCEFIELD;

    //-----------------------------------------------------------------------------------
    CForceFieldEntity::CForceFieldEntity()
        : m_acceleration(500.0f)
        , m_targetVelocity(750.0f)
        , m_color(1.0f, 0.3f, 0.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CForceFieldEntity::~CForceFieldEntity()
    {
//         CForceFieldRenderer* pFFR = static_cast<CForceFieldRenderer*>(gRenderSystem->GetRenderer("ForceFieldRenderer"));
//         pFFR->RemoveForceField(GetName());
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        CPolygon polygon;
        polygon.SetNumPoints(4);
        polygon.AddPoint(Vec3(-300.0f, -300.0f, 0.0f));
        polygon.AddPoint(Vec3(-300.0f, 300.0f, 0.0f));
        polygon.AddPoint(Vec3(300.0f, 300.0f, 0.0f));
        polygon.AddPoint(Vec3(300.0f, -300.0f, 0.0f));
        AddPolygon(polygon);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        CPolygon polygon;
        polygon.SetNumPoints(4);
        polygon.AddPoint(Vec3(-300.0f, -300.0f, 0.0f));
        polygon.AddPoint(Vec3(-300.0f, 300.0f, 0.0f));
        polygon.AddPoint(Vec3(300.0f, 300.0f, 0.0f));
        polygon.AddPoint(Vec3(300.0f, -300.0f, 0.0f));
        AddPolygon(polygon);


        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldEntity::OnEntityCloned(CEntity& entity)
    {
        CSpriteEntity::OnEntityCloned(entity);

        CForceFieldEntity& other = static_cast<CForceFieldEntity&>(entity);
        SetAcceleration(other.GetAcceleration());
        SetTargetVelocity(other.GetTargetVelocity());
        SetColor(other.GetColor());
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CForceFieldEntity::Update);

        CSpriteEntity::Update(dt);

        m_sprite.SetCenter(Vec3(300.0f, 300.0f, 0.0f) * m_pNode->GetDerivedScale());

        ApplyTransform(m_pNode->GetDerivedRST(), 1.0f);
    }
    
    //-----------------------------------------------------------------------------------
    void CForceFieldEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        m_forceFieldRenderObject.m_mtxTransform = m_pNode->GetDerivedRST();
        m_forceFieldRenderObject.m_up = m_pNode->GetDerivedUp();
        m_forceFieldRenderObject.m_orientation = m_pNode->GetDerivedOrientation();
        m_forceFieldRenderObject.m_color = m_color;

        queue.m_forceFields.push_back(&m_forceFieldRenderObject);
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }



} // env