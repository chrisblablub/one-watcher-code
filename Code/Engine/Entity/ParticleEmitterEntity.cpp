#include <Engine/Entity/ParticleEmitterEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/ParticleSystemMgr.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Camera.h>

namespace env
{
    ENTITY_TYPEID CParticleEmitterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_EMITTER;

    //-----------------------------------------------------------------------------------
    CParticleEmitterEntity::CParticleEmitterEntity()
        : m_currentEmitTime(0.0)
        , m_currentEmitDuration(FLT_MAX)
        , m_particlesToEmit(0.0)
        , m_time(0.0)
        , m_velocityAngle(90.0f)
        , m_vColorBegin(1.0f, 1.0f, 1.0f, 1.0f)
        , m_vColorEnd(1.0f, 1.0f, 1.0f, 1.0f)
        , m_accelerationMin(0.0f)
        , m_accelerationMax(0.0f)
        , m_velocityBegin(20.0f)
        , m_velocityEnd(40.0f)
        , m_velocityExp(1.0f)
        , m_velocityRange(0.2f)
        , m_lifetimeMin(1.0f)
        , m_lifetimeMax(1.5f)
        , m_emitRate(5)
        , m_emitDuration(0.0)
        , m_sequence(0)
        , m_pParticleSystem(NULL)
        , m_bEmitNewParticles(true)
        , m_bGlobalSpaceParticles(true)
        , m_bDynamicBBox(true)
        , m_orientation(PARTICLE_ORIENTATION_TO_VELOCITY)
        , m_facing(PARTICLE_FACING_CAMERA)
        , m_orientationSpeedMin(-0.5f)
        , m_orientationSpeedMax(0.5f)
        , m_distortionMin(0.0f)
        , m_distortionMax(1.0f)
        , m_particleSize(40)
        , m_particleSizeBegin(40)
        , m_particleSizeEnd(40)
        , m_particleSizeRange(0.0f)
        , m_numParticles(50)
        , m_numParticlesAlive(0)
        , m_numParticlesEmitted(0)
        , m_fadeInTime(0.1f)
        , m_fadeOutTime(0.1f)
        , m_warmUpTime(0.0f)
        , m_blendMode(0.0f)
        , m_prevPosition(0.0f, 0.0f, 0.0f)
        , m_vBBoxMin(0.0f, 0.0f, 0.0f)
        , m_vBBoxMax(0.0f, 0.0f, 0.0f)
        , m_bClipping(true)
        , m_bVelocityFromCenter(false)
        , m_bPauseParticles(false)
        , m_bEmitAllParticlesOnce(false)
        , m_bStartDisabled(false)
        , m_emitDelay(0.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    CParticleEmitterEntity::~CParticleEmitterEntity()
    {
        Unload();


        if (m_pParticleSystem)
            m_pParticleSystem->RemoveEmitter(GetName());
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::Unload()
    {
        m_time = 0.0f;
        m_currentEmitTime = 0.0f;
        m_particlesToEmit = 0.0f;

        for (int i = 0; i < m_particles.size(); ++i)
            ENV_DELETE(m_particles[i]);

        m_numParticlesAlive = 0;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::Reset()
    {
        m_time = 0.0f;
        m_currentEmitTime = 0.0f;
        m_currentEmitDuration = m_emitDuration;
        m_numParticlesEmitted = 0;
        m_numParticlesAlive = 0;
        m_bEmitNewParticles = true;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::EmitParticles()
    {
        Reset();

        SetEmitNewParticles(true);

        m_currentEmitDuration = m_emitDuration;

    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        Reset();

        if (m_bStartDisabled)
        {
            m_bEmitNewParticles = false;
        }

        m_bPauseParticles = false;

        m_particlesToEmit = 0.0f;
        m_numParticlesAlive = 0;
        m_numParticlesEmitted = 0;

        m_prevPosition = Vec3(0.0f, 0.0f, 0.0f);

        if (Equals(m_warmUpTime, 0.0f))
            return;

        for (float time = 0.0f; time < m_warmUpTime; time += 1.0f)
        {
            Update(1.0f);
        }
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::SaveAsTemplate(CScript& script)
    {
        script["EmitterEntityId"] = GetMetaTypeId();
        script["colorBegin"] = GetColorBegin();
        script["colorEnd"] = GetColorEnd();
        script["accelerationMin"] = GetAccelerationMin();
        script["accelerationMax"] = GetAccelerationMax();
        script["velocityBegin"] = GetVelocityBegin();
        script["velocityEnd"] = GetVelocityEnd();
        script["velocityRange"] = GetVelocityRange();
        script["velocityExp"] = GetVelocityExp();
        script["lifeTimeMin"] = GetLifeTimeMin();
        script["lifeTimeMax"] = GetLifeTimeMax();
        const CQuaternion& quat = m_pNode->GetOrientation();
        script["yawPitchRoll"] = Vec3(quat.GetYaw(), quat.GetPitch(), quat.GetRoll());
        script["position"] = m_pNode->GetPosition();
        script["velocityAngle"] = GetVelocityAngle();
        script["emitRate"] = GetEmitRate();
        script["emitDuration"] = GetEmitDuration();
        script["emitDelay"] = GetEmitDelay();
        script["animSequence"] = 0;
        script["particleSizeBegin"] = GetParticleSizeBegin();
        script["particleSizeEnd"] = GetParticleSizeEnd();
        script["particleSizeRange"] = GetParticleSizeRange();
        script["numParticles"] = (int)GetNumParticles();
        script["fadeIn"] = GetFadeInTime();
        script["fadeOut"] = GetFadeOutTime();
        script["dynamicBBox"] = GetDynamicBoundingBox();
        script["clipping"] = GetClipping();
        script["velocityFromCenter"] = GetVelocityFromCenter();
        script["sequence"] = (int)GetMaterialSequence();
        script["blendMode"] = GetBlendMode();
        script["orientation"] = (int)GetOrientation();
        script["orientationSpeedMin"] = GetOrientationSpeedMin();
        script["orientationSpeedMax"] = GetOrientationSpeedMax();
        script["distortionMin"] = GetDistortionMin();
        script["distortionMax"] = GetDistortionMax();

        if (!GetDynamicBoundingBox())
        {
            script["bboxMin"] = GetBoundingBoxMin();
            script["bboxMax"] = GetBoundingBoxMax();
        }

        script["warmUpTime"] = GetWarmUpTime();
        script["globalSpace"] = GetGlobalSpaceParticles();
        script["facing"] = (int)GetFacing();
        script["emitAllParticlesOnce"] = GetEmitAllParticlesOnce();
        script["startDisabled"] = GetStartDisabled();
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::LoadFromTemplate(CScript& emitter)
    {
        Unload();

        if (emitter.Exists("color"))
            SetColor(emitter["color"]);
        else
        {
            if (emitter.Exists("colorBegin"))
                SetColorBegin(emitter["colorBegin"]);

            if (emitter.Exists("colorEnd"))
                SetColorEnd(emitter["colorEnd"]);
        }

        if (emitter.Exists("acceleration"))
            SetAcceleration(emitter["acceleration"]);
        else
        {
            if (emitter.Exists("accelerationMin"))
                SetAccelerationMin(emitter["accelerationMin"]);

            if (emitter.Exists("accelerationMax"))
                SetAccelerationMax(emitter["accelerationMax"]);
        }

        if (emitter.Exists("velocity"))
            SetVelocity(emitter["velocity"]);
        else
        {
            if (emitter.Exists("velocityBegin"))
                SetVelocityBegin(emitter["velocityBegin"]);

            if (emitter.Exists("velocityEnd"))
                SetVelocityEnd(emitter["velocityEnd"]);
        }

        if (emitter.Exists("velocityExp"))
            SetVelocityExp(emitter["velocityExp"]);

        if (emitter.Exists("velocityRange"))
            SetVelocityRange(emitter["velocityRange"]);


        if (emitter.Exists("lifeTime"))
            SetLifeTime(emitter["lifeTime"]);
        else
        {
            if (emitter.Exists("lifeTimeMin"))
                SetLifeTimeMin(emitter["lifeTimeMin"]);

            if (emitter.Exists("lifeTimeMax"))
                SetLifeTimeMax(emitter["lifeTimeMax"]);
        }

        if (emitter.Exists("yawPitchRoll"))
        {
            Vec3 orientation = emitter["yawPitchRoll"];
            m_pNode->SetOrientation(CQuaternion(orientation.x, orientation.y, orientation.z));
        }

        if (emitter.Exists("position"))
            m_pNode->SetPosition(emitter["position"]);

        if (emitter.Exists("velocityAngle"))
            SetVelocityAngle(emitter["velocityAngle"]);

        if (emitter.Exists("emitRate"))
            SetEmitRate(emitter["emitRate"]);

        if (emitter.Exists("emitDuration"))
            SetEmitDuration(emitter["emitDuration"]);

        if (emitter.Exists("emitDelay"))
            SetEmitDelay(emitter["emitDelay"]);

        if (emitter.Exists("blendMode"))
            SetBlendMode(emitter["blendMode"]);

        //if(emitter.Exists("size"))
        //	m_size = emitter["size"];

        if (emitter.Exists("particleSize"))
            SetParticleSize(emitter["particleSize"]);
        else
        {
            if (emitter.Exists("particleSizeBegin"))
                SetParticleSizeBegin(emitter["particleSizeBegin"]);

            if (emitter.Exists("particleSizeEnd"))
                SetParticleSizeEnd(emitter["particleSizeEnd"]);
        }

        if (emitter.Exists("particleSizeRange"))
            SetParticleSizeRange(emitter["particleSizeRange"]);

        if (emitter.Exists("numParticles"))
            SetNumParticles(emitter["numParticles"]);

        if (emitter.Exists("fadeIn"))
            SetFadeInTime(emitter["fadeIn"]);

        if (emitter.Exists("fadeOut"))
            SetFadeOutTime(emitter["fadeOut"]);

        SetDynamicBoundingBox(false);

        if (emitter.Exists("dynamicBBox"))
        {
            SetDynamicBoundingBox(emitter["dynamicBBox"]);
        }

        if (emitter.Exists("clipping"))
            SetClipping(emitter["clipping"]);


        if (!GetDynamicBoundingBox() && emitter.Exists("bboxMin"))
        {
            SetBoundingBoxMin(emitter["bboxMin"]);
        }

        if (!GetDynamicBoundingBox() && emitter.Exists("bboxMax"))
        {
            SetBoundingBoxMax(emitter["bboxMax"]);
        }

        if (emitter.Exists("warmUpTime"))
            SetWarmUpTime(emitter["warmUpTime"]);

        if (emitter.Exists("globalSpace"))
            SetGlobalSpaceParticles(emitter["globalSpace"]);

        if (emitter.Exists("sequence"))
            SetMaterialSequence(emitter["sequence"]);

        if (emitter.Exists("velocityFromCenter"))
            SetVelocityFromCenter(emitter["velocityFromCenter"]);

        if (emitter.Exists("orientation"))
            SetOrientation((PARTICLE_ORIENTATION)(int)emitter["orientation"]);

        if (emitter.Exists("facing"))
            SetFacing((PARTICLE_FACING)(int)emitter["facing"]);

        if (emitter.Exists("orientationSpeedMin"))
            SetOrientationSpeedMin(emitter["orientationSpeedMin"]);

        if (emitter.Exists("orientationSpeedMax"))
            SetOrientationSpeedMax(emitter["orientationSpeedMax"]);

        if (emitter.Exists("distortionMin"))
            SetDistortionMin(emitter["distortionMin"]);

        if (emitter.Exists("distortionMax"))
            SetDistortionMax(emitter["distortionMax"]);

        if (emitter.Exists("emitAllParticlesOnce"))
            SetEmitAllParticlesOnce(emitter["emitAllParticlesOnce"]);

        if (emitter.Exists("startDisabled"))
            SetStartDisabled(emitter["startDisabled"]);
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();
        SetTextPosition(Vec2(0.0, 24.0));
        SetText(GetName());
        SetMaterialFilename("Sprites/particleemitter.png");
        SetRenderGame(false);

        Unload();

        SetNumParticles(m_numParticles);

        m_pNode->SetOrientation(CQuaternion(0.0f, -PI * 0.5f, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();
        SetTextPosition(Vec2(0.0, 24.0));
        SetText(GetName());
        SetMaterialFilename("Sprites/particleemitter.png");
        SetRenderGame(false);

        SetNumParticles(m_numParticles);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);


        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
            m_boundingBoxDataCache.Initialize(m_boundingBox);
            queue.m_vector3Caches.push_back(&m_boundingBoxDataCache);
        }
    }



    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CParticleEmitterEntity::Update);

        m_pNode->SetScale(Vec3(1.0f, 1.0f, 1.0f));


        if (gEngine->GetEntityMgr()->GetActiveEntity() == this && !gEngine->GetInput()->IsKeyDown('E'))
        {
            // scale the bounding box
            if (gEngine->GetInput()->IsKeyDown(VK_CONTROL))
            {
                if (gEngine->GetInput()->IsKeyDown('X'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_vBBoxMax.x += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_vBBoxMax.x -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Y'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_vBBoxMax.y += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_vBBoxMax.y -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Z'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_vBBoxMax.z += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_vBBoxMax.z -= 16.0f;
                }
            }
            else if (gEngine->GetInput()->IsKeyDown(VK_SHIFT))
            {
                if (gEngine->GetInput()->IsKeyDown('X'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_vBBoxMin.x += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_vBBoxMin.x -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Y'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_vBBoxMin.y += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_vBBoxMin.y -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Z'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_vBBoxMin.z += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_vBBoxMin.z -= 16.0f;
                }
            }
        }


        if (!gEngine->GetEntityMgr()->GetTickParticles())
            return;

        if (m_bPauseParticles)
            return;

        m_time += dt;

        if (m_bEmitNewParticles && m_time > m_emitDelay)
        {
            if (m_currentEmitDuration <= 0.0f && m_emitDuration > 0.0f)
            {
                SetEmitNewParticles(false);
            }

            m_particlesToEmit += dt * m_emitRate;
            unsigned int numParticles = std::min((unsigned int)m_particlesToEmit, m_numParticles - m_numParticlesAlive);

            for (int i = 0; i < numParticles; ++i)
            {
                CParticleCPU& particle = *m_particles[i + m_numParticlesAlive];
                InitParticle(particle);
            }
            m_particlesToEmit -= numParticles;

            m_numParticlesAlive += numParticles;
            m_numParticlesEmitted += numParticles;

            m_currentEmitDuration -= dt;
        }

        if (m_bEmitAllParticlesOnce && m_numParticlesEmitted >= m_numParticles)
        {
            SetEmitNewParticles(false);
        }

        //if(!m_bEmitNewParticles)
        //	m_time = 0.0f;


        /* Update all alive particles */
        for (int i = 0; i < m_numParticlesAlive;)
        {
            CParticleCPU& particle = *m_particles[i];

            if (particle.m_spawnTime + particle.m_lifeTime < m_time)
            {
                /* Kill this particle */
                CParticleCPU* p = m_particles[m_numParticlesAlive - 1];
                m_particles[m_numParticlesAlive - 1] = m_particles[i];
                m_particles[i] = p;
                --m_numParticlesAlive;
            }
            else if (i < m_numParticlesAlive)
            {
                /* Update this particle */
                if (particle.m_spawnTime > m_time)
                    particle.m_spawnTime = m_time;

                float alpha = (float)(m_time - particle.m_spawnTime) / particle.m_lifeTime;


                //particle.vVelocity += particle.vAcceleration * dt;
                alpha = 1.0f - alpha;
                alpha = std::pow(alpha, m_velocityExp);
                //alpha = alpha * alpha;
                alpha = 1.0f - alpha;
                particle.m_velocity = alpha * particle.m_velocityEnd + (1.0f - alpha) * particle.m_velocityBegin;
                Vec4 velo = Vec4(particle.m_velocity, 0.0f);

                if (m_orientation == PARTICLE_ORIENTATION_RANDOM)
                    particle.m_angle += particle.m_orientSpeed * dt;

                if (m_bGlobalSpaceParticles)
                {
                    particle.m_initialPosition += dt * velo;
                    particle.position = particle.m_initialPosition;
                }
                else
                {
                    particle.m_initialPosition += dt * velo;
                    particle.position = particle.m_initialPosition * m_pNode->GetDerivedRST();
                }

                if (m_time - particle.m_spawnTime <= m_fadeInTime)
                {
                    particle.color = alpha * particle.m_endColor + (1 - alpha) * particle.m_startColor;
                    particle.color.w = ((float)(m_time - particle.m_spawnTime) / m_fadeInTime) * particle.color.w;
                }
                else if (particle.m_spawnTime + particle.m_lifeTime - m_time <= m_fadeOutTime)
                {
                    particle.color = alpha * particle.m_endColor + (1 - alpha) * particle.m_startColor;
                    particle.color.w = ((float)(particle.m_spawnTime + particle.m_lifeTime - m_time) / m_fadeOutTime) * particle.color.w;
                }
                else
                {
                    particle.color = alpha * particle.m_endColor + (1 - alpha) * particle.m_startColor;
                }

                float a = (m_time - particle.m_spawnTime) / particle.m_lifeTime;
                particle.m_size = (1.0f - a) * particle.m_startSize + a * particle.m_endSize;
                particle.blending = m_blendMode;
                particle.m_curFrame = 0;

                ++i;
            }
        }

        if (m_bDynamicBBox && m_bClipping)
        {
            // Computed in final world space 
            ComputeBoundingBox(m_vBBoxMin, m_vBBoxMax);

            m_boundingBox.ComputeBoundingVolume(m_vBBoxMin, m_vBBoxMax);
        }
        else if (m_bClipping)
        {
            // Set in local space
            Vec3 vMinGlobal = m_pNode->Transform(m_vBBoxMin);
            Vec3 vMaxGlobal = m_pNode->Transform(m_vBBoxMax);

            m_boundingBox.ComputeBoundingVolume(vMinGlobal, vMaxGlobal);
        }


        m_prevPosition = m_pNode->GetDerivedPosition();
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::ComputeBoundingBox(Vec3& vMin, Vec3& vMax)
    {
        vMin = Vec3(100000.0f, 100000.0f, 100000.0f);
        vMax = Vec3(-100000.0f, -100000.0f, -100000.0f);

        for (int i = 0; i < m_numParticlesAlive; ++i)
        {
            CParticleCPU& particle = *m_particles[i];
            if (particle.position.x < vMin.x)
                vMin.x = particle.position.x;
            if (particle.position.x > vMax.x)
                vMax.x = particle.position.x;

            if (particle.position.y < vMin.y)
                vMin.y = particle.position.y;
            if (particle.position.y > vMax.y)
                vMax.y = particle.position.y;

            if (particle.position.z < vMin.z)
                vMin.z = particle.position.z;
            if (particle.position.z > vMax.z)
                vMax.z = particle.position.z;
        }

        vMin -= std::max((1.0f + m_particleSizeRange) * m_particleSizeBegin, (1.0f + m_particleSizeRange) * m_particleSizeEnd);
        vMax += std::max((1.0f + m_particleSizeRange) * m_particleSizeBegin, (1.0f + m_particleSizeRange) * m_particleSizeEnd);

        if (vMin.x > vMax.x)
            vMax.x = vMin.x = 0.0f;
        if (vMin.y > vMax.y)
            vMax.y = vMin.y = 0.0f;
        if (vMin.z > vMax.z)
            vMax.z = vMin.z = 0.0f;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::InitParticlePosition(CParticleCPU& particle)
    {
        particle.m_initialPosition = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::InitParticleVelocityAcceleration(CParticleCPU& particle)
    {
        if (m_bVelocityFromCenter)
        {
            Vec3 dir(particle.m_initialPosition);
            particle.m_velocity = glm::normalize(dir);
        }
        else
        {
            if (m_velocityAngle > 0.0f)
            {
                float radAngle = m_velocityAngle / 360.0f * 2 * PI;
                radAngle *= 0.5f;

                float angleY = GetRandomFloat(-radAngle, radAngle);

                float angleZ = GetRandomFloat(0.0f, 2 * PI);


                if (!m_bGlobalSpaceParticles)
                {
                    // Rotate velocity around global Y and Z Axis
                    // because the initial velocity is given as the z axis
                    particle.m_velocity = Vec3(0.0f, 0.0f, 1.0f);
                    Matrix4x4 mtxRotY = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleY, Vec3(0.0f, 1.0f, 0.0f)));
                    Matrix4x4 mtxRotZ = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleZ, Vec3(0.0f, 0.0f, 1.0f)));
                    mtxRotY = mtxRotY * mtxRotZ;
                    mtxRotY = glm::transpose(glm::inverse(mtxRotY));
                    particle.m_velocity = Vec3(Vec4(particle.m_velocity, 0.0f) * mtxRotY);
                    particle.m_velocity = glm::normalize(particle.m_velocity);
                }

                else
                {
                    // Rotate velocity vector around local Y and Z Axis, because the inizial velocity
                    // is given in world space
                    particle.m_velocity = Vec3(0.0f, 0.0f, 1.0f);
                    particle.m_velocity = m_pNode->GetDerivedDirection();
                    Matrix4x4 mtxRotY = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleY, m_pNode->GetDerivedUp()));
                    Matrix4x4 mtxRotZ = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleZ, m_pNode->GetDerivedDirection()));
                    mtxRotY = mtxRotY * mtxRotZ;
                    mtxRotY = glm::transpose(glm::inverse(mtxRotY));
                    particle.m_velocity = Vec3(Vec4(particle.m_velocity, 0.0f) * mtxRotY);
                    particle.m_velocity = glm::normalize(particle.m_velocity);
                }



            }
            else
            {
                particle.m_velocity = Vec3(0.0f, 0.0f, 1.0f);
                particle.m_velocity = m_pNode->GetDerivedDirection();

                if (!m_bGlobalSpaceParticles)
                    particle.m_velocity = Vec3(0.0f, 0.0f, 1.0f);
                else
                    particle.m_velocity = m_pNode->GetDerivedDirection();
            }
        }

        particle.m_velocityBegin = particle.m_velocityEnd = particle.m_velocity;
        particle.m_velocityBegin *= m_velocityBegin;
        particle.m_velocityEnd *= m_velocityEnd;
        particle.m_velocity *= GetRandomFloat(m_velocityBegin, m_velocityEnd);
        Vec3 dir = glm::normalize(particle.m_velocity);
        particle.m_initialAcceleration = dir;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::InitParticle(CParticleCPU& particle)
    {
        InitParticlePosition(particle);

        InitParticleVelocityAcceleration(particle);

        if (m_bGlobalSpaceParticles)
        {
            Matrix4x4 mtxRST = m_pNode->GetDerivedRST();
            particle.m_initialPosition = particle.m_initialPosition * mtxRST;


            mtxRST = glm::transpose(glm::inverse(mtxRST));
            particle.m_velocity = Vec3(Vec4(particle.m_velocity, 0.0f) * mtxRST);
            particle.m_initialAcceleration = Vec3(Vec4(particle.m_initialAcceleration, 0.0f) * mtxRST);


        }

        float scale = m_pNode->GetDerivedScale().x;
        float sizeJitter = GetRandomFloat(-m_particleSizeRange, m_particleSizeRange);
        particle.m_startSize = (m_particleSizeBegin + (m_particleSizeBegin * sizeJitter)) * scale;
        particle.m_endSize = (m_particleSizeEnd + (m_particleSizeEnd * sizeJitter)) * scale;
        particle.m_size = particle.m_startSize;

        particle.m_spawnTime = m_time;
        particle.m_lifeTime = GetRandomFloat(m_lifetimeMin * scale, m_lifetimeMax * scale);

        particle.m_startColor = m_vColorBegin;
        particle.m_endColor = m_vColorEnd;
        particle.color = particle.m_startColor;

        particle.m_distortion = GetRandomFloat(m_distortionMin, m_distortionMax);

        particle.m_sequence = rand();


        if (m_orientation == PARTICLE_ORIENTATION_TO_VELOCITY)
            particle.m_angle = 0.0f;
        else
            particle.m_angle = 0.0f;

        particle.m_orientSpeed = GetRandomFloat(m_orientationSpeedMin, m_orientationSpeedMax);


        if (!m_pParticleSystem)
            return;

        CManagedMaterial* pMtrl = m_pParticleSystem->GetMaterialPtr();
        if (!pMtrl)
            return;

        TextureRect r;
        pMtrl->GetTextureCoordinates(m_sequence, 0, r.left, r.top, r.right, r.bottom);
        particle.texCoords.x = r.left;
        particle.texCoords.y = r.top;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::SetParticleSystem(CParticleSystemEntity* pSys)
    {
        m_pParticleSystem = pSys;
    }

    //-----------------------------------------------------------------------------------
    CParticleSystemEntity* CParticleEmitterEntity::GetParticleSystem()
    {
        return m_pParticleSystem;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::SetNumParticles(unsigned int numParticles)
    {
        m_numParticles = numParticles;
        m_numParticlesAlive = 0;

        if (!m_particles.empty())
        {
            for (int i = 0; i < m_particles.size(); ++i)
                ENV_DELETE(m_particles[i]);
        }

        m_particles.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_particles[i] = new CParticleCPU();

        OnResetDevice();
    }

    //-----------------------------------------------------------------------------------
    std::vector< CParticleCPU* >& CParticleEmitterEntity::GetParticles()
    {
        return m_particles;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::SetBoundingBoxMin(const Vec3& size)
    {
        m_vBBoxMin = size;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::SetDynamicBoundingBox(bool bSet)
    {
        m_bDynamicBBox = bSet;
    }

    //-----------------------------------------------------------------------------------
    void CParticleEmitterEntity::SetBoundingBoxMax(const Vec3& size)
    {
        m_vBBoxMax = size;
    }





    ENTITY_TYPEID CPlanarPointParticleEmitterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PLANARPOINTEMITTER;

    //-----------------------------------------------------------------------------------
    CPlanarPointParticleEmitterEntity::CPlanarPointParticleEmitterEntity()
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    void CPlanarPointParticleEmitterEntity::InitParticlePosition(CParticleCPU& particle)
    {
        CParticleEmitterEntity::InitParticlePosition(particle);
    }

    //-----------------------------------------------------------------------------------
    void CPlanarPointParticleEmitterEntity::InitParticleVelocityAcceleration(CParticleCPU& particle)
    {
        if (m_bVelocityFromCenter)
        {
            Vec3 dir(particle.m_initialPosition);
            particle.m_velocity = glm::normalize(dir);
        }
        else
        {
            if (m_velocityAngle > 0.0f)
            {
                float radAngle = m_velocityAngle / 360.0f * 2 * PI;
                radAngle *= 0.5f;

                float angleY = GetRandomFloat(-radAngle, radAngle);
                angleY = PI * 0.5f;
                float angleZ = GetRandomFloat(0.0f, 2 * PI);
                //angleZ = 0.0f;

                if (!m_bGlobalSpaceParticles)
                {
                    // Rotate velocity around global Y and Z Axis
                    // because the initial velocity is given as the z axis

                    particle.m_velocity = Vec3(0.0, 0.0, 1.0);
                    Matrix4x4 mtxRotY = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleY, Vec3(0.0f, 1.0f, 0.0f)));
                    Matrix4x4 mtxRotZ = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleZ, Vec3(0.0f, 0.0f, 1.0f)));
                    mtxRotY = mtxRotY * mtxRotZ;
                    mtxRotY = glm::transpose(glm::inverse(mtxRotY));
                    particle.m_velocity = Vec3(Vec4(particle.m_velocity, 0.0f) * mtxRotY);
                    particle.m_velocity = glm::normalize(particle.m_velocity);

                }

                else
                {
                    // Rotate velocity vector around local Y and Z Axis, because the inizial velocity
                    // is given in world space

                    particle.m_velocity = Vec3(0.0, 0.0, 1.0);
                    particle.m_velocity = m_pNode->GetDerivedDirection();
                    Matrix4x4 mtxRotY = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleY, m_pNode->GetDerivedUp()));
                    Matrix4x4 mtxRotZ = glm::transpose(glm::rotate(Matrix4x4(1.0f), angleZ, m_pNode->GetDerivedDirection()));
                    mtxRotY = mtxRotY * mtxRotZ;
                    mtxRotY = glm::transpose(glm::inverse(mtxRotY));
                    particle.m_velocity = Vec3(Vec4(particle.m_velocity, 0.0f) * mtxRotY);
                    particle.m_velocity = glm::normalize(particle.m_velocity);
                }



            }
            else
            {
                particle.m_velocity = Vec3(0.0, 0.0, 1.0);
                particle.m_velocity = m_pNode->GetDerivedDirection();

                if (!m_bGlobalSpaceParticles)
                    particle.m_velocity = Vec3(0.0, 0.0, 1.0);
                else
                    particle.m_velocity = m_pNode->GetDerivedDirection();
            }
        }

        particle.m_velocityBegin = particle.m_velocityEnd = particle.m_velocity;
        particle.m_velocityBegin *= m_velocityBegin;
        particle.m_velocityEnd *= m_velocityEnd;
        particle.m_velocity *= GetRandomFloat(m_velocityBegin, m_velocityEnd);
        Vec3 dir = glm::normalize(particle.m_velocity);

        particle.m_initialAcceleration = dir;
    }





