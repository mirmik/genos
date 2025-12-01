#ifndef IGRIS_TIME_ARDUTIME_H
#define IGRIS_TIME_ARDUTIME_H

static inline int64_t millis() { return igris::millis(); }
static inline int64_t micros() { return igris::micros(); }
static inline int64_t nanos() { return igris::nanos(); }

#endif
