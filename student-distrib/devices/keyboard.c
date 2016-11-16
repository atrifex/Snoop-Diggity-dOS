#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "types.h"
#include "filesystem.h"

// specifies the color on the screen
int curr_attribute = ATTRIB;
int curr_back_attribute = 0;

// variables associated with reading
volatile int allowed_to_read = 0;          // allows read to stop blocking
volatile int read_waiting = 0;

// holds state of "command" keys
uint8_t keyboard_state = 0;

// Stdin and index
uint8_t stdin[KEYBOARD_BUFF_SIZE];       // number of chars in a row is 80 ---> why do we want 128 then?
int stdin_index;                     // points to current free spot in stdin


//TODO: make seperate drivers for keybord and terminal

/*
 * get_char
 * DESCRIPTION: Retrieves scancode(s) from control data port
 * INPUT: none.
 * OUTPUTS: scancode(s) present in control data port
 * RETURN VALUE: none.
 * SIDE EFFECTS: Drains keyboard input
*/
uint8_t get_char()
{
    // get first byte of data
    uint8_t data = inb(KEYBOARD_DATA_PORT);

    // get second byte of multi-byte sequences
    if(data == MB_SEQ_INIT) {
        io_wait();
        data = inb(KEYBOARD_DATA_PORT);
    }

    return data;
}

/*
 * init_kbd
 * DESCRIPTION: Initializes the connected PS2 keyboard on boot-up.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
void init_kbd()
{
    // set up the scancode table
    init_scancode_table();

    // init keyboard buffer attributes and fill keyboard buffer w/ null bytes
    stdin_index = 0;
    memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);

    //TODO: try to change the mode of the keyboard

    // enable the interrupt on the PIC
    enable_irq(KEYBOARD_LINE_NO);
}

/*
 * open_terminal
 * DESCRIPTION: always open
 * INPUT:       const uint8_t * pathname - path to file being opened
 * OUTPUTS:  file descriptor
 * RETURN VALUE: int fd - unsigned number if success, -1 if unable to open file
 * SIDE EFFECTS: user has access to the file
*/
int32_t open_terminal(const uint8_t *pathname)
{
    return 0;
}

/*
 * close_terminal
 * DESCRIPTION: always open
 * INPUT:   int32_t fd - file descriptor to the file being closed
 * OUTPUTS: error state
 * RETURN VALUE: int error - 0 if success and -1 if error
 * SIDE EFFECTS: uses gives up access to std out
*/
int32_t close_terminal(int32_t fd)
{
    return 0;
}


/*
 * read_terminal
 * DESCRIPTION: gets pointer to input buffer
 * INPUT:   int32_t fd - file descriptor to open fil
 *          void * buf - pointer to buf to copy to
 *          int32_t nbytes - number of bytes to copy to user (stops at null or this number)
 * OUTPUTS: copy stdin to user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied
 *                            error: -1
 * SIDE EFFECTS: user gets copy of the current stdin
*/
int32_t read_terminal(int32_t fd, void * buf, int32_t nbytes)
{
    // error check
    if(fd != STDIN){
        return ERROR;
    }

    int i = 0;
    unsigned long flags;

    // fixes type confussion
    uint8_t * buffer = (uint8_t*) buf;

    // checks for error condition
    if(nbytes < 0)
        return FAILURE;

    // tells interrupt that there is a read waiting
    read_waiting = 1;

    // if we haven't seen a new line then not allowed to read
    while(allowed_to_read == 0);

    // critical section
    cli_and_save(flags);

    // copy all information uptil the null char char
    while(stdin[i] != NULL_CHAR && i < KEYBOARD_BUFF_SIZE)
    {
        buffer[i] = stdin[i];
        i++;
    }

    // flush buffer and set stdin_index to 0
    memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
    stdin_index = 0;

    // disallow reading
    allowed_to_read = 0;
    read_waiting = 0;
    restore_flags(flags);

    // return number of bytes read
    return i;
}

/*
 * write_terminal
 * DESCRIPTION: does nothing
 * INPUT:   int32_t fd - file descriptor to open fil
 *          void * buf - pointer to buf to copy from
 *          int32_t nbytes - number of bytes to copy from user
 *          int32_t flag  - tells us if we are trying to redirect stdin
 * OUTPUTS: copy stdin from user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied
 *                            error: -1
 * SIDE EFFECTS: user outputs their string
*/
int32_t write_terminal(int32_t fd, const void *buf, int32_t nbytes)
{
    // error checks
    if(fd != STDOUT){
        return ERROR;
    }
    if(nbytes < 0){
        return FAILURE;
    }

    // print buf to the screen
    put_t((uint8_t *)buf, nbytes, 1);

    // return number of bytes read
    return nbytes;
}


