// MUST be between 1 - 9
#define ALARMCOUNT 9

struct Alarm
{
    int time;
    unsigned char isEnabled;
};

// Stores the values of the hex representations of the used letters
// Letters: A, L, r, E, n
unsigned char digitLetters[5] = {0x77, 0x38, 0x50, 0x79, 0x54};

// Stores displayed alarm in a numerical format.
extern unsigned int alarmCache[4];

// Stores the currently selected alarm.
unsigned char currentAlarm = 0;

// The set alarms are stored here.
Alarm alarms[ALARMCOUNT];

// Displays the alarms.
// digitNum = number of displayed digit.
void DisplayAlarms(int digitNum);

// Displays the time of the currently selected alarm
// digitNum = number of displayed digit.
void DisplayAlarmTime(int digitNum);