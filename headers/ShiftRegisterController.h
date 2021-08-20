// These pins controll the shift register
#define REG_DATAINPUT 0
#define REG_OUTPUTCLK 1
#define REG_STORAGECLK 2

// Sends data to the shift register.
void SendData(unsigned char data);