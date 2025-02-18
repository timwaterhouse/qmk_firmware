// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "features/achordion.h"

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  // Your macros ...

  if (record->event.pressed) {
    // On every key press, print the event's keycode and matrix position.
    dprintf("kc=0x%04X, row=%2u, col=%2u\n",
        keycode, record->event.key.row, record->event.key.col);
  }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  // Exceptionally consider the following chords as holds, even though they
  // are on the same hand in Dvorak.
  /* switch (tap_hold_keycode) { */
  /*   case HOME_A:  // A + U. */
  /*     if (other_keycode == HOME_U) { return true; } */
  /*     break; */

  /*   case HOME_S:  // S + H and S + G. */
  /*     if (other_keycode == HOME_H || other_keycode == KC_G) { return true; } */
  /*     break; */
  /* } */

  // Also allow same-hand holds when the other key is in the rows below the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
  /* if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 4) { return true; } */
  const uint16_t row = other_record->event.key.row;
  const uint16_t col = other_record->event.key.col;
  if (row == 4 || row == 9 || col == 0) { return true; }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

