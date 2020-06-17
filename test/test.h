#ifdef __clang__

#pragma clang diagnostic push

#if !defined(__has_warning) || __has_warning("-Wimplicit-int-float-conversion")
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#endif

#endif

#include "doctest.h"

#ifdef __clang__

#pragma clang diagnostic pop

#endif
