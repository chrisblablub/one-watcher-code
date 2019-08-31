#include <Engine/Entity/PathEntity.h>
#include <Engine/Entity/PathWaypointEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Scenario.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CPathEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PATH;

    //-----------------------------------------------------------------------------------
    CPathEntity::CPathEntity()
        : m_levelTime(0.0f)
        , m_bLoop(true)
        , m_bPaused(false)
        , m_pCharacter(nullptr)
        , m_bReverse(false)
        , m_bRenderDebugWireframe(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Path] Start", PATH_EVENT_START);
        RegisterEvent("[Path] Stop", PATH_EVENT_STOP);
        RegisterEvent("[Path] Reset", PATH_EVENT_RESET);
        RegisterEvent("[Path] SetStartWaypoint", PATH_EVENT_SET_START_CORNER);
        RegisterEventParameter(PATH_EVENT_SET_START_CORNER, "WaypointIndex", CBaseParam::PARAMETER_TYPE_INT, "Index", 0, (int)0, (int)100);
    }

    //-----------------------------------------------------------------------------------
    CPathEntity::~CPathEntity()
    {
        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& train = *it;

            m_pNode->DetachChildNode(train.pPathNode->GetName());
            gEngine->GetSceneMgr()->RemoveNode(train.pPathNode->GetName());
        }

        for (int i = 0; i < m_waypoints.size(); ++i)
        {
            m_waypoints[i].first->UnsetPath();
//             if (m_waypoints[i].first)
//                 gEngine->GetEntityMgr()->RemoveEntity(m_waypoints[i].first->GetName());
//             if (m_waypoints[i].second)
//                 gEngine->GetEntityMgr()->RemoveEntity(m_waypoints[i].second->GetName());
        }
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        GetSprite().SetAlphaBlending(true);
        SetText(GetName());
        SetTextPosition(Vec2(0.0, 24.0));
        SetMaterialFilename("Sprites/path.png");
        SetRenderGame(false);

        CPathWaypointEntity* pWP01 = (CPathWaypointEntity*)gEngine->GetEntityMgr()->CreateEntity(CPathWaypointEntity::ENTITY_META_TYPEID);
        CPathWaypointEntity* pWP02 = (CPathWaypointEntity*)gEngine->GetEntityMgr()->CreateEntity(CPathWaypointEntity::ENTITY_META_TYPEID);
        pWP01->GetNode()->SetPosition(Vec3(-6 * 32.0f, 0.0f, 0.0f));
        pWP02->GetNode()->SetPosition(Vec3(6 * 32.0f, 0.0f, 0.0f));
        AddWaypoint(pWP01);
        AddWaypoint(pWP02);
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& train = *it;
            train.startCorner = train.startCornerInitial;

            if (!train.pPathNode)
            {
                assert(train.m_version <= 1);
                train.pPathNode = gEngine->GetSceneMgr()->CreateSceneNode();
                m_pNode->AttachChildNode(train.pPathNode);
                train.pPathNode->AttachChildNode(train.pAttachedNode);
                train.pAttachedNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::OnEntityCloned(CEntity& entity)
    {
        CSpriteEntity::OnEntityCloned(entity);

        CPathEntity& other = static_cast<CPathEntity&>(entity);

        while (GetNumWaypoints() > 0)
        {
            CPathWaypointEntity* pWP = m_waypoints.front().first;

            //RemoveWaypoint(pWP->GetName());

            gEngine->GetEntityMgr()->RemoveEntity(pWP->GetName());
        }

        m_waypoints.clear();

        SetLoop(other.GetLoop());

        for (int i = 0; i < other.GetNumWaypoints(); ++i)
        {
            CPathWaypointEntity* pOtherWp = other.GetWaypoint(i);

            CPathWaypointEntity* pWp = static_cast<CPathWaypointEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PATHWAYPOINT));
            //pWp->SetPath(this);

            AddWaypoint(pWp);

            pWp->GetNode()->SetPosition(pOtherWp->GetNode()->GetPosition());
            pWp->GetNode()->SetOrientation(pOtherWp->GetNode()->GetOrientation());
            pWp->SetAcceleration(pOtherWp->GetAcceleration());
            pWp->SetBreakAcceleartion(pOtherWp->GetBreakAcceleartion());
            pWp->SetPause(pOtherWp->GetPause());
            pWp->SetSpeed(pOtherWp->GetSpeed());
        }
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_levelTime = 0.0f;

        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& train = *it;
            if (!bGameMode)
                train.startCorner = train.startCornerInitial;

            train.currentCorner = !bGameMode ? train.startCornerInitial : train.startCorner;
            train.currentPause = 0.0f;
            train.trackTime = 0.0f;
            train.accumPause = 0.0f;
            train.currentSpeed = 0.0f;
            if (m_waypoints.size() > train.currentCorner)
            {
                if (train.pPathNode)
                {
                    train.pPathNode->SetPosition(m_waypoints[train.currentCorner].first->GetNode()->GetPosition());
                    train.pPathNode->SetOrientation(m_waypoints[train.currentCorner].first->GetNode()->GetOrientation());
                }

                m_waypoints[train.currentCorner].first->OnFired(CEntity::ENTITY_EVENT_FIRE);

                train.currentPause = m_waypoints[train.currentCorner].first->GetPause();
            }
        }

        m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::OnJustUnpicked()
    {
        CSpriteEntity::OnJustUnpicked();

        SetPaused(false);
        SetRenderDebugWireframe(false);
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::OnJustPicked()
    {
        CSpriteEntity::OnJustPicked();

        SetPaused(true);
        SetRenderDebugWireframe(true);
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case PATH_EVENT_START:
            SetPaused(false);
            break;
        case PATH_EVENT_STOP:
            SetPaused(true);
            break;
        case PATH_EVENT_RESET:
            OnGameModeChanged(true);
            break;
        case PATH_EVENT_SET_START_CORNER:
        {
            int waypointIndex = 0;
            if (eventParameter.IsParameterSet("WaypointIndex"))
            {
                eventParameter.GetParameter("WaypointIndex", waypointIndex);

                if (waypointIndex < m_waypoints.size())
                {
                    for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
                    {
                        Train& train = *it;
                        train.startCorner = waypointIndex;
                    }
                }
            }
        }
            break;
        default:
            CSpriteEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::AddWaypoint(CPathWaypointEntity* pCorner)
    {
        for (int i = 0; i < m_waypoints.size(); ++i)
        {
            if (m_waypoints[i].first == pCorner)
                return;
        }
        pairWP p;
        p.first = pCorner;
        p.second = nullptr;
        m_waypoints.push_back(p);

        pCorner->SetPath(this);
        pCorner->SetAutoDelete(true);
        m_pNode->AttachChildNode(pCorner->GetNode());
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::RemoveWaypoint(const std::string& strCorner)
    {
        for (int i = 0; i < m_waypoints.size(); ++i)
        {
            if (m_waypoints[i].first->GetName() == strCorner)
            {
                if (m_waypoints[i].second)
                {
                    //gEngine->GetEntityMgr()->RemoveEntity(m_waypoints[i].second->GetName());
                    m_waypoints[i].second = nullptr;
                }

                for (int j = i; j < m_waypoints.size() - 1; ++j)
                {
                    m_waypoints[j] = m_waypoints[j + 1];
                }
                m_waypoints.pop_back();


                break;
            }
        }

        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
            (*it).currentCorner = 0;
    }

    //-----------------------------------------------------------------------------------
    Train* CPathEntity::AttachNode(CSceneNode* pNode, const std::string& strEntityName)
    {
        if (!pNode)
            return nullptr;

        DetachNode(pNode);

        Train t;
        t.pPathNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pNode->AttachChildNode(t.pPathNode);
        t.pPathNode->AttachChildNode(pNode);
        pNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        t.pAttachedNode = pNode;
        t.strEntityName = strEntityName;
        t.offset = 0.0f;
        t.accumPause = 0.0f;
        t.bReverse = false;
        t.currentCorner = 0;
        t.startCornerInitial = 0;
        t.currentPause = 0.0f;
        t.trackTime = 0.0f;
        t.bFollowPlayer = false;
        t.currentSpeed = 0.0f;

        if (m_waypoints.size() >= t.currentCorner)
        {
            Vec3 vWaypointPos = m_waypoints[t.currentCorner].first->GetNode()->GetPosition();
            t.pPathNode->SetPosition(vWaypointPos);
        }

        m_trains.push_back(t);

        SetDeletable(m_trains.empty());

        return GetTrain(strEntityName);
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::DetachNode(CSceneNode* pNode)
    {
        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& t = *it;
            if (t.pAttachedNode == pNode)
            {
                t.pPathNode->DetachChildNode(pNode->GetName());
                m_pNode->DetachChildNode(t.pPathNode->GetName());
                gEngine->GetSceneMgr()->RemoveNode(t.pPathNode->GetName());
                m_trains.erase(it);
                gEngine->GetSceneMgr()->GetRootNode()->AttachChildNode(pNode);
                break;
            }
        }

        SetDeletable(m_trains.empty());
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::DetachNode(const std::string& strEntityName)
    {
        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& t = *it;
            if (t.strEntityName == strEntityName)
            {
                t.pPathNode->DetachChildNode(t.pAttachedNode->GetName());
                m_pNode->DetachChildNode(t.pPathNode->GetName());
                gEngine->GetSceneMgr()->RemoveNode(t.pPathNode->GetName());
                gEngine->GetSceneMgr()->GetRootNode()->AttachChildNode(t.pAttachedNode);
                m_trains.erase(it);
                break;
            }
        }

        SetDeletable(m_trains.empty());
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CPathEntity::Update);

        CSpriteEntity::Update(dt);

#ifndef ENV_FINAL
        if (gEngine->GetEditor())
            m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();

        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            if (!gEngine->GetEntityMgr()->GetEntity(it->strEntityName))
            {
                m_trains.erase(it);
                if (m_trains.empty())
                    break;
                else
                    it = m_trains.begin();
            }
        }
#endif // ENV_FINAL

        bool bForcePause = false;

        if (gEngine->GetEntityMgr()->GetActiveEntity())
        {
            bForcePause = true;
        }
        else
        {
            CEntity* pEntity = gEngine->GetEntityMgr()->GetActiveEntity();
            bool bWaypointSelected = false;
            for (int i = 0; i < m_waypoints.size(); ++i)
            {
                if (m_waypoints[i].first == pEntity)
                {
                    bWaypointSelected = true;
                    break;
                }
            }
            if (!bWaypointSelected)
                bForcePause = false;
            else
                bForcePause = true;
        }

        if (m_waypoints.size() <= 1 || m_bPaused || bForcePause)
            return;



        m_levelTime += dt;

        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& train = *it;

            if (train.accumPause < train.currentPause || !train.pPathNode)
            {
                train.accumPause += dt;
                train.trackTime = 0.0;
                continue;
            }

            train.trackTime += dt;

            if (train.offset >= m_levelTime)
                continue;

            if (!train.bFollowPlayer)
            {
                CPathWaypointEntity* pNextCorner = nullptr;
                if (train.bReverse)
                {
                    if (train.currentCorner == 0)
                        pNextCorner = m_waypoints[m_waypoints.size() - 1].first;
                    else
                        pNextCorner = m_waypoints[(train.currentCorner - 1) % m_waypoints.size()].first;
                }
                else
                {
                    int nextWP = (train.currentCorner + 1) % m_waypoints.size();
                    pNextCorner = m_waypoints[nextWP].first;
                }


                int currentCorner = train.currentCorner;

                // BUGFIX: setting an invalid waypoint index leads to a crash when loading a level
                if (currentCorner >= m_waypoints.size())
                {
                    currentCorner = 0;
                }


                CPathWaypointEntity* pCurrentCorner = m_waypoints[currentCorner].first;
                Vec3 vPath = pNextCorner->GetNode()->GetPosition() - train.pPathNode->GetPosition();
                Vec3 vDir;
                vDir = glm::normalize(vPath);

                float distance = glm::length(vPath);
                float distanceBrake = 0.0f;


                if (Equals(pCurrentCorner->GetAcceleration(), 0.0f))
                {
                    train.currentSpeed = pCurrentCorner->GetSpeed();
                }
                else
                {
                    distanceBrake = 0.5f * pCurrentCorner->GetSpeed() * pCurrentCorner->GetSpeed() / pCurrentCorner->GetBreakAcceleartion();

                    if (train.currentSpeed < pCurrentCorner->GetSpeed() && distance >= distanceBrake)
                        train.currentSpeed += std::max(pCurrentCorner->GetAcceleration() * dt, 0.0f);
                }


                //float speed = max(pCurrentCorner->GetSpeed() + pCurrentCorner->GetAcceleration() * dt, 0.0f);
                Vec3 vMove = vDir * train.currentSpeed * dt;

                if (glm::length(vPath) <= glm::length(vMove))
                {
                    // Store whether the next movement part is going in reversed direction or not
                    if (!m_bLoop && (train.currentCorner + 1) == (m_waypoints.size() - 1) && !train.bReverse)
                    {
                        if (m_bReverse)
                        {
                            m_waypoints[train.currentCorner + 1].first->OnFired(CEntity::ENTITY_EVENT_FIRE);

                            train.currentCorner = (train.currentCorner + 1) % m_waypoints.size();

                            train.bReverse = true;
                        }
                        else
                        {
                            m_waypoints[train.currentCorner + 1].first->OnFired(CEntity::ENTITY_EVENT_FIRE);

                            train.currentCorner = 0;

                            train.bReverse = false;
                        }

                    }
                    else if (train.bReverse && train.currentCorner == 1)
                    {
                        train.bReverse = false;
                        train.currentCorner = 0;

                        m_waypoints[train.currentCorner].first->OnFired(CEntity::ENTITY_EVENT_FIRE);
                    }
                    else
                    {
                        if (!train.bReverse)
                            train.currentCorner = (train.currentCorner + 1) % m_waypoints.size();
                        else
                        {
                            if (train.currentCorner == 0)
                            {
                                train.bReverse = false;
                                train.currentCorner = (train.currentCorner + 1) % m_waypoints.size();
                            }
                            else
                                --train.currentCorner;
                        }

                        m_waypoints[train.currentCorner].first->OnFired(CEntity::ENTITY_EVENT_FIRE);
                    }



                    train.pPathNode->SetOrientation(m_waypoints[train.currentCorner].first->GetNode()->GetOrientation());
                    train.pPathNode->SetPosition(m_waypoints[train.currentCorner].first->GetNode()->GetPosition());
                    train.currentPause = m_waypoints[train.currentCorner].first->GetPause();
                    train.accumPause = 0.0;
                    train.trackTime = 0.0;
                    train.trackTime = 0.0f;
                    train.currentSpeed = 0.0f;
                }
                else
                {
                    if (distance < distanceBrake)
                    {
                        train.currentSpeed = std::max(train.currentSpeed - pCurrentCorner->GetBreakAcceleartion() * dt, 30.0f);

                        vMove = vDir * train.currentSpeed * dt;
                    }
                    train.pPathNode->Translate(vMove);
                    train.currentPause = train.accumPause = 0.0;


                    Vec3 totalPath(pNextCorner->GetNode()->GetPosition() - pCurrentCorner->GetNode()->GetPosition());
                    float alpha = glm::length(vPath) / glm::length(totalPath);

                    alpha = std::max(0.0f, std::min(alpha, 1.0f));
                    //                     float roll = alpha * pCurrentCorner->GetNode()->GetOrientation().GetRoll() +
                    //                         (1.0f - alpha) * pNextCorner->GetNode()->GetOrientation().GetRoll();

                    CQuaternion startOrientation(pCurrentCorner->GetNode()->GetOrientation());
                    startOrientation.Slerp(pNextCorner->GetNode()->GetOrientation(), alpha);

                    //CQuaternion quat = train.pPathNode->GetOrientation();
                    //quat.SetRoll(roll);
                    train.pPathNode->SetOrientation(startOrientation);
                }
            }
            else // Train follows the player
            {
                if (train.bFollowPlayer && m_pCharacter && m_waypoints.size() == 2)
                {
                    CPathWaypointEntity* pCurrentCorner = m_waypoints[train.currentCorner].first;
                    CPathWaypointEntity* pNextCorner = m_waypoints[(train.currentCorner + 1) % 2].first;

                    Vec3 nextPos = pNextCorner->GetNode()->GetDerivedPosition();
                    Vec3 currentPos = pCurrentCorner->GetNode()->GetDerivedPosition();
                    Vec3 pathDir(nextPos - currentPos);
                    pathDir = glm::normalize(pathDir);
                    Vec3 charPos = m_pCharacter->GetNode()->GetDerivedPosition();

                    float param = ComputePointOnLineProjection(charPos, currentPos, nextPos - currentPos);
                    if (param < 0.0f)
                        param = 0.0f;
                    if (param > 1.0f)
                        param = 1.0f;


                    Vec3 targetPos = pCurrentCorner->GetNode()->GetPosition() + param * (pNextCorner->GetNode()->GetPosition() - pCurrentCorner->GetNode()->GetPosition());
                    Vec3 dir(targetPos - pCurrentCorner->GetNode()->GetPosition());

                    if (glm::length(dir) >= 0.0001f)
                        dir = glm::normalize(dir);

                    if (Equals(pCurrentCorner->GetAcceleration(), 0.0f))
                    {
                        train.pPathNode->SetPosition(targetPos);
                    }
                    else
                    {
                        Vec3 curentDir(targetPos - train.pPathNode->GetPosition());
                        float distToTarget = glm::length(curentDir);
                        curentDir = glm::normalize(curentDir);

                        float currentDist = glm::length(train.pPathNode->GetPosition() - pNextCorner->GetNode()->GetPosition());


                        Vec3 dirToNextCorner(pNextCorner->GetNode()->GetPosition() - train.pPathNode->GetPosition());

                        if (currentDist < 30 || glm::dot(dirToNextCorner, pathDir) < 0)
                        {
                            train.currentCorner = (train.currentCorner + 1) % 2;
                            train.currentSpeed *= 0.5f;
                        }
                        else
                        {
                            if (glm::dot(curentDir, pathDir) >= 0.0)
                            {
                                if (train.currentSpeed < pCurrentCorner->GetSpeed())
                                    train.currentSpeed += std::max(pCurrentCorner->GetAcceleration() * dt * std::max(distToTarget / 150.0f, 1.0f), 0.0f);
                            }
                            else
                            {
                                if (train.currentSpeed >= 0.0f)
                                {
                                    train.currentSpeed -= std::max(pCurrentCorner->GetAcceleration() * dt * std::max(distToTarget / 150.0f, 1.0f), 0.0f);
                                }
                                else
                                {

                                    train.currentCorner = (train.currentCorner + 1) % 2;
                                }

                            }
                        }
                    }


                    //train.currentSpeed += max(pCurrentCorner->GetAcceleration() * dt, 0.0f);
                    train.pPathNode->Translate(dir * dt * train.currentSpeed);
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (m_bRenderDebugWireframe)
        {
            m_debugWireframe.Reset();
            m_debugWireframe.SetPrimitiveType(PRIMITIVE_TYPE_LINELIST);
            m_debugWireframe.Reserve(m_waypoints.size() * 4);

            for (auto i = 0; i < m_waypoints.size() - 1; ++i)
            {
                CPathWaypointEntity* pWP0 = m_waypoints[i].first;
                CPathWaypointEntity* pWP1 = m_waypoints[i + 1].first;

                m_debugWireframe.Add(PolygonVertex(pWP0->GetNode()->GetDerivedPosition(), Vec3(1.0f, 0.0f, 0.0f)));
                m_debugWireframe.Add(PolygonVertex(pWP1->GetNode()->GetDerivedPosition(), Vec3(1.0f, 0.0f, 0.0f)));

                m_debugWireframe.Add(PolygonVertex(m_pNode->GetDerivedPosition(), Vec3(0.0f, 1.0f, 0.0f)));
                m_debugWireframe.Add(PolygonVertex(pWP0->GetNode()->GetDerivedPosition(), Vec3(0.0f, 1.0f, 0.0f)));
            }

            queue.m_vector3Caches.push_back(&m_debugWireframe);
        }
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::SetLoop(bool bLoop)
    {
        if (m_bLoop == bLoop)
            return;

        m_bLoop = bLoop;
    }

    //-----------------------------------------------------------------------------------
    bool CPathEntity::GetLoop()
    {
        return m_bLoop;
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::SetReverse(bool bReverse)
    {
        m_bReverse = bReverse;
    }

    //-----------------------------------------------------------------------------------
    bool CPathEntity::GetReverse()
    {
        return m_bReverse;
    }

    //-----------------------------------------------------------------------------------
    void CPathEntity::SetPaused(bool bPaused)
    {
        m_bPaused = bPaused;
    }

    //-----------------------------------------------------------------------------------
    bool CPathEntity::GetPaused()
    {
        return m_bPaused;
    }

    //-----------------------------------------------------------------------------------
    const std::list< Train >& CPathEntity::GetTrains()
    {
        return m_trains;
    }

    //-----------------------------------------------------------------------------------
    Train* CPathEntity::GetTrain(const std::string& strEntityName)
    {
        for (std::list< Train >::iterator it = m_trains.begin(); it != m_trains.end(); ++it)
        {
            Train& t = *it;
            if (t.strEntityName == strEntityName)
                return &t;
        }

        return nullptr;
    }

    //-----------------------------------------------------------------------------------
    int CPathEntity::GetNumWaypoints()
    {
        return m_waypoints.size();
    }

    //-----------------------------------------------------------------------------------
    CPathWaypointEntity* CPathEntity::GetWaypoint(unsigned int index)
    {
        if (index >= m_waypoints.size())
            return nullptr;

        return m_waypoints[index].first;
    }

} /* End namespace */