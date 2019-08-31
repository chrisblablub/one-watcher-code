
#include <Graphic/RenderObjects/MeshRenderObject.h>

namespace env
{
    const MESH_RENDER_CATEGORY gMeshRenderCategories[5] = 
    { 
        MESH_RENDER_CATEGORY_DEFERRED, 
        MESH_RENDER_CATEGORY_FORWARD, 
        MESH_RENDER_CATEGORY_CHARACTER, 
        MESH_RENDER_CATEGORY_OUTLINE,
        MESH_RENDER_CATEGORY_TRANSMISSIVE
    };
} // env
