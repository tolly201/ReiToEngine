#ifndef RHI_RENDERER_TYPES_H
#define RHI_RENDERER_TYPES_H

#include "L0_Macro/Include.h"

namespace ReiToEngine {

enum class ERenderBackendType : u8 {
    UNKNOWN = 0x00,
    VULKAN = 0x01,
    OPENGL = 0x02,
    DIRECTX = 0x03,
    METAL = 0x04,
    CPU = 0x05,
    MAX = 0xFF
};


enum class ERenderFrontendType : u8 {
    MESH = 0x00,
    NERF = 0x01,
    GAUSSIAN = 0x02,
    MAX = 0xFF
};

struct render_packet {
    f64 delta_time;
};

struct RenderData {
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
