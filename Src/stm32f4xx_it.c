#include "main.h"
#include "common.h"

void NMI_Handler(void) { SystemHalt("NMI"); }
void HardFault_Handler(void) { SystemHalt("Hardfault"); }
void MemManage_Handler(void) { SystemHalt("MemManage"); }
void BusFault_Handler(void) { SystemHalt("Busfault"); }
void UsageFault_Handler(void) { SystemHalt("UsageFault"); }

void SVC_Handler(void) { }
void DebugMon_Handler(void) { }
void PendSV_Handler(void) { }

void SysTick_Handler(void) { HAL_IncTick(); }

extern CAN_HandleTypeDef hcan1;
void CAN1_RX0_IRQHandler(void) { HAL_CAN_IRQHandler(&hcan1); }
void CAN1_RX1_IRQHandler(void) { HAL_CAN_IRQHandler(&hcan1); }
void CAN1_TX_IRQHandler(void) { HAL_CAN_IRQHandler(&hcan1); }
void CAN2_RX0_IRQHandler(void) { HAL_CAN_IRQHandler(&hcan1); }
void CAN2_RX1_IRQHandler(void) { HAL_CAN_IRQHandler(&hcan1); }
void CAN2_TX_IRQHandler(void) { HAL_CAN_IRQHandler(&hcan1); }
