#include QMK_KEYBOARD_H

// -----------------------------------------------------------------------------
// Layers
// -----------------------------------------------------------------------------

enum layers {
    _BASE = 0,
    _VOL,
    _MAC,
};

// -----------------------------------------------------------------------------
// Custom keycodes
// -----------------------------------------------------------------------------

enum custom_keycodes {
    COPYTXT = SAFE_RANGE,
    OFF,
    DF_WIN,
    DF_MAC,
};

// -----------------------------------------------------------------------------
// Tap dance IDs
// -----------------------------------------------------------------------------

enum tapdance {
    MCLK = 0,
    RCLK,
    LTAB,
    RTAB,
    VOLU,
};

// -----------------------------------------------------------------------------
// Combo IDs
// -----------------------------------------------------------------------------

enum combo_events {
    RESET_COMBO = 0,
    OS_TOGGLE_COMBO,
    COMBO_LENGTH,
};

uint16_t COMBO_LEN = COMBO_LENGTH;

// -----------------------------------------------------------------------------
// Ball mode state
// -----------------------------------------------------------------------------

typedef enum {
    BALL_MODE_POINTER = 0,
    BALL_MODE_SCROLL,
    BALL_MODE_ZOOM,
    BALL_MODE_VOLUME,
} ball_mode_t;

static ball_mode_t ball_mode = BALL_MODE_POINTER;
static bool sticky_scroll = false;

static int16_t scroll_accum_x = 0;
static int16_t scroll_accum_y = 0;
static int16_t arrow_accum_x  = 0;
static int16_t volume_accum_y = 0;

#define DEFAULT_CPI      400
#define ZOOM_CPI         50 
#define SCROLL_DIVISOR   2
#define ARROW_THRESHOLD  35
#define VOLUME_THRESHOLD 35

// -----------------------------------------------------------------------------
// Tap dance state tracking
// -----------------------------------------------------------------------------

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
} td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

static td_tap_t mclk_tap_state = { .is_press_action = true, .state = TD_NONE };
static td_tap_t rclk_tap_state = { .is_press_action = true, .state = TD_NONE };
static td_tap_t volu_tap_state = { .is_press_action = true, .state = TD_NONE };

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

static bool is_mac_mode(void) {
    return get_highest_layer(default_layer_state) == _MAC;
}

static td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) {
            return TD_SINGLE_TAP;
        } else {
            return TD_SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        if (state->interrupted) {
            return TD_DOUBLE_SINGLE_TAP;
        } else if (state->pressed) {
            return TD_DOUBLE_HOLD;
        } else {
            return TD_DOUBLE_TAP;
        }
    } else if (state->count == 3) {
        if (state->interrupted || !state->pressed) {
            return TD_TRIPLE_TAP;
        } else {
            return TD_TRIPLE_HOLD;
        }
    }

    return TD_UNKNOWN;
}

static void reset_ball_accumulators(void) {
    scroll_accum_x = 0;
    scroll_accum_y = 0;
    arrow_accum_x  = 0;
    volume_accum_y = 0;
}

static void send_close_tab(void) {
    if (is_mac_mode()) {
        tap_code16(G(KC_W));
    } else {
        tap_code16(C(KC_W));
    }
}

static void send_reopen_tab(void) {
    if (is_mac_mode()) {
        tap_code16(G(S(KC_T)));
    } else {
        tap_code16(C(S(KC_T)));
    }
}

static void register_zoom_mod(void) {
    if (is_mac_mode()) {
        register_code(KC_LGUI);
    } else {
        register_code(KC_LCTL);
    }
}

static void unregister_zoom_mod(void) {
    if (is_mac_mode()) {
        unregister_code(KC_LGUI);
    } else {
        unregister_code(KC_LCTL);
    }
}

static void set_ball_mode(ball_mode_t new_mode) {
    if (ball_mode == BALL_MODE_ZOOM && new_mode != BALL_MODE_ZOOM) {
        unregister_zoom_mod();
        pointing_device_set_cpi(DEFAULT_CPI);
    }

    if (ball_mode != BALL_MODE_ZOOM && new_mode == BALL_MODE_ZOOM) {
        register_zoom_mod();
        pointing_device_set_cpi(ZOOM_CPI);
    }

    ball_mode = new_mode;
    reset_ball_accumulators();
}

static void clear_pointer_motion(report_mouse_t *mouse_report) {
    mouse_report->x = 0;
    mouse_report->y = 0;
}

static void clear_all_mouse_output(report_mouse_t *mouse_report) {
    mouse_report->x = 0;
    mouse_report->y = 0;
    mouse_report->h = 0;
    mouse_report->v = 0;
}

static void enable_sticky_scroll(void) {
    sticky_scroll = true;
    reset_ball_accumulators();
}

static void disable_sticky_scroll(void) {
    sticky_scroll = false;
    reset_ball_accumulators();
}

// -----------------------------------------------------------------------------
// Pointing device behavior
// -----------------------------------------------------------------------------

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (sticky_scroll || ball_mode == BALL_MODE_SCROLL || ball_mode == BALL_MODE_ZOOM) {
        scroll_accum_x += mouse_report.x;
        scroll_accum_y += mouse_report.y;

        mouse_report.h = scroll_accum_x / SCROLL_DIVISOR;
        mouse_report.v = -(scroll_accum_y / SCROLL_DIVISOR);

        scroll_accum_x %= SCROLL_DIVISOR;
        scroll_accum_y %= SCROLL_DIVISOR;

        clear_pointer_motion(&mouse_report);
        return mouse_report;
    }

    switch (ball_mode) {
        case BALL_MODE_POINTER:
            return mouse_report;

        case BALL_MODE_VOLUME:
            volume_accum_y += mouse_report.y;

            if (volume_accum_y >= VOLUME_THRESHOLD) {
                tap_code(KC_VOLD);
                volume_accum_y = 0;
            } else if (volume_accum_y <= -VOLUME_THRESHOLD) {
                tap_code(KC_VOLU);
                volume_accum_y = 0;
            }

            clear_all_mouse_output(&mouse_report);
            return mouse_report;

        case BALL_MODE_SCROLL:
        case BALL_MODE_ZOOM:
            // already handled above
            return mouse_report;
    }

    return mouse_report;
}

