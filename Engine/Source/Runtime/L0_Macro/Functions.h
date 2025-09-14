#ifndef MACRO_FUNCTIONS_H
#define MACRO_FUNCTIONS_H

#include "Defines.h"
template <typename T>
constexpr T RTCLAMP(const T& value, const T& min, const T& max) {
    return (value <= min) ? min : (value >= max) ? max : value;
}

template <typename T>
constexpr T RTMIN(const T& x, const T& y) {
    return (x < y) ? x : y;
}

template <typename T>
constexpr T RTMAX(const T& x, const T& y) {
    return (x > y) ? x : y;
}

constexpr u64 RTGibibytes(u64 amount) {
    return amount * 1024ULL * 1024ULL * 1024ULL;
}

constexpr u64 RTMebibytes(u64 amount) {
    return amount * 1024ULL * 1024ULL;
}

constexpr u64 RTKibibytes(u64 amount) {
    return amount * 1024ULL;
}

constexpr u64 RTGigabytes(u64 amount) {
    return amount * 1000ULL * 1000ULL * 1000ULL;
}

constexpr u64 RTMegabytes(u64 amount) {
    return amount * 1000ULL * 1000ULL;
}

constexpr u64 RTKilobytes(u64 amount) {
    return amount * 1000ULL;
}

constexpr u64 RTAligned(u64 operand, u64 granularity) {
    return ((operand + (granularity - 1)) & ~(granularity - 1));
}

constexpr range64 RTAlignedRange(u64 offset, u64 size, u64 granularity) {
    return range64{
        RTAligned(offset, granularity),
        RTAligned(size, granularity)
        };
}

template <typename T>
constexpr T RTGETFLAG(const T& flags, const T& flag) {
    return (flags & flag) == flag;
}

template <typename T>
constexpr T RTSETFLAG(const T& flags, const T& flag, bool enable) {
    return flags = (enable ? (flags | flag) : (flags & ~flag));
}


template <typename T>
constexpr T RT_CLAMP(const T& value, const T& minval, const T& maxval) {
    return (value <= minval) ? minval : ((value >= maxval) ? maxval : value);
}
#endif
