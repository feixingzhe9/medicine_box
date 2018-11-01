/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "includes.h"
#include "usart.h"

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
//void PendSV_Handler(void)
//{
//}

 //void SysTick_Handler(void)
//{
//}


void DMA1_Channel7_IRQHandler(void)					//USART2-TX
{
    OSIntEnter();
    if (DMA_GetITStatus(DMA1_IT_TC7) != RESET)
    {
        DMA_Cmd(DMA1_Channel7, DISABLE);
        DMA_ClearFlag(DMA1_FLAG_TC7);
        DMA_ClearITPendingBit(DMA1_IT_TC7);  // 清除中断标志位
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //等待一包数据发送完成
    }
    OSIntExit();
}

void DMA1_Channel6_IRQHandler(void)					//USART2-TX
{
    OSIntEnter();
    if (DMA_GetITStatus(DMA1_IT_TC6) != RESET)
    {
        DMA_Cmd(DMA1_Channel7, DISABLE);
        DMA_ClearFlag(DMA1_FLAG_TC6);
        DMA_ClearITPendingBit(DMA1_IT_TC6);  // 清除中断标志位
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //等待一包数据发送完成
    }
    OSIntExit();
}

uint32_t rcv_dma_test_cnt = 0;
void USART2_IRQHandler(void)
{
	volatile unsigned char temper=0;
    uint16_t rcv_len = 0;
    OSIntEnter();
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        rcv_dma_test_cnt++;
        temper = USART2->SR;
        temper = USART2->DR;	//清USART_IT_IDLE

        DMA_Cmd(DMA1_Channel6, DISABLE);
        rcv_len = RCV_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);
        //发送命令处理信号量
//        OSSemPost(Usart2CMDSem);
//        DMA1_Channel6->CMAR = (uint32_t)&Usart2RevBuffer[0];
        DMA_SetCurrDataCounter(DMA1_Channel6, RCV_SIZE);

        DMA_Cmd(DMA1_Channel6,ENABLE);
    }
    OSIntExit();
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
