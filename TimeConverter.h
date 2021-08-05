// Outputs a single digit that is converted from the "Time" variable
// The digit determines which part of the time is outputted
// E.g.: (Time: 43200 [12:00]) -> TimeToNum(0) => output = 1
// (Time: 43200 [12:00]) -> TimeToNum(1) => output = 2
unsigned int TimeToNum(int digit, unsigned int time);

void CacheDigits(unsigned int digitsCache[], unsigned int time);