/*
 * process_sent_scancode
 * DESCRIPTION: returns an unsigned long containing characters to display in the two most significant bits
 *                              in the 3 least significant bits returns the (potentially) updated keyboard_state
 *                              1xx control is on, x1x capslock is on, xx1 shift is on
 * INPUT: keyboard_state indicating current keyboard_state of shift, capslock, and control
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
unsigned long process_sent_scancode()
{
    scancode_t mapped;

    // get scancode
    uint8_t raw_scancode = get_char();

    if(raw_scancode == DELETE_SCAN_CODE)
        return keyboard_state;

    // see if we need to update keyboard_state
    switch(raw_scancode) {
        case (BACKSPACE_PRESS):
            TURN_BACKSPACE_ON(keyboard_state);
            break;
        case (BACKSPACE_RELEASE):
            TURN_BACKSPACE_OFF(keyboard_state);
            break;
        case (CAPS_LOCK_PRESS):
            TOGGLE_CAPS_LOCK(keyboard_state);
            break;
        case (CAPS_LOCK_RELEASE):
            //don't care about this
            break;
        case (SHIFT_PRESS):
            TURN_SHIFT_ON(keyboard_state);
            break;
        case (SHIFT_RELEASE):
            TURN_SHIFT_OFF(keyboard_state);
            break;
        case (R_SHIFT_PRESS):
            TURN_R_SHIFT_ON(keyboard_state);
            break;
        case (R_SHIFT_RELEASE):
            TURN_R_SHIFT_OFF(keyboard_state);
            break;
        case (CONTROL_PRESS):
            TURN_CONTROL_ON(keyboard_state);
            break;
        case (CONTROL_RELEASE):
            TURN_CONTROL_OFF(keyboard_state);
            break;
    }

    // get mapped value of scancode
    mapped = scancode_table[raw_scancode];

    // if not a make scancode then return
    if(!IS_MAKE_SC(mapped)) {
        return keyboard_state;
    }

    // check if control is pressed
    if(CONTROL_ON(keyboard_state)) {
        if(mapped.result == CLEAR_SCREEN_SHORTCUT) {
            // ctrl + l is pressed then clear screen
            clear_and_reset();
            set_cursor_location(0,0);
            printf_t("391OS> ");            // prints prompt for shell
            printf_t("%s",stdin);           // print current buffered value
        } else if(mapped.result == ASCII_SIX){
            // ctrl + 6 is pressed then change color
            curr_attribute++;
            if(curr_attribute > MAX_ATTRIB)
                curr_attribute = MIN_ATTRIB;
            change_atribute((curr_back_attribute << 4) | curr_attribute);
        } else if(mapped.result == ASCII_SEVEN){
            // ctrl + 7 is pressed then change color
            curr_back_attribute++;
            if(curr_back_attribute > MAX_BACKGROUD_ATTRIB)
                curr_back_attribute = MIN_BACKGROUD_ATTRIB;
            change_atribute((curr_back_attribute << 4) | curr_attribute);
        }
        return keyboard_state;
	} else if(BACKSPACE_ON(keyboard_state)){
        // if there are values in stdin BKSP_CHAR is seen, then delete last char
        if(stdin_index > 0) {
            putc_kbd(BKSP_CHAR);
            stdin[--stdin_index] = NULL_CHAR;
        }
    } else if(mapped.result == NEW_LINE && stdin_index < KEYBOARD_BUFF_SIZE-1) {
        // play new line on screen
        putc_kbd(NEW_LINE);
        // place NEW_LINE in buffer followed by a NULL_CHAR
        stdin[stdin_index++] = NEW_LINE;
        stdin[stdin_index] = NULL_CHAR;

        if(read_waiting == 1){
            // if shell is trying to read, then allow reading
            allowed_to_read = 1;
        }
        else{
            //else clear stdin and disallow reading
            memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
            stdin_index = 0;
            allowed_to_read = 0;
        }
        return keyboard_state;
    }

    // stop user from adding greater than 128 keyboard inputs
    if(stdin_index >= KEYBOARD_BUFF_SIZE-NULL_NL_PADDING) {
        return keyboard_state;
    }

    //if caps lock is on and shift is not then
    if(CAPS_LOCK_ON(keyboard_state) && !SHIFT_ON(keyboard_state) && !R_SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped)) {
            // if letter, then print capital version
            putc_kbd(mapped.result - ASCII_SHIFT_VAL);
            stdin[stdin_index++] = (mapped.result - ASCII_SHIFT_VAL);
            stdin[stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            // if not letter but printable, then print the car directly
            putc_kbd(mapped.result);
            stdin[stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
            stdin[stdin_index] = NULL_CHAR;
        }
    } else if(SHIFT_ON(keyboard_state) || R_SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped) && !CAPS_LOCK_ON(keyboard_state)) {
            // if shift and caps is not on, then print the capital version of the letter
            putc_kbd(mapped.result - ASCII_SHIFT_VAL);
            stdin[stdin_index++] = (mapped.result - ASCII_SHIFT_VAL); // shifting letters is simple
            stdin[stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            // if printable, then check futher characteristics
            if(non_alpha_shift_table[mapped.result] != 0) {
                // if it as char like '1', then print its shited value
                putc_kbd(non_alpha_shift_table[mapped.result]);
                stdin[stdin_index++] = (non_alpha_shift_table[mapped.result]);
                stdin[stdin_index] = NULL_CHAR;
            } else {
                // if its not shiftable, then just print directly
                putc_kbd(mapped.result);
                stdin[stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
                stdin[stdin_index] = NULL_CHAR;
            }
        }
    } else {
        if(IS_PRINTABLE_SC(mapped)) {
            // if no spcial conditions and input is printable, then print the input.
            putc_kbd(mapped.result);
            stdin[stdin_index++] = (mapped.result);
            stdin[stdin_index] = NULL_CHAR;
        }
    }

    return keyboard_state; // return current kb state
}
