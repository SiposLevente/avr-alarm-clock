// Outputs a single digit that is converted from the "Time" variable
// The digit determines which part of the time is outputted
// E.g.: (Time: 43200 [12:00]) -> TimeToNum(0) => output = 1
// (Time: 43200 [12:00]) -> TimeToNum(1) => output = 2
// Acceptable value of digit: 0-3
int TimeToNum(int digit);

// Converts "date" variable to digits.
// Acceptable value of digit: 0-3
// 0-1: month digit 0 and 1
// 2-3: day digit 0 and 1
int DateToNum(int digit);

// Converts year to digits.
// Acceptable value of digit: 0-3
int YearToNum(int digit);

// Checks if the current year is a leap year, if yes it sets the "isLeapYear" variable
void LeapYearCheck();

// Returns the current month as a number.
int GetMonth();

// Returns the current day as a number.
int GetDay();

// Caches time.
void CacheTime();

// Caches year.
void CacheYear();

// Caches month.
void CacheMonth();

// Caches day.
void CacheDay();

// Days in each month.
static unsigned int monthDayCount[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Stores time in a numerical format.
static unsigned int timeCache[4];

// Stores month and date digits in a numerical format. ([0] month,[1] month,[2] day,[3] day)
static unsigned int dateCache[4];

// Stores year as digits.
static unsigned int yearCache[4];

// Indicates if the current year is a leap year.
static unsigned char isLeapYear = 0;

// Holds the time value in minutes. Default value = 720 (12:00).
static int time = 720;

// Stores the year.
static int year = 2021;

// Stores the day and the month.
static int date = 1;