    ENTITY_TYPEID CSphereParticleEmitterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SPHEREEMITTER;

    //-----------------------------------------------------------------------------------
    CSphereParticleEmitterEntity::CSphereParticleEmitterEntity()
        : m_radius(120.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    void CSphereParticleEmitterEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CSphereParticleEmitterEntity::Update);

        CParticleEmitterEntity::Update(dt);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this && gEngine->GetInput()->IsKeyDown('E'))
        {
            // scale the bounding box
            if (gEngine->GetInput()->IsKeyDown(VK_ADD))
            {
                m_radius += 50.0f * dt;
            }
            else if (gEngine->GetInput()->IsKeyDown(VK_SUBTRACT))
            {
                m_radius = std::max(0.0f, m_radius - 50.0f * dt);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CSphereParticleEmitterEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CParticleEmitterEntity::AddToRenderQueue);

        CParticleEmitterEntity::AddToRenderQueue(dt, queue);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
            CBoundingVolume box;
            box.ComputeBoundingVolume(-Vec3(m_radius, m_radius, m_radius), Vec3(m_radius, m_radius, m_radius));
            m_emitterBoxDataCache.Initialize(box, Vec3(0.0f, 0.0f, 1.0f));
            m_emitterBoxDataCache.Transform(m_pNode->GetDerivedRST());
            queue.m_vector3Caches.push_back(&m_emitterBoxDataCache);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSphereParticleEmitterEntity::InitParticlePosition(CParticleCPU& particle)
    {
        float angle = GetRandomFloat(0.0f, 2 * PI);
        float angle2 = GetRandomFloat(0.0f, 2 * PI);
        Vec4 d = Vec4(0.0f, 1.0f, 0.0f, 0.0f);

        Matrix4x4 mtx = glm::transpose(glm::rotate(Matrix4x4(1.0f), angle, Vec3(0.0f, 0.0f, 1.0f)));
        Matrix4x4 mtx2 = glm::transpose(glm::rotate(Matrix4x4(1.0f), angle2, Vec3(0.0f, 1.0f, 0.0f)));
        mtx = mtx * mtx2;
        mtx = glm::transpose(glm::inverse(mtx));
        d = d * mtx;
        d = glm::normalize(d);


        float x = GetRandomFloat(0.0f, 1.0f);
        /*std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_real_distribution<> dist(0.0f, 1.0f);
        x =  dist(eng);*/


        float param = 0.0f;
        if (x < 0.01f)
            param = GetRandomFloat(0.0f, m_radius * 0.25f);
        else if (x < 0.05f)
            param = GetRandomFloat(m_radius * 0.25f, m_radius * 0.5f);
        else if (x < 1.4f)
            param = GetRandomFloat(m_radius * 0.5f, m_radius * 0.75f);
        else
            param = GetRandomFloat(m_radius * 0.75f, m_radius * 1.0f);

        particle.m_initialPosition = d * param;
        particle.m_initialPosition.w = 1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CSphereParticleEmitterEntity::LoadFromTemplate(CScript& script)
    {
        CParticleEmitterEntity::LoadFromTemplate(script);

        if (script.Exists("radius"))
            m_radius = (script["radius"]);
    }

    //-----------------------------------------------------------------------------------
    void CSphereParticleEmitterEntity::SaveAsTemplate(CScript& script)
    {
        CParticleEmitterEntity::SaveAsTemplate(script);

        script["radius"] = m_radius;
    }





    ENTITY_TYPEID CBoxParticleEmitterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_BOXEMITTER;

    //-----------------------------------------------------------------------------------
    CBoxParticleEmitterEntity::CBoxParticleEmitterEntity()
        : m_emitterBoxMax(100.0f, 100.0f, 100.0f)
        , m_emitterBoxMin(-100.0f, -100.0f, -100.0f)
        , m_bAlignScreenX(false)
        , m_bAlignScreenY(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    void CBoxParticleEmitterEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CBoxParticleEmitterEntity::Update);

        if (m_bAlignScreenX || m_bAlignScreenY)
        {
            CCamera* pCamera = gEngine->GetWorldMgr()->GetActiveCamera();

            Vec3 camPos = pCamera->GetAttachedNode()->GetDerivedPosition();
            Vec3 emitterPos = m_pNode->GetDerivedPosition();

            Matrix4x4 mtxTransform = m_pNode->GetDerivedRST();
            Matrix4x4 mtxTransformInverse = glm::inverse(mtxTransform);

            if (m_bAlignScreenX)
            {
                float xWorld = pCamera->GetFrustumHalfWidth(-camPos.z + emitterPos.z);
                Vec3 xLeft = Vec3(camPos.x - xWorld, emitterPos.y, emitterPos.z);
                Vec3 xRight = Vec3(camPos.x + xWorld, emitterPos.y, emitterPos.z);

                Vec4 xLeftLocal = Vec4(xLeft, 1.0f) * mtxTransformInverse;
                Vec4 xRightLocal = Vec4(xRight, 1.0f) * mtxTransformInverse;

                m_emitterBoxMin.x = xLeftLocal.x;
                m_emitterBoxMax.x = xRightLocal.x;
            }

            if (m_bAlignScreenY)
            {
                float yWorld = pCamera->GetFrustumHalfHeight(-camPos.z + emitterPos.z);
                Vec3 yBottom = Vec3(emitterPos.x, camPos.y - yWorld, emitterPos.z);
                Vec3 yTop = Vec3(emitterPos.x, camPos.y + yWorld, emitterPos.z);

                Vec4 yBottomLocal = Vec4(yBottom, 1.0f) * mtxTransformInverse;
                Vec4 yTopLocal = Vec4(yTop, 1.0f) * mtxTransformInverse;

                m_emitterBoxMin.z = yBottomLocal.z;
                m_emitterBoxMax.z = yTopLocal.z;
            }
        }

        CParticleEmitterEntity::Update(dt);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this && gEngine->GetInput()->IsKeyDown('E'))
        {
            // scale the bounding box
            if (gEngine->GetInput()->IsKeyDown(VK_CONTROL))
            {
                if (gEngine->GetInput()->IsKeyDown('X'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_emitterBoxMax.x += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_emitterBoxMax.x -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Y'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_emitterBoxMax.y += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_emitterBoxMax.y -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Z'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_emitterBoxMax.z += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_emitterBoxMax.z -= 16.0f;
                }
            }
            else if (gEngine->GetInput()->IsKeyDown(VK_SHIFT))
            {
                if (gEngine->GetInput()->IsKeyDown('X'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_emitterBoxMin.x += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_emitterBoxMin.x -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Y'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_emitterBoxMin.y += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_emitterBoxMin.y -= 16.0f;
                }
                else if (gEngine->GetInput()->IsKeyDown('Z'))
                {
                    if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                        m_emitterBoxMin.z += 16.0f;
                    else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                        m_emitterBoxMin.z -= 16.0f;
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CBoxParticleEmitterEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CBoxParticleEmitterEntity::AddToRenderQueue);

        CParticleEmitterEntity::AddToRenderQueue(dt, queue);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
            CBoundingVolume box;
            box.ComputeBoundingVolume(m_emitterBoxMin, m_emitterBoxMax);
            m_emitterBoxDataCache.Initialize(box, Vec3(0.0f, 0.0f, 1.0f));
            m_emitterBoxDataCache.Transform(m_pNode->GetDerivedRST());
            queue.m_vector3Caches.push_back(&m_emitterBoxDataCache);
        }
    }

    //-----------------------------------------------------------------------------------
    void CBoxParticleEmitterEntity::InitParticlePosition(CParticleCPU& particle)
    {
        Vec3 particlePos(0.0f, 0.0f, 0.0f);
        GetRandomPoint(particlePos, m_emitterBoxMin, m_emitterBoxMax);
        particle.m_initialPosition = Vec4(particlePos, 1.0f);
    }

    //-----------------------------------------------------------------------------------
    void CBoxParticleEmitterEntity::LoadFromTemplate(CScript& script)
    {
        CParticleEmitterEntity::LoadFromTemplate(script);

        if (script.Exists("emitterBoxMin"))
            m_emitterBoxMin = script["emitterBoxMin"];

        if (script.Exists("emitterBoxMax"))
            m_emitterBoxMax = script["emitterBoxMax"];

        if (script.Exists("alignScreenX"))
            m_bAlignScreenX = script["alignScreenX"];

        if (script.Exists("alignScreenY"))
            m_bAlignScreenY = script["alignScreenY"];
    }

    //-----------------------------------------------------------------------------------
    void CBoxParticleEmitterEntity::SaveAsTemplate(CScript& script)
    {
        CParticleEmitterEntity::SaveAsTemplate(script);

        script["emitterBoxMin"] = m_emitterBoxMin;
        script["emitterBoxMax"] = m_emitterBoxMax;
        script["alignScreenX"] = m_bAlignScreenX;
        script["alignScreenY"] = m_bAlignScreenY;
    }
}