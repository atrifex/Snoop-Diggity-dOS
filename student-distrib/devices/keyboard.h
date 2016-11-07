#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "scancodes.h"
#include "types.h"

#define CLEAR_SCREEN_SHORTCUT 'l' // control-L clears screen

#define STDIN 0
#define STDOUT 1

// state mask bits
#define SHIFT_MASK 0x1
#define CONTROL_MASK 0x2
#define CAPS_LOCK_MASK 0x4
#define BACKSPACE_MASK 0x8
#define ENTER_MASK 0x10
#define R_SHIFT_MASK 0x20

// keyboad buffer attributes
#define KEYBOARD_BUFF_SIZE 128
#define NULL_NL_PADDING 2

// delete
#define DELETE_SCAN_CODE 0x53

// update state macros
#define TURN_SHIFT_ON(state) (state = state | SHIFT_MASK)
#define TURN_R_SHIFT_ON(state) (state = state | R_SHIFT_MASK)
#define TURN_CONTROL_ON(state) (state = state | CONTROL_MASK)
#define TURN_BACKSPACE_ON(state) (state = state | BACKSPACE_MASK)
#define TURN_ENTER_ON(state) (state = state | ENTER_MASK)
#define TOGGLE_CAPS_LOCK(state) (state = state ^ CAPS_LOCK_MASK)

#define TURN_SHIFT_OFF(state) (state = state & ~SHIFT_MASK)
#define TURN_R_SHIFT_OFF(state) (state = state & ~R_SHIFT_MASK)
#define TURN_CONTROL_OFF(state) (state = state & ~CONTROL_MASK)
#define TURN_BACKSPACE_OFF(state) (state = state & ~BACKSPACE_MASK)
#define TURN_ENTER_OFF(state) (state = state & ~ENTER_MASK)

// check state macros
#define CAPS_LOCK_ON(state) (state & CAPS_LOCK_MASK)
#define CONTROL_ON(state) (state & CONTROL_MASK)
#define BACKSPACE_ON(state) (state & BACKSPACE_MASK)
#define ENTER_ON(state) (state & ENTER_MASK)
#define SHIFT_ON(state) (state & SHIFT_MASK)
#define R_SHIFT_ON(state) (state & R_SHIFT_MASK)

// initializer for a multibyte scancode sequence
#define MB_SEQ_INIT 0xE0

// shift value for making a lowercase letter uppercase
#define ASCII_SHIFT_VAL ('a' - 'A')

// keyboard communication packets
#define MAKE_RELEASE_WORD 0xF8
#define KEYBOARD_ACK 0xFA
#define KEYBOARD_RESEND 0xFE

#define PS2_CONTROL_PORT 0x64
#define PS2_DISABLE_KBD 0xAD
#define PS2_ENABLE_KBD 0xAE

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CONTROL_PORT 0x60

#define KEYBOARD_LINE_NO 1

// modifier keys
#define CONTROL_PRESS 0x1D
#define CONTROL_RELEASE 0x9D

#define SHIFT_PRESS 0x2A
#define SHIFT_RELEASE 0xAA

#define R_SHIFT_PRESS 0x36
#define R_SHIFT_RELEASE 0xB6

#define CAPS_LOCK_PRESS 0x3A
#define CAPS_LOCK_RELEASE 0xBA

#define BACKSPACE_PRESS 0x0E
#define BACKSPACE_RELEASE 0x8E

#define ENTER_PRESS 0x1C
#define ENTER_RELEASE 0x9C

// checkpoint 2
extern int rtcTest;
extern int readByIndex;
extern volatile int testVal;
extern int first_rtc_disable;
extern int can_print_by_name;
extern int can_ls;
extern int interrupt_seen;
#define TEST_ZERO   0              // normal mode
#define TEST_ONE    1              // ls emulation
#define TEST_TWO    2              // read file by name
#define TEST_THREE  3              // read file by index
#define TEST_FOUR   4              // RTC Test
#define TEST_FIVE   5              // Stop RTC

#define ASCII_ZERO  48
#define ASCII_ONE   49
#define ASCII_TWO   50
#define ASCII_THREE 51
#define ASCII_FOUR  52
#define ASCII_FIVE  53
#define RTC_MODES   16


#define RTC_TEST_0 1
#define RTC_TEST_1 2
#define RTC_TEST_2 4
#define RTC_TEST_3 8
#define RTC_TEST_4 16
#define RTC_TEST_5 32
#define RTC_TEST_6 64
#define RTC_TEST_7 128
#define RTC_TEST_8 256
#define RTC_TEST_9 512
#define RTC_TEST_10 1024
#define RTC_TEST_11 2048
#define RTC_TEST_12 4096
#define RTC_TEST_13 8192
#define RTC_TEST_14 16384
#define RTC_TEST_15 32768


// attribute related defines
#define ASCII_SIX  54
#define MAX_ATTRIB 0x0F
#define MIN_ATTRIB 0x01

/* Process the sent scancode after an interrupt */
extern unsigned long process_sent_scancode();
/* Initialize the keyboard device */
extern void init_kbd();

/* Driver related functions */
extern int32_t open_terminal(const uint8_t *pathname);
extern int32_t read_terminal(int32_t fd, void * buf, int32_t nbytes);
extern int32_t write_terminal(int32_t fd, const void *buf, int32_t nbytes);
extern int32_t close_terminal(int32_t fd);


#endif