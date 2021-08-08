// Outputs a single digit that is converted from the "Time" variable
// The digit determines which part of the time is outputted
// E.g.: (Time: 43200 [12:00]) -> TimeToNum(0) => output = 1
// (Time: 43200 [12:00]) -> TimeToNum(1) => output = 2
int TimeToNum(int digit);

// Caches the currently shown digits.
void CacheDigits(unsigned int digitsCache[]);

// Days in each month.
unsigned int monthDayNum[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Indicates if the current year is a leap year.
unsigned char isLeapYear = 0;

// Holds the time value in minutes. Default value = 720 (12:00).
int time = 720;

// Stores the year.
int year = 2021;

// Stores the day and the month.
int date = 1;
