  .syntax unified     /*.syntax unified 是一个指示，说明下面的指令是ARM和THUMB通用格式的*/
  .cpu cortex-m4      /* 设定内核 */
  .fpu softvfp
  .thumb            /* thumb指令集 */

#include "version.h"

/* .global 使某方法可以被其他目标文件使用 */
.global  g_pfnVectors
.global  Default_Handler


/* data节的初始化值的起始地址。
在链接器脚本中定义 */
.word  _sidata    
/* .data节的起始地址。在链接器脚本中定义 */  
.word  _sdata
/* .data节的结束地址。在链接器脚本中定义*/
.word  _edata
/* .bss节的起始地址。在链接器脚本中定义 */
.word  _sbss
/* .bss节的结束地址。在链接器脚本中定义 */
.word  _ebss
/* 用于SystemInit_ExtMemCtl的堆栈；始终使用内部RAM */
.word _stext
.word _etext

/**
 * @brief  这是处理器第一次调用的代码
*           在重置事件之后开始执行。只有绝对的
*           执行必要的设置，之后应用程序
*           调用提供的main（）例程。
 * @param  None
 * @retval : None
*/


    .section  .text.Reset_Handler   /* 创建新的代码段 */
  .weak  Reset_Handler            /* 声明启动函数 */
  .type  Reset_Handler, %function    /* 声明Reset_Handler 为函数 */
Reset_Handler:  
  ldr   sp, =_estack    		 /* 设置堆栈指针 */
/* 将数据段初始值设定项从闪存复制到SRAM */  
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
   
/* 零填充bss段。 */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* 调用时钟系统初始化功能.*/
  bl  SystemInit   
/* 调用静态构造函数  汇编器自带 */
  bl __libc_init_array
/* 调用应用程序的入口点.*/
  bl  main
  bx  lr    
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  这是当处理器接收到
*           意外中断。这只是进入一个无限循环
*           由调试器检查的系统状态。
 * @param  None     
 * @retval None       
*/
.section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
.size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* 向量表。请注意，正确的构造
*必须放在上面，以确保它以物理地址结束
*0x0000.0000。
* 
*******************************************************************************/
.section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     WWDGT_IRQHandler                  /*16:Window Watchdog Timer            */                                        
  .word     LVD_IRQHandler                    /* 17:LVD through EXTI Line detect    */                        
  .word     TAMPER_IRQHandler                 /* 18:Tamper through EXTI Line detect */            
  .word     RTC_IRQHandler                    /* 19:RTC through EXTI Line           */                      
  .word     FMC_IRQHandler                    /* 20:FMC                             */                                          
  .word     RCU_CTC_IRQHandler                /* 21:RCU and CTC                     */                                            
  .word     EXTI0_IRQHandler                  /* 22:EXTI Line 0                     */                        
  .word     EXTI1_IRQHandler                  /* 23:EXTI Line 1                     */                          
  .word     EXTI2_IRQHandler                  /* 24:EXTI Line 2                     */                          
  .word     EXTI3_IRQHandler                  /* 25:EXTI Line 3                     */                          
  .word     EXTI4_IRQHandler                  /* 26:EXTI Line 4                     */                          
  .word     DMA0_Channel0_IRQHandler          /* 27:DMA0 Channel0                   */                  
  .word     DMA0_Channel1_IRQHandler          /* 28:DMA0 Channel1                   */                   
  .word     DMA0_Channel2_IRQHandler          /* 29:DMA0 Channel2                   */                   
  .word     DMA0_Channel3_IRQHandler          /* 30:DMA0 Channel3                   */                   
  .word     DMA0_Channel4_IRQHandler          /* 31:DMA0 Channel4                   */                   
  .word     DMA0_Channel5_IRQHandler          /* 32:DMA0 Channel5                   */                   
  .word     DMA0_Channel6_IRQHandler          /* 33:DMA0 Channel6                   */                   
  .word     ADC0_1_IRQHandler                 /* 34:ADC0 and ADC1                   */                   
  .word     CAN0_TX_IRQHandler   			  /* 35:CAN0 TX                         */                         
  .word     CAN0_RX0_IRQHandler  		      /* 36:CAN0 RX0                        */                          
  .word     CAN0_RX1_IRQHandler               /* 37:CAN0 RX1                        */                          
  .word     CAN0_EWMC_IRQHandler              /* 38:CAN0 EWMC                       */                          
  .word     EXTI5_9_IRQHandler                /* 39:EXTI5 to EXTI9                  */                          
  .word     TIMER0_BRK_TIMER8_IRQHandler      /* 40:TIMER0 Break and TIMER8         */         
  .word     TIMER0_UP_TIMER9_IRQHandler       /* 41:TIMER0 Update and TIMER910      */         
  .word     TIMER0_TRG_CMT_TIMER10_IRQHandler /* 42:TIMER0 Trigger and Commutation and TIMER10  */
  .word     TIMER0_Channel_IRQHandler         /* 43:TIMER0 Channel Capture Comparee             */                          
  .word     TIMER1_IRQHandler                 /* 44:TIMER1                          */                   
  .word     TIMER2_IRQHandler                 /* 45:TIMER2                          */                   
  .word     TIMER3_IRQHandler                 /* 46:TIMER3                          */                   
  .word     I2C0_EV_IRQHandler                /* 47:I2C0 Event                      */                          
  .word     I2C0_ER_IRQHandler                /* 48:I2C0 Error                      */                          
  .word     I2C1_EV_IRQHandler                /* 49:I2C1 Event                      */                          
  .word     I2C1_ER_IRQHandler                /* 50:I2C1 Error                      */                            
  .word     SPI0_IRQHandler                   /* 51:SPI0                            */                   
  .word     SPI1_IRQHandler                   /* 52:SPI1                            */                   
  .word     USART0_IRQHandler                 /* 53:USART0                          */                   
  .word     USART1_IRQHandler                 /* 54:USART1                          */                   
  .word     USART2_IRQHandler    		      /* 55:USART2                          */                   
  .word     EXTI10_15_IRQHandler              /* 56:EXTI10 to EXTI15                */                          
  .word     RTC_Alarm_IRQHandler              /* 57:RTC Alarm                       */                 
  .word     USBFS_WKUP_IRQHandler             /* 58:USBFS Wakeup                    */                       
  .word     TIMER7_BRK_TIMER11_IRQHandler     /* 59:TIMER7 Break and TIMER11  			*/         
  .word     TIMER7_UP_TIMER12_IRQHandler      /* 60:TIMER7 Update and TIMER12    		*/         
  .word     TIMER7_TRG_CMT_TIMER13_IRQHandler /* 61:TIMER7 Trigger and Commutation and TIMER13				*/
  .word     TIMER7_Channel_IRQHandler         /* 62:TIMER7 Channel Capture Compare  */                          
  .word     0                                 /* 63:               */                          
  .word     EXMC_IRQHandler                   /* 64:EXMC                            */                   
  .word     0                                 /* 65:                          */                   
  .word     TIMER4_IRQHandler                 /* 66:TIMER4                          */                   
  .word     SPI2_IRQHandler                   /* 67:SPI2                            */                   
  .word     UART3_IRQHandler                  /* 68:UART3                           */                   
  .word     UART4_IRQHandler                  /* 69:UART4                           */                   
  .word     TIMER5_IRQHandler                 /* 70:TIMER5                          */                   
  .word     TIMER6_IRQHandler                 /* 71:TIMER6                          */
  .word     DMA1_Channel0_IRQHandler          /* 72:DMA1 Channel0                   */                   
  .word     DMA1_Channel1_IRQHandler          /* 73:DMA1 Channel1                   */                   
  .word     DMA1_Channel2_IRQHandler          /* 74:DMA1 Channel2                   */                   
  .word     DMA1_Channel3_IRQHandler          /* 75:DMA1 Channel3                   */                   
  .word     DMA1_Channel4_IRQHandler          /* 76:DMA1 Channel4                   */                   
  .word     0                   		          /* 77:Ethernet                        */                   
  .word     0                                 /* 78:Ethernet Wakeup through EXTI Line              */                     
  .word     CAN1_TX_IRQHandler  			        /* 79:CAN1 TX                         */                          
  .word     CAN1_RX0_IRQHandler 			        /* 80:CAN1 RX0                        */                          
  .word     CAN1_RX1_IRQHandler 			        /* 81:CAN1 RX1                        */                          
  .word     CAN1_EWMC_IRQHandler			        /* 82:CAN1 EWMC*/ 
  .word     USBFS_IRQHandler                  /* 83:USBFS IRQHandler  */
  .word     _version                          /* 84:版本号记录  */