// -----------------------------------------------------------------------------
// Tap dance callbacks
// -----------------------------------------------------------------------------
void mclk_finished(tap_dance_state_t *state, void *user_data) {
    mclk_tap_state.state = cur_dance(state);

    switch (mclk_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(MS_BTN3);
            break;
        case TD_SINGLE_HOLD:
            set_ball_mode(BALL_MODE_SCROLL);
            break;
        case TD_DOUBLE_TAP:
            send_reopen_tab();
            break;
        default:
            break;
    }
}

void mclk_reset(tap_dance_state_t *state, void *user_data) {
    if (mclk_tap_state.state == TD_SINGLE_HOLD) {
        set_ball_mode(BALL_MODE_POINTER);
    }
    mclk_tap_state.state = TD_NONE;
}

void rclk_finished(tap_dance_state_t *state, void *user_data) {
    rclk_tap_state.state = cur_dance(state);

    switch (rclk_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(MS_BTN2);
            break;
        case TD_SINGLE_HOLD:
            set_ball_mode(BALL_MODE_ZOOM);
            break;
        case TD_DOUBLE_TAP:
            send_close_tab();
            break;
        case TD_TRIPLE_TAP:
            enable_sticky_scroll();
            break;
        default:
            break;
    }
}

void rclk_reset(tap_dance_state_t *state, void *user_data) {
    if (rclk_tap_state.state == TD_SINGLE_HOLD) {
        set_ball_mode(BALL_MODE_POINTER);
    }
    rclk_tap_state.state = TD_NONE;
}

void volu_finished(tap_dance_state_t *state, void *user_data) {
    volu_tap_state.state = cur_dance(state);

    switch (volu_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_ESC);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_VOL);
            set_ball_mode(BALL_MODE_VOLUME);
            break;
        default:
            break;
    }
}

void volu_reset(tap_dance_state_t *state, void *user_data) {
    if (volu_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_VOL);
        set_ball_mode(BALL_MODE_POINTER);
    }
    volu_tap_state.state = TD_NONE;
}

// -----------------------------------------------------------------------------
// Tap dance registration
// -----------------------------------------------------------------------------

tap_dance_action_t tap_dance_actions[] = {
    [MCLK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mclk_finished, mclk_reset),
    [RCLK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rclk_finished, rclk_reset),
    [VOLU] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, volu_finished, volu_reset),
};

// -----------------------------------------------------------------------------
// Combos
// -----------------------------------------------------------------------------

const uint16_t PROGMEM reset_combo[] = {DPI_CONFIG, TD(MCLK), TD(RCLK), COMBO_END};
const uint16_t PROGMEM os_toggle_combo[] = {DPI_CONFIG, TD(MCLK), COMBO_END};

combo_t key_combos[] = {
    [RESET_COMBO] = COMBO_ACTION(reset_combo),
    [OS_TOGGLE_COMBO] = COMBO_ACTION(os_toggle_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) {
        return;
    }

    switch (combo_index) {
        case RESET_COMBO:
            reset_keyboard();
            break;
        case OS_TOGGLE_COMBO:
            if (is_mac_mode()) {
                set_single_persistent_default_layer(_BASE);
            } else {
                set_single_persistent_default_layer(_MAC);
            }
            disable_sticky_scroll();
            set_ball_mode(BALL_MODE_POINTER);
            break;
    }
}

// -----------------------------------------------------------------------------
// Keymaps
// -----------------------------------------------------------------------------

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        C(A(KC_TAB)), DPI_CONFIG, TD(MCLK), TD(RCLK),
        MS_BTN1,                            TD(VOLU)
    ),

    [_VOL] = LAYOUT(
        _______, _______, S(KC_COMM), S(KC_DOT),
        _______,                      _______
    ),

    [_MAC] = LAYOUT(
        C(KC_UP), DPI_CONFIG, TD(MCLK), TD(RCLK),
        MS_BTN1,                        TD(VOLU)
    ),
};

// -----------------------------------------------------------------------------
// Custom key processing
// -----------------------------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed && sticky_scroll) {
        disable_sticky_scroll();
    }

    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case COPYTXT:
            SEND_STRING(
                SS_DOWN(X_LCTL)
                SS_TAP(X_C)
                SS_DELAY(100)
                SS_TAP(X_L)
                SS_DELAY(50)
                SS_TAP(X_V)
                SS_DELAY(50)
                SS_TAP(X_A)
                SS_DELAY(50)
                SS_TAP(X_C)
                SS_UP(X_LCTL)
            );
            return false;

        case OFF:
            disable_sticky_scroll();
            set_ball_mode(BALL_MODE_POINTER);
            return false;

        case DF_WIN:
            set_single_persistent_default_layer(_BASE);
            disable_sticky_scroll();
            set_ball_mode(BALL_MODE_POINTER);
            return false;

        case DF_MAC:
            set_single_persistent_default_layer(_MAC);
            disable_sticky_scroll();
            set_ball_mode(BALL_MODE_POINTER);
            return false;
    }

    return true;
}