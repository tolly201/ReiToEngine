#ifndef PLATFORM_HAL_WINDOW_WINDOW_ENUMS_H
#define PLATFORM_HAL_WINDOW_WINDOW_ENUMS_H

#include <stdint.h>
#include <stdbool.h>

enum EWINDOW_UPDATE_STATE
{
    WINDOW_UPDATE_STATE_OK             =  0,
    WINDOW_UPDATE_STATE_EXIT           = -1,
    WINDOW_UPDATE_STATE_INVALID_WINDOW = -2,
    WINDOW_UPDATE_STATE_INVALID_BUFFER = -3,
    WINDOW_UPDATE_STATE_INTERNAL_ERROR = -4,
};


enum EWINDOW_WINDOW_FLAGS {
    WF_RESIZABLE          = 0x01,
    WF_FULLSCREEN         = 0x02,
    WF_FULLSCREEN_DESKTOP = 0x04,
    WF_BORDERLESS         = 0x08,
    WF_ALWAYS_ON_TOP      = 0x10,
};

// Opaque pointer
struct mfb_window;
struct mfb_timer;


#ifndef __ANDROID__
#define MFB_RGB(r, g, b)        (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)
#define MFB_ARGB(a, r, g, b)    (((uint32_t) a) << 24) | (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)
#else
    #ifdef HOST_WORDS_BIGENDIAN
    #define MFB_RGB(r, g, b)     (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)
    #define MFB_ARGB(a, r, g, b) (((uint32_t) a) << 24) | (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)
    #else
    #define MFB_ARGB(r, g, b)    (((uint32_t) a) << 24) | (((uint32_t) b) << 16) | (((uint32_t) g) << 8) | ((uint32_t) r)
    #define MFB_RGB(r, g, b)     (((uint32_t) b) << 16) | (((uint32_t) g) << 8) | ((uint32_t) r)
    #endif
#endif

#endif

