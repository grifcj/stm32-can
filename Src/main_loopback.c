#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

CAN_HandleTypeDef hcan1 = {};

static void CAN_Init(void)
{
   // Initialize CAN module
   hcan1.Instance = CAN1;
   hcan1.Init.TimeTriggeredMode = DISABLE;
   hcan1.Init.AutoBusOff = DISABLE;
   hcan1.Init.AutoWakeUp = DISABLE;
   hcan1.Init.AutoRetransmission = ENABLE;
   hcan1.Init.ReceiveFifoLocked = DISABLE;
   hcan1.Init.TransmitFifoPriority = DISABLE;
   hcan1.Init.Mode = CAN_MODE_LOOPBACK;
   hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
   hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
   hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
   hcan1.Init.Prescaler = 6;
   if (HAL_CAN_Init(&hcan1) != HAL_OK)
   {
      SystemHalt("HAL_CAN_Init Error");
   }

   // Set filter to do what?
   // Are we configuring to only allow our message or ANY message to come through?
   CAN_FilterTypeDef filterConfig = {};
   filterConfig.FilterBank = 0;
   filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
   filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
   filterConfig.FilterIdHigh = 0x0000;
   filterConfig.FilterIdLow = 0x0000;
   filterConfig.FilterMaskIdHigh = 0x0000;
   filterConfig.FilterMaskIdLow = 0x0000;
   filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
   filterConfig.FilterActivation = ENABLE;
   filterConfig.SlaveStartFilterBank = 14;
   if (HAL_CAN_ConfigFilter(&hcan1, &filterConfig) != HAL_OK)
   {
      SystemHalt("HAL_CAN_ConfigFilter Error");
   }

   if (HAL_CAN_Start(&hcan1) != HAL_OK)
   {
      SystemHalt("HAL_CAN_Start Error");
   }
}

void InitHardware()
{
   // Init periperhal driver stack
   HAL_Init();

   // We don't necessarily need clocks for renode tests, but
   // if we were to ever put on hardware would need them
   SystemClock_Config();

   // Use the uart for logging
   UART_Init();

   // We're testing CAN so initialize it of course
   CAN_Init();

   // Button used for triggering send
   BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
}

void TransmitMessageAndExpectLoopbackEcho()
{
   static int counter = 0;

   printf("Send CAN message count=%d\n", counter++);

#define MAX_CAN_DATA 8
   uint8_t txData[MAX_CAN_DATA] = { 1, 2, 3, 4, 5, 6, 7, 8 };

   CAN_TxHeaderTypeDef txHeader;
   txHeader.StdId = 0x321;
   txHeader.RTR = CAN_RTR_DATA;
   txHeader.IDE = CAN_ID_STD;
   txHeader.DLC = MAX_CAN_DATA;
   txHeader.TransmitGlobalTime = DISABLE;
   uint32_t txMailbox = 0;
   if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, txData, &txMailbox) != HAL_OK)
   {
      Log("HAL_CAN_AddTxMessage Error");
   }

   while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0)
   {
   }

   CAN_RxHeaderTypeDef rxHeader = {};
   uint8_t rxData[MAX_CAN_DATA] = {};
   if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
   {
      Log("HAL_CAN_GetRxMessage Error");
   }

   Log("Received CAN message");

   if ((rxHeader.StdId == 0x321)
         && (rxHeader.RTR ==  CAN_RTR_DATA)
         && (rxHeader.DLC == MAX_CAN_DATA)
         && (memcmp(txData, rxData, MAX_CAN_DATA) == 0))
   {
      Log("Received data matches transmitted data");
   }
}

int main(void)
{
   InitHardware();

   while (1)
   {
      static const int KEY_PRESSED = 0x01;
      static const int KEY_RELEASED = 0x00;

      // User presses tamper button to send message
      if (BSP_PB_GetState(BUTTON_TAMPER) == KEY_PRESSED)
      {
         // Wait until button released before continuing
         while (BSP_PB_GetState(BUTTON_TAMPER) != KEY_RELEASED)
         {
         }

         TransmitMessageAndExpectLoopbackEcho();
      }
   }
}
