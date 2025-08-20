#ifndef RHI_RENDERER_BACKEND_H
#define RHI_RENDERER_BACKEND_H

#include "L0_Macro/Include.h"
#include "RendererTypes.h"
namespace ReiToEngine {

class RendererBackEnd {
    PlatformState* platform_state;
    public:
    RendererBackEnd() = default;
    virtual ~RendererBackEnd() = default;

    virtual b8 Initialize(ERenderBackEndType renderer_type, const char* application_name, PlatformState* plat_state) = 0;
    virtual b8 Terminate() = 0;
    virtual b8 Tick() = 0;
    virtual b8 Resized(u32 width, u32 height) = 0;

    virtual b8 BeginFrame(f64 delta_time) = 0;
    virtual b8 EndFrame(f64 delta_time) = 0;
};
}
#endif
