// Number of modes
#define MODECOUNT 2

// Controlls the 7 segment display digits
// If set to 0 the digit will light up
// Uses DDRD.
#define DIGITSELECT_0 4
#define DIGITSELECT_1 5
#define DIGITSELECT_2 6
#define DIGITSELECT_3 7

// Buttons used to operate the alarm clock
// Uses DDRD/PIND - Pull up inputs
#define NEXTBTN 2
#define INCREMENTBTN 3
#define MODESELECTBTN 7

// Controlls the buzzer and the led
// DDRB
#define BUZZER 3

// To display the correct number you have to start sending the digits from the last digit (which place represents the highest value)
// 1 => 0x06 => 00000110 => (send 0 and shift) x 5, (send 1 and shift) x 2, (send 0 and shift)
static unsigned char digitNumbers[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Stores the values of the hex representations of the used letters
// Letters: A, L, r
static unsigned char digitLetters[3] = {0x77, 0x38, 0x50};

// Numbers with a decimal dot.
static unsigned char digitNumbersDecimalDot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

// The set alarms are stored here.
static int alarms[9];

// This array holds the displayed values.
static unsigned int digitsCache[4];

// Holds the value of the minute counter
// If this variable reaches 0 it indicates that a minute has passed.
static unsigned char minuteCounter = 60;

// If true the dot point for the given digit will light up.
static unsigned char showDotPoint = 0;

// Holds the value of the selected mode.
static unsigned char currentMode = 0;

// Determines if the current mode is in an alternative mode.
static unsigned char altMode = 0;

// Determines if the current mode is in an alternative mode.
static unsigned char editMode = 0;

// 1 if mode button is pressed.
static unsigned char btnPress = 0;

// Counts how many seconds were the button pressed.
static unsigned char btnHoldCounter = 0;

//Stores current which digit is lit up
static unsigned char currDigit = 0;

// Initial setup. Sets the pins and other basic variables to the default state.
void InitSetup();

// Displays the time on a given digit.
void DisplayDigit(int digitNum, unsigned char dotPoint);

// Caches the currently shown digits.
void CacheDigits();

// Steps every 1 second
void TimerOneSetup();

// This timer will refresh the 7 segment display
void TimerTwoSetup();

// External interrupt setup. Detects button presses.
void ExtInterruptSetup();