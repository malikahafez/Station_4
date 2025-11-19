// utils.c
#include "utils.h"

// Helper function to map a value from one range to another (replaces Arduino map())
// Note: Uses 'long' internally to prevent overflow during multiplication
long map_value(long x, long in_min, long in_max, long out_min, long out_max) {
  // Constrain the input value to the input range boundaries
  if (x < in_min) x = in_min;
  if (x > in_max) x = in_max;
  
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Helper function to constrain a value within a range (replaces Arduino constrain())
int constrain_value(int amt, int low, int high) {
    if (amt < low) return low;
    if (amt > high) return high;
    return amt;
}