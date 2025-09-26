#ifndef HAL_FILE_DEFINES_H
#define HAL_FILE_DEFINES_H
#include "L0_Macro/Include.h"
namespace ReiToEngine
{
enum class EFileWrite : u8
{
	None			  = 0x00,
	NoFail            = 0x01,
	NoReplaceExisting = 0x02,
	EvenIfReadOnly    = 0x04,
	Append			  = 0x08,
	AllowRead	      = 0x10,
	Silent			  = 0x20,
    Max               = 0xFF
};

enum class EFileRead : u8
{
	None				= 0x00,
	NoFail              = 0x01,
	Silent				= 0x02,
	AllowWrite			= 0x04,
    Max                 = 0xFF
};

enum class ECopyResult : u8
{
	OK						= 0x00,
	Fail					= 0x01,
	Canceled				= 0x02,
};
enum class EFileOpenFlags : u8
{
	IO_READ			= 0x01,					// Open for reading
	IO_WRITE		= 0x02,					// Open for writing
	IO_APPEND		= 0x40,					// When writing, keep the existing data, set the filepointer to the end of the existing data
};

enum class EFileCommon : u32
{
	FILE_MAX_PATH = 256
};

// Same-type bitwise operators for flag enums
// EFileWrite
RT_FORCEINLINE constexpr EFileWrite operator|(EFileWrite lhs, EFileWrite rhs)
{
    return static_cast<EFileWrite>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
}
RT_FORCEINLINE constexpr EFileWrite operator&(EFileWrite lhs, EFileWrite rhs)
{
    return static_cast<EFileWrite>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
}
RT_FORCEINLINE constexpr EFileWrite& operator|=(EFileWrite& lhs, EFileWrite rhs)
{
    lhs = (lhs | rhs);
    return lhs;
}
RT_FORCEINLINE constexpr EFileWrite& operator&=(EFileWrite& lhs, EFileWrite rhs)
{
    lhs = (lhs & rhs);
    return lhs;
}

// EFileRead
RT_FORCEINLINE constexpr EFileRead operator|(EFileRead lhs, EFileRead rhs)
{
    return static_cast<EFileRead>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
}
RT_FORCEINLINE constexpr EFileRead operator&(EFileRead lhs, EFileRead rhs)
{
    return static_cast<EFileRead>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
}
RT_FORCEINLINE constexpr EFileRead& operator|=(EFileRead& lhs, EFileRead rhs)
{
    lhs = (lhs | rhs);
    return lhs;
}
RT_FORCEINLINE constexpr EFileRead& operator&=(EFileRead& lhs, EFileRead rhs)
{
    lhs = (lhs & rhs);
    return lhs;
}

// EFileOpenFlags
RT_FORCEINLINE constexpr EFileOpenFlags operator|(EFileOpenFlags lhs, EFileOpenFlags rhs)
{
    return static_cast<EFileOpenFlags>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
}
RT_FORCEINLINE constexpr EFileOpenFlags operator&(EFileOpenFlags lhs, EFileOpenFlags rhs)
{
    return static_cast<EFileOpenFlags>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
}
RT_FORCEINLINE constexpr EFileOpenFlags& operator|=(EFileOpenFlags& lhs, EFileOpenFlags rhs)
{
    lhs = (lhs | rhs);
    return lhs;
}
RT_FORCEINLINE constexpr EFileOpenFlags& operator&=(EFileOpenFlags& lhs, EFileOpenFlags rhs)
{
    lhs = (lhs & rhs);
    return lhs;
}

struct SFileStatData
{
	EFileWrite Write;
	EFileRead Read;
	ECopyResult CopyResult;
	EFileOpenFlags OpenState;
};

}
#endif
