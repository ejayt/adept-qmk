# Ploopy Adept – Custom QMK Keymap (ejay)

## Overview

This is a custom QMK keymap for the **Ploopy Adept trackball**, designed as a hybrid:

* 🖱️ High-control pointing device
* ⌨️ Macro pad for browser + system control
* 🔁 Dual OS support (Windows + macOS)
* 🎯 Mode-based trackball behavior (scroll, zoom, volume, arrows)

The layout is optimized for **single-hand use with minimal finger travel**.

---

## Button Layout

```
LAYOUT(
  top_left, middle_left, middle_right, top_right,
  bottom_left,                      bottom_right
)
```

| Position     | Button |
| ------------ | ------ |
| Top Left     | TL     |
| Middle Left  | ML     |
| Middle Right | MR     |
| Top Right    | TR     |
| Bottom Left  | BL     |
| Bottom Right | BR     |

---

## Base Behavior (Windows Default)

### TL – Task Switcher

* Tap: `Ctrl + Alt + Tab` (persistent switcher)

### ML – DPI

* Tap: Cycle DPI

### MR – Middle Click / Zoom / Reopen Tab

* Tap: Middle click
* Hold: **Zoom mode** (ball = zoom in/out)
* Double Tap: Reopen tab (`Ctrl + Shift + T`)

---

### TR – Right Click / Scroll / Close Tab

* Tap: Right click
* Hold: **Scroll mode** (drag scroll)
* Double Tap: Close tab (`Ctrl + W`)
* Triple Tap: Enter **Lock Layer** (persistent scroll)

---

### BL – Left Click

* Standard left click

---

### BR – Escape / Volume Mode

* Tap: `Esc`
* Hold: **Volume mode** (ball = volume up/down)

---

## macOS Behavior

Switching to macOS mode updates all relevant shortcuts:

| Action        | Windows      | macOS       |
| ------------- | ------------ | ----------- |
| Close tab     | Ctrl+W       | Cmd+W       |
| Reopen tab    | Ctrl+Shift+T | Cmd+Shift+T |
| Copy          | Ctrl+C       | Cmd+C       |
| Paste         | Ctrl+V       | Cmd+V       |
| Zoom modifier | Ctrl         | Cmd         |

### TL (Mac Only)

* Tap: **Mission Control** (`Ctrl + ↑`)

---

## Modes (Trackball Behavior)

| Mode    | Activated By       | Behavior                           |
| ------- | ------------------ | ---------------------------------- |
| Pointer | Default            | Normal mouse movement              |
| Scroll  | TR Hold            | Ball scrolls (vertical/horizontal) |
| Zoom    | MR Hold            | Ball zooms (Ctrl/Cmd + scroll)     |
| Volume  | BR Hold            | Ball controls system volume        |
| Arrows  | (Unused currently) | Ball sends arrow keys              |

---

## Lock Layer

Activated via:

* TR triple tap
* OR combo (see below)

### Behavior:

* Locks scroll mode
* Allows OS switching

| Button | Action                 |
| ------ | ---------------------- |
| TL     | Switch to macOS mode   |
| BR     | Switch to Windows mode |
| Others | Exit lock mode         |

---

## Combos

| Combo                          | Action           |
| ------------------------------ | ---------------- |
| TL + TR (AltTab + Right Click) | Reset keyboard   |
| ML + MR                        | Escape           |
| BR + TR                        | Enter Lock Layer |

---

## OS Switching

The device maintains a **persistent default layer**:

* `DF_MAC` → macOS mode
* `DF_WIN` → Windows mode

This avoids relying on OS-level modifier remapping.

---

## Design Philosophy

This layout follows a few key principles:

### 1. Mode-based control

Instead of more buttons, the ball changes behavior:

* Move → Scroll → Zoom → Volume

### 2. Tap vs Hold vs Multi-tap

Each button has layered intent:

* Tap = primary action
* Hold = mode shift
* Double tap = secondary action
* Triple tap = system-level behavior

### 3. OS-native behavior

No reliance on OS remapping:

* All differences handled in firmware
* Fully portable across machines

### 4. Minimal cognitive load

* Same physical actions across OS
* Only modifier differences change

---

## Build & Flash

### Compile

```bash
qmk compile -kb ploopyco/madromys/rev1_001 -km ejay
```

### Flash

1. Put Adept into bootloader mode
2. Drag `.uf2` file onto device

---

## Notes / Future Improvements

* `COPYTXT` macro still uses Ctrl (needs macOS-aware version)
* Possible split of `_LOCK` into:

  * OS selection layer
  * scroll lock layer
* Potential OS auto-detection (not currently used)

---

## Author

Ejay – DevOps engineer, trackball enjoyer, and chaos-driven input designer 😄
