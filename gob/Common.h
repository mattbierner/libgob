#pragma once

/**
    Pack an object.

    http://stackoverflow.com/a/3312896/306149
*/
#ifdef _MSC_VER
#define PACKED(...) __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop))
#else
#define PACKED(...) __VA_ARGS__ __attribute__((__packed__))
#endif