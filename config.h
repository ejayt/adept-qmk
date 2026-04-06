#pragma once

// -----------------------------------------------------------------------------
// Tap Dance
// -----------------------------------------------------------------------------

// How long QMK waits to determine tap vs hold / multi-tap
#define TAPPING_TERM 200

// Helps hold behavior feel more intentional on tap dance / mod-tap style usage
#define PERMISSIVE_HOLD

// -----------------------------------------------------------------------------
// Combos
// -----------------------------------------------------------------------------

// Time window for combo detection
#define COMBO_TERM 35

// -----------------------------------------------------------------------------
// Pointing Device
// -----------------------------------------------------------------------------

// Invert these only if movement/scroll direction feels wrong
// #define POINTING_DEVICE_INVERT_X
// #define POINTING_DEVICE_INVERT_Y

// You can swap axes if needed, but leave off unless required
// #define POINTING_DEVICE_ROTATION_90
// #define POINTING_DEVICE_ROTATION_180
// #define POINTING_DEVICE_ROTATION_270

// -----------------------------------------------------------------------------
// Mouse / Trackball behavior
// -----------------------------------------------------------------------------

// Default CPI can also be controlled in code, but this gives a sane base
#define POINTING_DEVICE_DEFAULT_CPI 800

// -----------------------------------------------------------------------------
// Safety / cleanup
// -----------------------------------------------------------------------------

// Ignore accidental tiny mod differences during combo sequences
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION