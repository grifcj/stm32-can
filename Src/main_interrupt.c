#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

CAN_HandleTypeDef hcan1 = {};

#define CAN_TEST_MSG_STDID 0x321

#define MAX_CAN_DATA 8
uint8_t txData[MAX_CAN_DATA] = { 1, 2, 3, 4, 5, 6, 7, 8 };
uint8_t rxData[MAX_CAN_DATA] = {};

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
   hcan1.Init.Mode = CAN_MODE_NORMAL;
   hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
   hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
   hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
   hcan1.Init.Prescaler = 6;
   if (HAL_CAN_Init(&hcan1) != HAL_OK)
   {
      SystemHalt("HAL_CAN_Init Error");
   }

   // Configure filter to match the transmitted message id
   CAN_FilterTypeDef filterConfig = {};
   filterConfig.FilterBank = 0;
   filterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
   filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
   filterConfig.FilterIdHigh = CAN_TEST_MSG_STDID << 5;
   filterConfig.FilterIdLow = 0x0000;
   filterConfig.FilterMaskIdHigh = 0x0000;
   filterConfig.FilterMaskIdLow = 0x0000;
   filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
   filterConfig.FilterActivation = ENABLE;
   filterConfig.SlaveStartFilterBank = 27;
   if (HAL_CAN_ConfigFilter(&hcan1, &filterConfig) != HAL_OK)
   {
      SystemHalt("HAL_CAN_ConfigFilter Error");
   }

   if (HAL_CAN_Start(&hcan1) != HAL_OK)
   {
      SystemHalt("HAL_CAN_Start Error");
   }

   if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
   {
      SystemHalt("HAL_CAN_ActivateNotification Error");
   }

   /*##-3- Configure the NVIC #################################################*/
   /* NVIC configuration for CAN1 Reception complete interrupt */
   HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
   HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
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

void TransmitMessage()
{
   static int counter = 0;

   printf("Send CAN message count=%d\n", counter++);

   CAN_TxHeaderTypeDef txHeader;
   txHeader.StdId = CAN_TEST_MSG_STDID;
   txHeader.RTR = CAN_RTR_DATA;
   txHeader.IDE = CAN_ID_STD;
   txHeader.DLC = MAX_CAN_DATA;
   txHeader.TransmitGlobalTime = DISABLE;
   uint32_t txMailbox = 0;
   if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, txData, &txMailbox) != HAL_OK)
   {
      Log("HAL_CAN_AddTxMessage Error");
   }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
   CAN_RxHeaderTypeDef rxHeader = {};
   if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
   {
      SystemHalt("HAL_CAN_GetRxMessage Error");
   }

   printf("Received CAN message id=0x%X RTR=%d DLC=%d "
         "data={%x, %x, %x, %x, %x, %x, %x, %x}\n",
         rxHeader.StdId,
         rxHeader.RTR,
         rxHeader.DLC,
         rxData[0],
         rxData[1],
         rxData[2],
         rxData[3],
         rxData[4],
         rxData[5],
         rxData[6],
         rxData[7]);

   if ((rxHeader.StdId == 0x321)
         && (rxHeader.RTR == CAN_RTR_DATA)
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
      static const int KEY_PRESSED = 0x00;
      static const int KEY_RELEASED = 0x01;

      // User presses tamper button to send message
      if (BSP_PB_GetState(BUTTON_TAMPER) == KEY_PRESSED)
      {
         // Wait until button released before continuing
         while (BSP_PB_GetState(BUTTON_TAMPER) != KEY_RELEASED)
         {
         }

         TransmitMessage();
      }
   }
}
