#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/ParticleSystemMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Math/Intersection.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/VertexBuffer.h>

namespace env
{
    ENTITY_TYPEID CParticleSystemEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PARTICLESYSTEM;

    //-----------------------------------------------------------------------------------
    CParticleSystemEntity::CParticleSystemEntity()
        : m_pDevice(nullptr)
        , m_pMaterial(nullptr)
        , m_pParticleVB(nullptr)
        , m_currentAngle(0.0f)
        , m_alignment(PARTICLESYSTEM_ALIGNMENT_NONE)
        , m_bEmitNewParticles(true)
        , m_bRenderParticles(true)
        , m_bLoadFromTemplate(true)
        , m_strRenderer("Default")
        , m_numTotalParticles(0)
        , m_numTotalParticlesAlive(0)
        , m_time(0.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Particles] Enable", PARTICLESYSTEM_EVENT_ENABLE);
        RegisterEvent("[Particles] Disable", PARTICLESYSTEM_EVENT_DISABLE);
        RegisterEvent("[Particles] Start Emit", PARTICLESYSTEM_EVENT_STARTEMIT);
        RegisterEvent("[Particles] Stop Emit", PARTICLESYSTEM_EVENT_STOPEMIT);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    CParticleSystemEntity::~CParticleSystemEntity()
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->SetParticleSystem(NULL);
            gEngine->GetEntityMgr()->RemoveEntity(it->second);
        }
        m_emitters.clear();

        for (mapAffectors::iterator it = m_affectors.begin(); it != m_affectors.end(); ++it)
            ENV_DELETE(it->second);
        m_affectors.clear();

        Unload();
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();
        GetSprite().SetAlphaBlending(true);
        SetTextPosition(Vec2(0.0, 24.0));
        SetText(GetName());
        SetMaterialFilename("Sprites/particlesystem.png");
        SetRenderGame(false);

        Initialize(gRenderSystem->GetRenderDevice());
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();
        SetRenderGame(false);


        Initialize(gRenderSystem->GetRenderDevice());

        SetParticleMaterialFilename(m_strMaterial);

        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
            it->second->SetParticleSystem(this);

        if (m_bLoadFromTemplate && !m_strParticleSystemTemplate.empty())
            LoadFromTemplate(m_strParticleSystemTemplate);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case PARTICLESYSTEM_EVENT_ENABLE:
            //LoadFromTemplate();
            Reset();
            SetPauseParticles(false);
            SetRenderParticles(true);

            break;
        case PARTICLESYSTEM_EVENT_DISABLE:
            SetPauseParticles(true);
            SetRenderParticles(false);

            break;
        case PARTICLESYSTEM_EVENT_STARTEMIT:
            SetEmitNewParticles(true);
            //m_bEmitNewParticles = true;
            break;
        case PARTICLESYSTEM_EVENT_STOPEMIT:
            SetEmitNewParticles(false);
            //m_bEmitNewParticles = false;
            break;
        default:
            CSpriteEntity::OnInput(eventId, eventParameter);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    bool CParticleSystemEntity::Initialize(CRenderDevice* pDevice)
    {
        m_pDevice = pDevice;

        return true;
    }

    inline size_t GetParticleStride(const std::string& strRenderer)
    {
        return (strRenderer == "Distort") ? sizeof(CParticleDistortGPU) : sizeof(CParticleColorGPU);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::OnResetDevice()
    {
        if (m_pParticleVB || m_numTotalParticles == 0)
            return;


        m_pParticleVB = m_pDevice->CreateVertexBuffer(GetParticleStride(m_strRenderer) * m_numTotalParticles * 6, USAGE_TYPE_VERTEXINDEXBUFFERS);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::OnLostDevice()
    {
        ENV_DELETE(m_pParticleVB);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetRenderEditor(bool bEditor)
    {
        CSpriteEntity::SetRenderEditor(bEditor);

        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            CParticleEmitterEntity* pEmitter = it->second;
            pEmitter->SetRenderEditor(bEditor);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetRenderGame(bool bGame)
    {
        CSpriteEntity::SetRenderGame(bGame);

        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            CParticleEmitterEntity* pEmitter = it->second;
            pEmitter->SetRenderGame(bGame);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetRender(bool bRender)
    {
        CSpriteEntity::SetRender(bRender);

        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            CParticleEmitterEntity* pEmitter = it->second;
            pEmitter->SetRender(bRender);
        }
    }

    //-----------------------------------------------------------------------------------
    void CreateTangentVectors(CParticleDistortGPU& p0, CParticleDistortGPU& p1, CParticleDistortGPU& p2)
    {
        float u0 = p0.texCoords.x;
        float v0 = p0.texCoords.y;
        float u1 = p1.texCoords.x;
        float v1 = p1.texCoords.y;
        float u2 = p2.texCoords.x;
        float v2 = p2.texCoords.y;
        float s1 = u1 - u0;
        float t1 = v1 - v0;
        float s2 = u2 - u0;
        float t2 = v2 - v0;
        Vec3 Q1 = Vec3(p1.position - p0.position);
        Vec3 Q2 = Vec3(p2.position - p0.position);
        Vec3 vTangent = Vec3(1.0, 1.0, 1.0) * 1.0f / (s1 * t2 - s2 * t1);
        Vec3 vBinormal = Vec3(1.0, 1.0, 1.0) * 1.0f / (s1 * t2 - s2 * t1);
        vTangent.x *= (t2 * Q1.x - t1 * Q2.x);
        vTangent.y *= (t2 * Q1.y - t1 * Q2.y);
        vTangent.z *= (t2 * Q1.z - t2 * Q2.z);
        vBinormal.x *= (-s2 * Q1.x + s1 * Q2.x);
        vBinormal.y *= (-s2 * Q1.y + s1 * Q2.y);
        vBinormal.z *= (-s2 * Q1.z + s1 * Q2.z);

        vTangent = glm::normalize(vTangent);
        vBinormal = glm::normalize(vBinormal);

        p0.tangent = Vec4(vTangent, 0.0f);
        p0.binormal = Vec4(vBinormal, 0.0f);
        p1.tangent = Vec4(vTangent, 0.0f);
        p1.binormal = Vec4(vBinormal, 0.0f);
        p2.tangent = Vec4(vTangent, 0.0f);
        p2.binormal = Vec4(vBinormal, 0.0f);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetupParticleDistort(CParticleDistortGPU* pParticle, 
                                                     CParticleCPU* pCurParticle)
    {
        ENV_ASSERT_DBG(pParticle);

        for (int j = 0; j < 6; ++j)
        {
            pParticle[j].distortion = pCurParticle->m_distortion;
        }

        CreateTangentVectors(pParticle[1], pParticle[2], pParticle[0]);
        CreateTangentVectors(pParticle[3], pParticle[4], pParticle[5]);
    }

    //-----------------------------------------------------------------------------------
    template< class Particle >
    void CParticleSystemEntity::SetupParticlesColor(Particle* pParticle,
                                                    CParticleCPU* pCurParticle, 
                                                    const Vec4& particlePos, 
                                                    const Vec4& texCoords,
                                                    const Vec3 particleVertices[4],
                                                    const Matrix4x4& mtxInvView,
                                                    const PARTICLE_FACING particleEmitterFacing)
    {
        pParticle[0].position = Vec4(particleVertices[0], 0);
        pParticle[0].position += particlePos;
        if (particleEmitterFacing == PARTICLE_FACING_SCREEN)
            pParticle[0].position = pParticle[0].position * mtxInvView;
        pParticle[0].color = pCurParticle->color;
        pParticle[0].texCoords = Vec2(texCoords.x, texCoords.y);
        pParticle[0].blending = pCurParticle->blending;

        pParticle[1].position = Vec4(particleVertices[1], 0);
        pParticle[1].position += particlePos;
        if (particleEmitterFacing == PARTICLE_FACING_SCREEN)
            pParticle[1].position = pParticle[1].position * mtxInvView;
        pParticle[1].color = pCurParticle->color;
        pParticle[1].texCoords = Vec2(texCoords.x, texCoords.w);
        pParticle[1].blending = pCurParticle->blending;

        pParticle[2].position = Vec4(particleVertices[2], 0);
        pParticle[2].position += particlePos;
        if (particleEmitterFacing == PARTICLE_FACING_SCREEN)
            pParticle[2].position = pParticle[2].position * mtxInvView;
        pParticle[2].color = pCurParticle->color;
        pParticle[2].texCoords = Vec2(texCoords.z, texCoords.w);
        pParticle[2].blending = pCurParticle->blending;

        pParticle[3].position = Vec4(particleVertices[2], 0);
        pParticle[3].position += particlePos;
        if (particleEmitterFacing == PARTICLE_FACING_SCREEN)
            pParticle[3].position = pParticle[3].position * mtxInvView;
        pParticle[3].color = pCurParticle->color;
        pParticle[3].texCoords = Vec2(texCoords.z, texCoords.w);
        pParticle[3].blending = pCurParticle->blending;

        pParticle[4].position = Vec4(particleVertices[3], 0);
        pParticle[4].position += particlePos;
        if (particleEmitterFacing == PARTICLE_FACING_SCREEN)
            pParticle[4].position = pParticle[4].position * mtxInvView;
        pParticle[4].color = pCurParticle->color;
        pParticle[4].texCoords = Vec2(texCoords.z, texCoords.y);
        pParticle[4].blending = pCurParticle->blending;

        pParticle[5].position = Vec4(particleVertices[0], 0);
        pParticle[5].position += particlePos;
        if (particleEmitterFacing == PARTICLE_FACING_SCREEN)
            pParticle[5].position = pParticle[5].position * mtxInvView;
        pParticle[5].color = pCurParticle->color;
        pParticle[5].texCoords = Vec2(texCoords.x, texCoords.y);
        pParticle[5].blending = pCurParticle->blending;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CParticleSystemEntity::AddToRenderQueue);

        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (!m_bRenderParticles)
            return;


        CCamera* pCamera = gEngine->GetWorldMgr()->GetActiveCamera();

        Vec3 vRainPos = pCamera->GetAttachedNode()->GetDerivedPosition();
        float z = m_pNode->GetDerivedPosition().z;

        switch (m_alignment)
        {
        case PARTICLESYSTEM_ALIGNMENT_LEFT:
        {
            float xWorld = pCamera->GetFrustumHalfWidth(-vRainPos.z + z) - 50.0f;
            vRainPos.x -= xWorld;
        }
        break;
        case PARTICLESYSTEM_ALIGNMENT_TOP:
        {
            float yWorld = pCamera->GetFrustumHalfHeight(-vRainPos.z + z) - 50.0f;
            vRainPos.y += yWorld;
        }
        break;
        case PARTICLESYSTEM_ALIGNMENT_BOTTOM:
        {
            float yWorld = pCamera->GetFrustumHalfHeight(-vRainPos.z + z) - 50.0f;
            vRainPos.y -= yWorld;
        }
        break;
        case PARTICLESYSTEM_ALIGNMENT_RIGHT:
        {
            float xWorld = pCamera->GetFrustumHalfWidth(-vRainPos.z + z) - 50.0f;
            vRainPos.x += xWorld;
        }
        break;
        case PARTICLESYSTEM_ALIGNMENT_CENTER:
            break;
        default:
            break;
        }


        if (m_alignment != PARTICLESYSTEM_ALIGNMENT_NONE)
        {
            vRainPos.z = z;
            m_pNode->SetPosition(vRainPos);
            //m_pNode->PropagateTransform();
        }

        Matrix4x4 mtxId(1.0f);;


        int numParticles = 0;
        m_numTotalParticlesAlive = 0;
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            CParticleEmitterEntity* pEmitter = it->second;

            std::vector< CParticleCPU* >& particles = pEmitter->GetParticles();

            for (mapAffectors::iterator jt = m_affectors.begin(); jt != m_affectors.end(); ++jt)
            {
                for (int i = 0; i < pEmitter->GetNumParticlesAlive(); ++i)
                {
                    jt->second->AffectParticle(*particles[i], pEmitter, dt);
                }
            }

            if (!pEmitter->GetClipping() || intersection::FrustumIntersectsBB(gEngine->GetWorldMgr()->GetActiveCamera()->GetFrustum(), pEmitter->GetBoundingBox(), mtxId)
                != intersection::INTERSECTION_OUTSIDE)
                m_numTotalParticlesAlive += pEmitter->GetNumParticlesAlive();

            numParticles += pEmitter->GetNumParticles();
        }

        if (m_numTotalParticles != numParticles)
        {
            m_numTotalParticles = numParticles;
            ENV_DELETE(m_pParticleVB);
            OnResetDevice();
        }

        //ENV_DBG_CODE(m_profiler->BeginProfiling<CCPUProfiler>("PARTICLESYSTEMENTITY", "Update"));

        /* Update the VertexBuffer */
        int numEmittersRendered = 0;
        if (m_numTotalParticlesAlive > 0 && m_pParticleVB)
        {
            void* pParticle = /*static_cast<CParticleColorGPU*>*/(m_pParticleVB->Lock(0, GetParticleStride(m_strRenderer) * 6 * m_numTotalParticlesAlive, 0));

            int i = 0;
            for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
            {
                CParticleEmitterEntity* pEmitter = it->second;

                if (pEmitter->GetClipping() && intersection::FrustumIntersectsBB(gEngine->GetWorldMgr()->GetActiveCamera()->GetFrustum(), pEmitter->GetBoundingBox(), mtxId) == intersection::INTERSECTION_OUTSIDE)
                    continue;



                for (int j = 0; j < pEmitter->GetNumParticlesAlive(); ++j)
                {
                    std::vector< CParticleCPU* >& particles = pEmitter->GetParticles();


                    Vec4 texCoords(0, 0, 0, 0);
                    float aspectRatio = 1.0f;
                    if (m_pMaterial)
                    {
                        //int seq = particles[j]->sequence % m_pMaterial->GetNumSequences();
                        m_pMaterial->GetTextureCoordinates(pEmitter->GetMaterialSequence(), 0, texCoords.x, texCoords.y, texCoords.z, texCoords.w);
                        aspectRatio = (float)m_pMaterial->GetFrameWidth() / m_pMaterial->GetFrameHeight();
                    }

                    // Compute the derived velocity. All subsequent computations can
                    // be uniformly performed in world space, even for particles simulated in their
                    // local coordinate system.
                    Vec3 derivedVelocity(particles[j]->m_initialAcceleration);
                    if (pEmitter->GetGlobalSpaceParticles())
                    {
                        derivedVelocity = particles[j]->m_velocity;
                    }
                    else
                    {
                        Matrix4x4 mtxEmitterRST = pEmitter->GetNode()->GetDerivedRST();
                        mtxEmitterRST = glm::inverse(mtxEmitterRST);
                        mtxEmitterRST = glm::transpose(mtxEmitterRST);
                        derivedVelocity = Vec3(Vec4(derivedVelocity, 0.0f) * mtxEmitterRST);
                    }

                    derivedVelocity = glm::normalize(derivedVelocity);


                    CParticleCPU* pCurParticle = particles[j];
                    Vec4 particlePos = pCurParticle->position;


                    // Create the four vertices, making up every particle with the correct size.
                    // These vertices are used in the following the orient the particle according
                    // to the chosen settings.
                    Vec3 particleVertices[4];
                    particleVertices[0] = Vec3(aspectRatio * -particles[j]->m_size, -particles[j]->m_size, 0.0f);
                    particleVertices[1] = Vec3(aspectRatio * -particles[j]->m_size,  particles[j]->m_size, 0.0f);
                    particleVertices[2] = Vec3(aspectRatio *  particles[j]->m_size,  particles[j]->m_size, 0.0f);
                    particleVertices[3] = Vec3(aspectRatio *  particles[j]->m_size, -particles[j]->m_size, 0.0f);




                    // Align the particles.
                    Vec3 up(0.0f, 1.0f, 0.0f);
                    Vec3 dir(derivedVelocity);
                    Vec3 right(1.0f, 0.0f, 0.0f);

                    if (pEmitter->GetFacing() != PARTICLE_FACING_SCREEN)
                    {
                        // The particle is created with the corresponding size in the plane defined 
                        // by the 'right' and 'dir' axis. The 'up' axis therefore defines the 
                        // particles normal.

                        if (pEmitter->GetFacing() == PARTICLE_FACING_CAMERA)
                        {
                            // Particle faces the camera: adjust the 'up' axis (normal) to 
                            // point the camera.
                            up = -(pCamera->GetAttachedNode()->GetDerivedPosition() - Vec3(particlePos));
                            right = glm::cross(dir, up);
                        }
                        else if (pEmitter->GetFacing() == PARTICLE_FACING_HORIZONTAL)
                        {
                            // Particle faces horizontally according to the orientation of 
                            // the EMITTER, not the global up axis.

                            up = pEmitter->GetNode()->GetDerivedDirection();
                            dir = derivedVelocity;

                            float length = glm::length(dir);
                            up = glm::normalize(up);
                            dir = glm::normalize(dir);

                            float angle = glm::dot(up, dir);
                            if (pEmitter->GetOrientation() == PARTICLE_ORIENTATION_TO_VELOCITY &&
                                angle < 0.99f && length > 0.01f)
                            {
                                // The velocity points into a different direction than the particles 'up' axis.
                                // This allows to rotate the particle according to the current veloctiy.
                                float t = ComputePointOnLineProjection(derivedVelocity, Vec3(0.0f, 0.0f, 0.0f), pEmitter->GetNode()->GetDerivedDirection());

                                dir = derivedVelocity - t * pEmitter->GetNode()->GetDerivedDirection();
                            }
                            else
                            {
                                // The 'up' and velocity vectors point into exactly the same direction,
                                // such that there is no way to rotate the particle. 
                                dir = pEmitter->GetNode()->GetDerivedUp();
                            }

                            right = glm::cross(dir, up);
                        }
                        else if (pEmitter->GetFacing() == PARTICLE_FACING_EMITTER)
                        {
                            // Nonsense
                            right = pEmitter->GetNode()->GetDerivedRight();
                            up = glm::cross(right, dir);
                        }
                        else
                        {
                            // Shouldn't happen.
                            right = glm::cross(dir, up);
                        }

                        dir = glm::normalize(dir);
                        up = glm::normalize(up);
                        right = glm::normalize(right);


                        particleVertices[0] = particleVertices[0].x * right + particleVertices[0].y * dir;
                        particleVertices[1] = particleVertices[1].x * right + particleVertices[1].y * dir;
                        particleVertices[2] = particleVertices[2].x * right + particleVertices[2].y * dir;
                        particleVertices[3] = particleVertices[3].x * right + particleVertices[3].y * dir;
                    }
                    else
                    {
                        up = Vec3(0.0f, 0.0f, 1.0f);

                        particlePos = particlePos * pCamera->GetViewMatrix();
                    }


                    // Rotate the particles.
                    // - Screen-facing particles are billboards, which can be rotated on the screen. There is 
                    //   no unique way to rotate screen-facing particles correctly according to the velocity, 
                    //   because they change their orientation according to the camera. This would require to rotate
                    //   them differently every frame, which results in artifacts. Therefore, these particles
                    //   are rotated only ONCE after emittance according to the current view.
                    //
                    // - Non-screen facing praticles are oriented to velocity automatically, such that
                    //   rotation is only necessary for random orientation.
                    //
                    if (pEmitter->GetFacing() == PARTICLE_FACING_SCREEN &&
                        pEmitter->GetOrientation() == PARTICLE_ORIENTATION_TO_VELOCITY)
                    {
                        // Rotate the particle in view-space around z-axis, such that it remains
                        // screen-facing. Compute the angle of rotation by projecting the velocity
                        // onto the screen and omit the z coordinate. The particle is afterwards 
                        // aligned according to the resulting 2D screen-space vector.
                        Matrix4x4 mtxInvTransView = glm::transpose(glm::inverse(pCamera->GetViewMatrix()));


                        derivedVelocity = Vec3(Vec4(derivedVelocity, 0.0f) * mtxInvTransView);

                        float angle = std::atan2(derivedVelocity.x, derivedVelocity.y);

                        if (Equals(particles[j]->m_angle, 0.0f))
                        {
                            particles[j]->m_angle = angle;
                        }
                        Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), -particles[j]->m_angle, Vec3(0.0f, 0.0f, 1.0f)));

                        for (int i = 0; i < 4; ++i)
                            particleVertices[i] = Vec3(Vec4(particleVertices[i], 1.0f) * mtxRot);
                    }
                    else if (pEmitter->GetOrientation() == PARTICLE_ORIENTATION_RANDOM)
                    {
                        // Rotate the pre-aligned particles along their local 'up' axis (normal).
                        Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), particles[j]->m_angle, up));

                        for (int i = 0; i < 4; ++i)
                            particleVertices[i] = Vec3(Vec4(particleVertices[i], 1.0f) * mtxRot);
                    }

                    Matrix4x4 mtxInvView = glm::inverse(pCamera->GetViewMatrix());

                    if (m_strRenderer == "Distort")
                    {
                        CParticleDistortGPU* pParticleDistortGPU = static_cast<CParticleDistortGPU*>(pParticle);
                        SetupParticlesColor(pParticleDistortGPU, pCurParticle, particlePos, texCoords, particleVertices, mtxInvView, pEmitter->GetFacing());
                        SetupParticleDistort(pParticleDistortGPU, pCurParticle );

                        pParticleDistortGPU += 6;
                        pParticle = pParticleDistortGPU;
                    }
                    else
                    {
                        CParticleColorGPU* pParticleColorGPU = static_cast<CParticleColorGPU*>(pParticle);
                        SetupParticlesColor(pParticleColorGPU, pCurParticle, particlePos, texCoords, particleVertices, mtxInvView, pEmitter->GetFacing());

                        pParticleColorGPU += 6;
                        pParticle = pParticleColorGPU;
                    }

                }

                i += pEmitter->GetNumParticlesAlive();
                ++numEmittersRendered;

            }

            m_pParticleVB->Unlock();
        }

        // std::stringstream str;
        // str << "Emitters rendered: " << numEmittersRendered << " / " << m_emitters.size() << std::endl;
        // ENV_LOG_INFO(str.str());
        // ENV_DBG_CODE(m_profiler->EndProfiling("PARTICLESYSTEMENTITY"));

        m_renderObject.m_numTotalParticlesAlive = static_cast<unsigned int>(m_numTotalParticlesAlive);
        m_renderObject.m_pMaterial = m_pMaterial;
        m_renderObject.m_pVertexBuffer = m_pParticleVB;

        if (m_numTotalParticlesAlive > 0)
        {
            queue.m_particleSystems[m_strRenderer].push_back(&m_renderObject);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

        if (!m_bRenderParticles)
            return;

        m_time += dt;
    }

    //-----------------------------------------------------------------------------------
    const std::string& CParticleSystemEntity::GetParticleSystemTemplate()
    {
        return m_strParticleSystemTemplate;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetAlignment(PARTICLESYSTEM_ALIGNMENT alignment)
    {
        m_alignment = alignment;
    }

    //-----------------------------------------------------------------------------------
    env::PARTICLESYSTEM_ALIGNMENT CParticleSystemEntity::GetAlignment()
    {
        return m_alignment;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetParticleRotationAngle(float angle)
    {
        m_currentAngle = angle;
    }

    //-----------------------------------------------------------------------------------
    float CParticleSystemEntity::GetParticleRotationAngle()
    {
        return m_currentAngle;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SaveAsTemplate(const std::string& strName)
    {
        CScript* pTemplate = gEngine->GetParticleSystemMgr()->CreateParticleSystemTemplate(strName);

        CScript& script = *pTemplate;
        script.RemoveSubscripts();
        //script["autoScale"] = false;
        script["texture"] = m_strMaterial;
        script["renderer"] = GetRenderer();
        script["loadFromTemplate"] = m_bLoadFromTemplate;


        CScript* pScriptAffectors = script.GetSubscript("Affectors");
        if (!pScriptAffectors)
            pScriptAffectors = script.AddSubscript("Affectors");

        for (mapAffectors::iterator it = m_affectors.begin(); it != m_affectors.end(); ++it)
        {
            (*pScriptAffectors)[it->first] = it->first;
        }

        int i = 0;
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it, ++i)
        {
            std::stringstream strEmitter;
            strEmitter << "Emitter#" << i;
            script.RemoveSubscript(strEmitter.str());
            CScript* pEmitter = script.AddSubscript(strEmitter.str());
            assert(pEmitter);

            it->second->SaveAsTemplate(*pEmitter);
        }

        gEngine->GetParticleSystemMgr()->SaveTemplates();

        m_strParticleSystemTemplate = strName;
    }

    //-----------------------------------------------------------------------------------
    bool CParticleSystemEntity::LoadFromTemplate(const std::string& strType)
    {
        Unload();

        m_strParticleSystemTemplate = strType;

        CScript* pScript = gEngine->GetParticleSystemMgr()->GetParticleSystemTemplate(strType);
        if (!pScript)
            return false;

        CScript& particleSystem = *pScript;


        //if(particleSystem.Exists("autoScale"))
        //	SetAutoScale(particleSystem["autoScale"]);

        if (particleSystem.Exists("animation"))
            SetParticleMaterialFilename(particleSystem["animation"]);
        else if (particleSystem.Exists("texture"))
            SetParticleMaterialFilename(particleSystem["texture"]);

        if (particleSystem.Exists("renderer"))
            SetRenderer(particleSystem["renderer"]);
        else
            SetRenderer("Default");

        if (particleSystem.Exists("loadFromTemplate"))
            m_bLoadFromTemplate = particleSystem["loadFromTemplate"];

        CScript::mapSubscripts& emitters = particleSystem.GetSubscripts();
        for (CScript::mapSubscripts::iterator it = emitters.begin(); it != emitters.end(); ++it)
        {
            if (it->first == "Affectors")
            {
                CScript& affectors = *it->second;
                std::string strName;
                CBaseParam::PARAMETER_TYPE paramType;
                while (affectors.GetNext(strName, paramType) && paramType == CBaseParam::PARAMETER_TYPE_STRING)
                {
                    IParticleAffector* pAffector = gEngine->GetParticleSystemMgr()->CreateParticleAffector(affectors[strName]);
                    m_affectors[strName] = pAffector;
                }
            }
            else
            {
                CScript& emitter = *it->second;
                ENTITY_TYPEID emitterEntityId = CParticleEmitterEntity::ENTITY_META_TYPEID;

                if (emitter.Exists("EmitterEntityId"))
                {
                    emitterEntityId = (ENTITY_TYPEID)(int)(emitter["EmitterEntityId"]);

                    CParticleEmitterEntity* pEmitter = static_cast<CParticleEmitterEntity*>(gEngine->GetEntityMgr()->CreateEntity(emitterEntityId));
                    AddEmitter(pEmitter->GetName());
                    pEmitter->LoadFromTemplate(emitter);
                }

            }
        }

        return true;
    }

    void CParticleSystemEntity::OnEntityCloned(CEntity& entity)
    {
        CSpriteEntity::OnEntityCloned(entity);

        CParticleSystemEntity& other = static_cast<CParticleSystemEntity&>(entity);

        m_bLoadFromTemplate = other.m_bLoadFromTemplate;
        m_strParticleSystemTemplate = other.m_strParticleSystemTemplate;

        if (!m_strParticleSystemTemplate.empty())
            LoadFromTemplate(m_strParticleSystemTemplate);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::Unload()
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->SetParticleSystem(NULL);
            gEngine->GetEntityMgr()->RemoveEntity(it->second->GetName());
        }
        m_emitters.clear();

        for (mapAffectors::iterator it = m_affectors.begin(); it != m_affectors.end(); ++it)
            ENV_DELETE(it->second);

        m_affectors.clear();

        if (m_pMaterial)
        {
            gMaterialMgr->Release(m_pMaterial->GetName());
            m_pMaterial = NULL;
        }

        m_numTotalParticlesAlive = 0;
        m_numTotalParticles = 0;

        ENV_DELETE(m_pParticleVB);
    }

    //-----------------------------------------------------------------------------------
    CParticleEmitterEntity* CParticleSystemEntity::AddEmitter(const std::string& strEmitter)
    {
        mapEmitters::iterator it = m_emitters.find(strEmitter);
        if (it != m_emitters.end())
            return it->second;

        CParticleEmitterEntity* pEmitter = static_cast<CParticleEmitterEntity*>(gEngine->GetEntityMgr()->GetEntity(strEmitter));
        if (pEmitter)
        {
            pEmitter->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            pEmitter->SetParticleSystem(this);
            pEmitter->SetAutoDelete(false);
            pEmitter->SetTime(m_time);
            if (m_pNode)
                m_pNode->AttachChildNode(pEmitter->GetNode());
            m_emitters[pEmitter->GetName()] = pEmitter;

            return pEmitter;
        }

        return NULL;
    }

    //-----------------------------------------------------------------------------------
    IParticleAffector* CParticleSystemEntity::AddAffector(const std::string& strName)
    {
        mapAffectors::iterator it = m_affectors.find(strName);
        if (it != m_affectors.end())
            return it->second;

        IParticleAffector* pAffector = gEngine->GetParticleSystemMgr()->CreateParticleAffector(strName);
        if (pAffector)
            m_affectors[strName] = pAffector;
        return pAffector;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::RemoveAffector(const std::string& strName)
    {
        mapAffectors::iterator it = m_affectors.find(strName);
        if (it != m_affectors.end())
        {
            ENV_DELETE(it->second);
            m_affectors.erase(it);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::RemoveEmitter(const std::string& strName)
    {
        mapEmitters::iterator it = m_emitters.find(strName);
        if (it != m_emitters.end())
        {
            it->second->SetParticleSystem(NULL);
            gEngine->GetEntityMgr()->RemoveEntity(it->second->GetName());
            m_emitters.erase(it);
        }
    }

    //-----------------------------------------------------------------------------------
    CParticleEmitterEntity* CParticleSystemEntity::GetEmitter(const std::string& str)
    {
        mapEmitters::iterator it = m_emitters.find(str);
        if (it == m_emitters.end())
            return NULL;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::EmitAllParticlesOnce()
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->EmitParticles();
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetEmitNewParticles(bool bSet)
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->SetEmitNewParticles(bSet);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetPickable(bool bPickable)
    {
        CSpriteEntity::SetPickable(bPickable);

        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->SetPickable(bPickable);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetStartDisabled(bool bSet)
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->SetStartDisabled(bSet);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetPauseParticles(bool bSet)
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->SetPauseParticles(bSet);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::Reset()
    {
        for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
        {
            it->second->Reset();
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemEntity::SetParticleMaterialFilename(const std::string& strTexture)
    {
        if (m_pMaterial)
            gMaterialMgr->Release(m_pMaterial->GetName());

        m_strMaterial = strTexture;

        m_pMaterial = gMaterialMgr->AddGetPtr(m_strMaterial);

        if (m_pMaterial && m_pMaterial->GetTexture())
        {
            m_pMaterial->GetTexture()->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        }
    }

    //-----------------------------------------------------------------------------------
    CVertexBuffer* CParticleSystemEntity::GetParticleVB()
    {
        return m_pParticleVB;
    }

} /* env */

