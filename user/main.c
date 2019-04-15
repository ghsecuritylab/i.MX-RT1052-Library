/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��V2.3.1�汾�⽨�Ĺ���ģ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
# include "include.h"

/*******************************************************************
 * Prototypes
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/
extern void delayms(uint32_t ms);
void PIT0Inter();
 long count_1s = 0;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
 int main(void)
{
    /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
    /* ��ʼ�������趨 */
    BOARD_InitPins();
    /* ��ʼ��ʱ���趨 */
    BOARD_BootClockRUN();
    /* ��ʼ���������� */    
    BOARD_InitBootPeripherals();
    /* ��ʼ�����Դ��� */
    BOARD_InitDebugConsole();
    /* ��ӡϵͳʱ�� */
    PRINTF("\r\n");
    PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
    /* �����������Ĵ���^_^. */
  
    TFT_init();
    UART_Init(LPUART2, 256000);
    UART_Send(LPUART2, "123123\r\n", 8);
    UART_Send(LPUART2, "123123\r\n", 8);
    UART_Send(LPUART2, "123123\r\n", 8);
    PIT_Init_us(kPIT_Chnl_0, 1000000, false);

    while(1)
    {
      TFT_showfloat(0, 0, count_1s, 3, 0, BLACK, WHITE);
      TFT_showfloat(0,1,321.3,3,1,BLACK,WHITE);
      TFT_showfloat(0,2,23213.3,3,1,BLACK,WHITE);     
      delayms(1);
    }     

}

 void PIT0Inter()
 {
     count_1s++;
     if (count_1s >= 999)
         count_1s = 0;
 }
/****************************END OF FILE**********************/
