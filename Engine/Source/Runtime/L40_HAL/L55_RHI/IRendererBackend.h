#ifndef RHI_IRENDERER_BACKEND_H
#define RHI_IRENDERER_BACKEND_H

#include "L0_Macro/Include.h"
#include "L20_Platform/Include.h"
#include "RendererTypes.h"
namespace ReiToEngine {

class IRendererBackend {
    PlatformState* platform_state;
    public:
    IRendererBackend() = default;
    virtual ~IRendererBackend() = default;

    virtual b8 Initialize(ERenderBackendType renderer_type, const char* application_name, PlatformState* plat_state) = 0;
    virtual b8 Terminate() = 0;
    virtual b8 Tick() = 0;
    virtual b8 Resized(u32 width, u32 height) = 0;

    virtual b8 BeginFrame(f64 delta_time) = 0;
    virtual b8 EndFrame(f64 delta_time) = 0;

    virtual b8 CreateSurface(RT_Platform_State& platform_state, SurfaceDesc& desc) = 0;
};
}
#endif
