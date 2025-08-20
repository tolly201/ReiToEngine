#ifndef RHI_RENDERER_FRONTEND_H
#define RHI_RENDERER_FRONTEND_H

#include "L0_Macro/Include.h"
#include "RendererBackEnd.h"
namespace ReiToEngine {
class RendererFrontend {
public:
    RendererFrontend() = default;
    virtual ~RendererFrontend() = default;
    virtual b8 Initialize(const char* application_name, struct platform_state* plat_state) = 0;
    virtual b8 Terminate() = 0;
    virtual b8 Tick() = 0;
    virtual b8 Resized(u32 width, u32 height) = 0;
    virtual b8 DrawFrame(render_packet* packet) = 0;
};
}
#endif
