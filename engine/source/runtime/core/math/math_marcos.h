#pragma once

/**
 * macro definition
 */

// min, max value
#define VKERNEL_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define VKERNEL_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define VKERNEL_PIN(a, min_value, max_value) VKERNEL_MIN(max_value, VKERNEL_MAX(a, min_value))

// valid index
#define VKERNEL_VALID_INDEX(idx, range) (((idx) >= 0) && ((idx) < (range)))
#define VKERNEL_PIN_INDEX(idx, range) VKERNEL_PIN(idx, 0, (range)-1)

// if>0 returns 1, and <0 returns -1
#define VKERNEL_SIGN(x) ((((x) > 0.0f) ? 1.0f : 0.0f) + (((x) < 0.0f) ? -1.0f : 0.0f))
