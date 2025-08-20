#ifndef RHI_RENDERER_TYPES_H
#define RHI_RENDERER_TYPES_H

#include "L0_Macro/Include.h"

namespace ReiToEngine {

enum class ERenderBackEndType : u8 {
    RENDER_TYPE_UNKNOWN = 0x00,
    RENDER_TYPE_VULKAN = 0x01,
    RENDER_TYPE_OPENGL = 0x02,
    RENDER_TYPE_DIRECTX = 0x03,
    RENDER_TYPE_METAL = 0x04,
    RENDER_TYPE_CPU = 0x05,
    RENDER_TYPE_MAX = 0xFF
};

struct render_packet {
    f64 delta_time;
};
enum class ESceneDataType : u8 {
    MESH = 0x00,
    NERF = 0x01,
    GAUSSIAN = 0x02,
};

struct SceneData {
    ESceneDataType type;
};

struct PlatformState {
    void* data;
};
}
#endif
