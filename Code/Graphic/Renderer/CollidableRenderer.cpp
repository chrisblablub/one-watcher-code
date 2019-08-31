#include <Graphic/Renderer/CollidableRenderer.h>
#include <Graphic/Core/VertexBuffer.h>

namespace env
{
    CCollidableRenderer::CCollidableRenderer()
        : m_pVB(0)
        , m_bVisible(true) {}

    CCollidableRenderer::~CCollidableRenderer()
    {
        ENV_DELETE(m_pVB);
    }

    void CCollidableRenderer::AddCollidable(CGeometryCollidable* p)
    {
        //m_collidables.push_back(p);
    }

    void CCollidableRenderer::Render(const Matrix4x4& mtxView)
    {
        if (!m_bVisible)
            return;

        // 		unsigned long fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
        // 		struct Vertex
        // 		{
        // 			VECTOR3 vPos;
        // 			D3DCOLOR diffuse;
        // 		};
        // 
        // 		ENV_DELETE(m_pVB);
        // 
        // 		std::list< CGeometryCollidable* > collidables;
        // 		gEngine->GetEntityMgr()->GetCollidables(collidables);
        // 
        // 		unsigned int numLines = 0;
        // 		for(std::list< CGeometryCollidable* >::iterator it = collidables.begin(); it != collidables.end(); ++it)
        // 		{
        // 			CGeometryCollidable& collidable = *(*it);
        // 			std::vector< CPolygon > polys = collidable.GetPolygons();
        // 			for(int i = 0; i < polys.size(); ++i)
        // 			{
        // 				numLines += polys[i].GetNumPoints();
        // 			}
        // 		}
        // 		//IDirect3DDevice9* pDevice = gRenderSystem->GetDevice();
        // 		CRenderDevice* pDevice = gRenderSystem->GetRenderDevice();
        // 		if(numLines > 0)
        // 		{
        // 			CEntity* pSelectedTrigger = gEngine->GetEntityMgr()->GetActiveEntity();
        // 			if(pSelectedTrigger)
        // 			{
        // 				std::vector< CPolygon > polys = pSelectedTrigger->GetPolygons();
        // 				for(int i = 0; i < polys.size(); ++i)
        // 				{
        // 					numLines += polys[i].GetNumPoints();
        // 				}
        // 			}
        // 
        // 			
        // 			m_pVB = pDevice->CreateVertexBuffer(numLines * 2 * sizeof(Vertex), USAGE_TYPE_WRITEONLY, MEMORY_TYPE_MANAGED);
        // 			Vertex* pVertex = static_cast< Vertex* >(m_pVB->Lock(0, 0, 0));
        // 			for(std::list< CGeometryCollidable* >::iterator it = collidables.begin(); it != collidables.end(); ++it)
        // 			{
        // 				CGeometryCollidable& collidable = **it;
        // 				std::vector< CPolygon > quads = collidable.GetTransformedPolygons();
        // 				for(int i = 0; i < quads.size(); ++i)
        // 				{
        // 					for(int j = 0; j < quads[i].GetNumPoints(); ++j)
        // 					{
        // 						pVertex->vPos = quads[i](j); ++pVertex;
        // 						pVertex->vPos = quads[i]((j + 1) % quads[i].GetNumPoints()); ++pVertex;
        // 					}
        // 				}
        // 			}
        // 
        // 			if(pSelectedTrigger)
        // 			{
        // 				std::vector< CPolygon > quads = pSelectedTrigger->GetTransformedPolygons();
        // 				for(int i = 0; i < quads.size(); ++i)
        // 				{
        // 					for(int j = 0; j < quads[i].GetNumPoints(); ++j)
        // 					{
        // 						pVertex->vPos = quads[i](j); 
        // 						pVertex->diffuse = COLOR_XRGB32(255, 0, 0); 
        // 						++pVertex;
        // 						pVertex->vPos = quads[i]((j + 1) % quads[i].GetNumPoints()); 
        // 						pVertex->diffuse = COLOR_XRGB32(255, 0, 0); ++pVertex;
        // 					}
        // 				}
        // 			}
        // 			m_pVB->Unlock();
        // 
        // 			m_pVB->Bind(0, 0, sizeof(Vertex));
        // 			pDevice->SetFVF(fvf);
        // 			pDevice->SetRenderState(RENDERSTATE_TYPE_FILLMODE, D3DFILL_WIREFRAME);
        // 			pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, D3DZB_FALSE);
        // 			pDevice->DrawPrimitive(PRIMITIVE_TYPE_LINELIST, 0, numLines);
        // 		}
        // 
        // 		pDevice->SetRenderState(RENDERSTATE_TYPE_FILLMODE, D3DFILL_WIREFRAME);
        // 
        // 		for(std::list< CGeometryCollidable* >::iterator it = collidables.begin(); it != collidables.end(); ++it)
        // 		{
        // 			CGeometryCollidable& collidable = *(*it);
        // 			std::vector< CCircle > circles = collidable.GetTransformedCircles();
        // 			for(int j = 0; j < circles.size(); ++j)
        // 			{
        // 			}
        // 		}
        // 
        // 
        // 		pDevice->SetRenderState(RENDERSTATE_TYPE_FILLMODE, D3DFILL_SOLID);
    }
} /* env */