/*******************************************************************************
*
* 为Default_handler的每个异常处理程序提供弱别名。
*由于它们是弱别名，任何同名函数都将覆盖
*这个定义。
* 
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,vPortSVCHandler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,xPortPendSVHandler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,xPortSysTickHandler              
  
   .weak      WWDGT_IRQHandler                   
   .thumb_set WWDGT_IRQHandler,Default_Handler      
                  
   .weak      LVD_IRQHandler      
   .thumb_set LVD_IRQHandler,Default_Handler
               
   .weak      TAMPER_IRQHandler            
   .thumb_set TAMPER_IRQHandler,Default_Handler
            
   .weak      RTC_IRQHandler                  
   .thumb_set RTC_IRQHandler,Default_Handler
            
   .weak      FMC_IRQHandler         
   .thumb_set FMC_IRQHandler,Default_Handler
                  
   .weak      RCU_CTC_IRQHandler      
   .thumb_set RCU_CTC_IRQHandler,Default_Handler
                  
   .weak      EXTI0_IRQHandler         
   .thumb_set EXTI0_IRQHandler,Default_Handler
                  
   .weak      EXTI1_IRQHandler         
   .thumb_set EXTI1_IRQHandler,Default_Handler
                     
   .weak      EXTI2_IRQHandler         
   .thumb_set EXTI2_IRQHandler,Default_Handler 
                 
   .weak      EXTI3_IRQHandler         
   .thumb_set EXTI3_IRQHandler,Default_Handler
                        
   .weak      EXTI4_IRQHandler         
   .thumb_set EXTI4_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel0_IRQHandler               
   .thumb_set DMA0_Channel0_IRQHandler,Default_Handler
         
   .weak      DMA0_Channel1_IRQHandler               
   .thumb_set DMA0_Channel1_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel2_IRQHandler               
   .thumb_set DMA0_Channel2_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel3_IRQHandler               
   .thumb_set DMA0_Channel3_IRQHandler,Default_Handler 
                 
   .weak      DMA0_Channel4_IRQHandler              
   .thumb_set DMA0_Channel4_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel5_IRQHandler               
   .thumb_set DMA0_Channel5_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel6_IRQHandler               
   .thumb_set DMA0_Channel6_IRQHandler,Default_Handler
                  
   .weak      ADC0_1_IRQHandler      
   .thumb_set ADC0_1_IRQHandler,Default_Handler
            
   .weak      CAN0_TX_IRQHandler   
   .thumb_set CAN0_TX_IRQHandler,Default_Handler
            
   .weak      CAN0_RX0_IRQHandler            
   .thumb_set CAN0_RX0_IRQHandler,Default_Handler
            
   .weak      CAN0_RX1_IRQHandler            
   .thumb_set CAN0_RX1_IRQHandler,Default_Handler
      
   .weak      CAN0_EWMC_IRQHandler      
   .thumb_set CAN0_EWMC_IRQHandler,Default_Handler
      
   .weak      EXTI5_9_IRQHandler   
   .thumb_set EXTI5_9_IRQHandler,Default_Handler
                  
   .weak      TIMER0_BRK_TIMER8_IRQHandler            
   .thumb_set TIMER0_BRK_TIMER8_IRQHandler,Default_Handler
                  
   .weak      TIMER0_UP_TIMER9_IRQHandler            
   .thumb_set TIMER0_UP_TIMER9_IRQHandler,Default_Handler
                  
   .weak      TIMER0_TRG_CMT_TIMER10_IRQHandler            
   .thumb_set TIMER0_TRG_CMT_TIMER10_IRQHandler,Default_Handler
                  
   .weak      TIMER0_Channel_IRQHandler   
   .thumb_set TIMER0_Channel_IRQHandler,Default_Handler
                     
   .weak      TIMER1_IRQHandler   
   .thumb_set TIMER1_IRQHandler,Default_Handler
                     
   .weak      TIMER2_IRQHandler   
   .thumb_set TIMER2_IRQHandler,Default_Handler
                  
   .weak      TIMER3_IRQHandler   
   .thumb_set TIMER3_IRQHandler,Default_Handler
                           
   .weak      I2C0_EV_IRQHandler            
   .thumb_set I2C0_EV_IRQHandler,Default_Handler
                        
   .weak      I2C0_ER_IRQHandler            
   .thumb_set I2C0_ER_IRQHandler,Default_Handler
                  
   .weak      I2C1_EV_IRQHandler      
   .thumb_set I2C1_EV_IRQHandler,Default_Handler
                     
   .weak      I2C1_ER_IRQHandler      
   .thumb_set I2C1_ER_IRQHandler,Default_Handler
                                  
   .weak      SPI0_IRQHandler               
   .thumb_set SPI0_IRQHandler,Default_Handler
               
   .weak      SPI1_IRQHandler               
   .thumb_set SPI1_IRQHandler,Default_Handler
            
   .weak      USART0_IRQHandler         
   .thumb_set USART0_IRQHandler,Default_Handler
            
   .weak      USART1_IRQHandler               
   .thumb_set USART1_IRQHandler,Default_Handler
                     
   .weak      USART2_IRQHandler            
   .thumb_set USART2_IRQHandler,Default_Handler
                     
   .weak      EXTI10_15_IRQHandler            
   .thumb_set EXTI10_15_IRQHandler,Default_Handler
                     
   .weak      RTC_Alarm_IRQHandler            
   .thumb_set RTC_Alarm_IRQHandler,Default_Handler
                     
   .weak      USBFS_WKUP_IRQHandler               
   .thumb_set USBFS_WKUP_IRQHandler,Default_Handler
               
   .weak      TIMER7_BRK_TIMER11_IRQHandler               
   .thumb_set TIMER7_BRK_TIMER11_IRQHandler,Default_Handler
                  
   .weak      TIMER7_UP_TIMER12_IRQHandler               
   .thumb_set TIMER7_UP_TIMER12_IRQHandler,Default_Handler
            
   .weak      TIMER7_TRG_CMT_TIMER13_IRQHandler               
   .thumb_set TIMER7_TRG_CMT_TIMER13_IRQHandler,Default_Handler
            
   .weak      TIMER7_Channel_IRQHandler               
   .thumb_set TIMER7_Channel_IRQHandler,Default_Handler

   .weak      EXMC_IRQHandler      
   .thumb_set EXMC_IRQHandler,Default_Handler

   .weak      TIMER4_IRQHandler               
   .thumb_set TIMER4_IRQHandler,Default_Handler
                  
   .weak      SPI2_IRQHandler               
   .thumb_set SPI2_IRQHandler,Default_Handler
                  
   .weak      UART3_IRQHandler               
   .thumb_set UART3_IRQHandler,Default_Handler
                  
   .weak      UART4_IRQHandler      
   .thumb_set UART4_IRQHandler,Default_Handler
                        
   .weak      TIMER5_IRQHandler   
   .thumb_set TIMER5_IRQHandler,Default_Handler
                        
   .weak      TIMER6_IRQHandler   
   .thumb_set TIMER6_IRQHandler,Default_Handler
                        
   .weak      DMA1_Channel0_IRQHandler                  
   .thumb_set DMA1_Channel0_IRQHandler,Default_Handler  

   .weak      DMA1_Channel1_IRQHandler                  
   .thumb_set DMA1_Channel1_IRQHandler,Default_Handler 

   .weak      DMA1_Channel2_IRQHandler      
   .thumb_set DMA1_Channel2_IRQHandler,Default_Handler
                        
   .weak      DMA1_Channel3_IRQHandler   
   .thumb_set DMA1_Channel3_IRQHandler,Default_Handler

   .weak      DMA1_Channel4_IRQHandler   
   .thumb_set DMA1_Channel4_IRQHandler,Default_Handler

   .weak      CAN1_TX_IRQHandler      
   .thumb_set CAN1_TX_IRQHandler,Default_Handler
                        
   .weak      CAN1_RX0_IRQHandler   
   .thumb_set CAN1_RX0_IRQHandler,Default_Handler
                        
   .weak      CAN1_RX1_IRQHandler   
   .thumb_set CAN1_RX1_IRQHandler,Default_Handler
                        
   .weak      CAN1_EWMC_IRQHandler                  
   .thumb_set CAN1_EWMC_IRQHandler,Default_Handler  

   .weak      USBFS_IRQHandler      
   .thumb_set USBFS_IRQHandler,Default_Handler
   