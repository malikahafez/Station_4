// utils.c
#include "utils.h"

// Helper function to map a value from one range to another (replaces Arduino map())
// Note: Uses 'long' internally to prevent overflow during multiplication
long map_value(long x, long in_min, long in_max, long out_min, long out_max) {
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;

    return ((x - in_min) * (out_max - out_min) + (in_max - in_min) / 2) / (in_max - in_min) + out_min;
}
// --- Round-mapped helper ---
long map_value_rounded(long x, long in_min, long in_max, long out_min, long out_max) {
    // Constrain input
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;

    // Round instead of truncate
    return ((x - in_min) * (out_max - out_min) + (in_max - in_min) / 2) / (in_max - in_min) + out_min;
} 



// Helper function to constrain a value within a range (replaces Arduino constrain())
// Constrain a value between min_val and max_val
int constrain_value(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// Optional version for long integers
long constrain_value_long(long value, long min_val, long max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}
