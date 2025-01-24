#pragma once
#include <stdint.h>

// Had linker issues that sais that the animations where defined multiple times.
// Read on stackoverflow that defining data in seperate .c file can help the linker to understand the including
// the header elsewhere will redefine a referense instead of defining it again.
extern const uint8_t heart_rate_animation[][128];
extern const uint8_t leaf_animation[][128];

extern const uint8_t temperature_animation[][128];
extern const uint8_t freezing_animation[][128];
extern const uint8_t alarm_animation[][128];

