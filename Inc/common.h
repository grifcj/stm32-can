#ifndef COMMON_H
#define COMMON_H

void SystemClock_Config(void);
void UART_Init(void);

void SystemHalt(const char* error);
void Log(const char* log);

#endif
