#ifndef PLATFORM_MATH_VERTEX3D_H
#define PLATFORM_MATH_VERTEX3D_H

#include "SVector3.h"
#include "SVector2.h"
#include "SVector4.h"
namespace ReiToEngine
{

//maybe enough to just typedef
//single it if needed
struct Vertex3D {
    SVector3<float> position;
};
}
#endif
