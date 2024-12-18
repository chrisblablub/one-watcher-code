
#include <Graphic/RenderObjects/MeshRenderObject.h>

namespace env
{
    const MESH_RENDER_CATEGORY gMeshRenderCategories[6] = 
    { 
        MESH_RENDER_CATEGORY_DEFERRED, 
        MESH_RENDER_CATEGORY_FORWARD, 
        MESH_RENDER_CATEGORY_CHARACTER, 
        MESH_RENDER_CATEGORY_OUTLINE,
        MESH_RENDER_CATEGORY_TRANSMISSIVE,
        MESH_RENDER_CATEGORY_DEFERRED_LAYER0
    };
} // env
