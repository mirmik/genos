#pragma once

#if defined(IGRIS_COMPAT_STD_TO_STD) && !defined(IGRIS_STD_AS_STD)
#define IGRIS_STD_AS_STD
#endif

#ifdef IGRIS_STD_AS_STD
#define IGRIS_STD_NS std
#else
#define IGRIS_STD_NS igris_std
#endif
