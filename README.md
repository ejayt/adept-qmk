# 🖲️ Ploopy Adept -- Custom QMK Keymap

Custom firmware for the **Ploopy Adept trackball** with:

-   Windows / macOS dual-mode (persistent)
-   Tap dance actions
-   Scroll, zoom, and volume ball modes
-   Sticky scroll
-   Combo-based OS switching

------------------------------------------------------------------------

## 🧠 Architecture Overview

### Layers

  Layer     Name      Purpose
  --------- --------- -----------------------------------
  `_BASE`   Windows   Default Windows behavior
  `_MAC`    macOS     Default macOS behavior
  `_VOL`    Volume    Temporary layer for alt functions

------------------------------------------------------------------------

## 🔁 OS Switching

### Toggle OS (Combo)

-   **DPI_CONFIG + Middle Click**
-   Toggles between Windows and macOS

### Reset (Combo)

-   **DPI_CONFIG + Middle Click + Right Click**
-   Resets keyboard

------------------------------------------------------------------------

## 🖱️ Ball Modes

-   Pointer: normal cursor movement\
-   Scroll: converts movement to scrolling\
-   Zoom: scroll + Ctrl (Windows) / Cmd (macOS)\
-   Volume: vertical movement controls volume

------------------------------------------------------------------------

## 📌 Sticky Scroll

-   Activated by **Right Click triple tap**
-   Cancels on **any key press**

------------------------------------------------------------------------

## 🎛️ Button Layout

    LAYOUT(
      top_left, middle_left, middle_right, top_right,
      bottom_left, bottom_right
    )

------------------------------------------------------------------------

## 🎮 Button Behavior

### Top Left

-   Windows: Ctrl + Alt + Tab\
-   macOS: Mission Control (Ctrl + ↑)

### Middle Left

-   DPI Config

### Middle Right (Tap Dance)

-   Tap: Middle Click\
-   Hold: Scroll Mode\
-   Double Tap: Reopen Tab

### Top Right (Tap Dance)

-   Tap: Right Click\
-   Hold: Zoom Mode\
-   Double Tap: Close Tab\
-   Triple Tap: Sticky Scroll

### Bottom Left

-   Left Click

### Bottom Right (Tap Dance)

-   Tap: Escape\
-   Hold: Volume Mode

------------------------------------------------------------------------

## 🔊 Volume Layer

-   `<` and `>` keys available while held

------------------------------------------------------------------------

## ⚙️ CPI / DPI

-   Default: 400\
-   Zoom: 50 (precision)

------------------------------------------------------------------------

## 🧩 Combos

  Combo               Action
  ------------------- -----------
  DPI + MCLK          Toggle OS
  DPI + MCLK + RCLK   Reset

------------------------------------------------------------------------

## 🧪 Smart Behavior

-   OS-aware shortcuts (Ctrl vs Cmd automatically)
-   Sticky scroll auto-disables on input
-   Mode switching resets accumulators

------------------------------------------------------------------------

## 🛠️ Notes

-   Default layer persists across reboots
-   No lock layer (simpler + safer)
-   Tap dance maximizes functionality with minimal buttons
