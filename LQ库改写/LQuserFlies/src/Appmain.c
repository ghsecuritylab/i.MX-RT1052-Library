/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2018��11��13��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
================================================
���ؼ�����ģʽ��0010
����      ģʽ��0001
����      ģʽ��0010
------------------------------------------------
��JLINKV9/OB�����ϰ汾��������SDRAM��QSPI FLASH;
��LQDAPLINKV3��������SDRAM��QSPI FLASH;
------------------------------------------------
���ֺ�����δ��֤��ϣ�Ĭ�ϲ���OV7725ģ�������4.3������
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "AHRS.h"    //ʹ����̬����ʱ��Ҫ��Ӹ�ͷ�ļ�
//==========================================================================================  
//���Ժ�������        �����ڲ�ģ�鼰����           ���ܳ���Ӧ�ÿ���               ���״��
//----------------------------------------------------------------------------------------- 
//Test_LED();        //����GPIO�����               LED��ʾ���������              �����
//Test_OLED();       //����ģ��SPI����              OLEDģ�鹦��                   �����
//Test_GPIO_KEY();   //����GPIO����                 ������⹦��                   ����� 
//Test_GPIO_EXINT(); //����GPIO���뼰�ⲿ�ж�       �������жϼ�⹦��             �����     
//LQ_UART_Test();    //����UART���ж�               ������USBתTTL���Լ�ͨ��       �����
//Test_ADC();        //����ADC�ɼ�����              ��Ŵ���������Դ��ѹ���       �����
//Test_PIT();        //����PIT��ʱ�жϹ���                                         �����
//Test_PWM_nCH();    //����PWM                      PWM1��PWM2��PWM4 ����PWM��     �����
//Test_Servo();      //������PWM����                ���ֶ����������               �����
//Test_Motor();      //������PWM����                �����������                   �����  
//Test_OLED_Camera();//����OLED��MT9V034����        OLED��ʾ������������ͷ��ͼ     �����
//Test_ENCoder();    //�������ɼ�                                                  �����
//Text_TFT_480_272();//����480 * 272 ��                                            �����
//Test_TFT_Camera(); //����OV7725 or ���ۺ�TFT4.3 + ������                         �����
//Test_WDG();        //���Կ��Ź�����               ������                       �����
//Test_9AX();        //����I2C���������            �����Ǽ����ٶȹ���             ����� 
//Test_MPU6050();    //����I2C��6�������ǹ���       �����Ǽ����ٶȹ���             ����� 
//Test_EEPROM();     //����EEPROM����               �ڲ�EEPROM���ݴ���             ����� 
//-----------------------------------------------------------------------------------------  
//========================================================================================== 
/*      ����˵��                                                                    ʱ��
 *      1.�Ż� Test_OLED_Camera() 
 *      2.����systick��ʱ������ 
 *      3.�Ż���ʱ����                                                             2018/11/08
 *
 *      1.�Ż� PID
 *      2.�Ż� Scheduler                                                           2018/11/09
 *
 *      1.���Ӵ���+DMA ����                                                        
 *      2.����QTMRģ���PWM��������                                                2018/11/19
 *
 *      1.����QTMRģ���������������                                               
 *      2.�Ż���������ͷSCCB                                                       2018/11/21
 *
 *      1.�Ż���������ͷ֡������                                                   2018/11/27
 *
 *      1.�Ż�7725����ͷ֡������
 *      2.������λ����ʾͼ����                                                   2018/11/28
 *
 *      1.�������188*120�ֱ��ʲ���ͼ����
 *      2.���ͼ�񶶶�����                                                         2019/01/15
 */
void delayms(uint32_t ms)
{
    uint32_t i,j;
    
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 200000; j++)//600M--1ms
        __NOP();//asm("NOP");   
    }
    return ;
}
void delayus(uint32_t us) //��ʱ1us ʵ��Ϊ1.18us   ��ʱ10us ʵ��Ϊ10.02us   ��ʱ100us ʵ��Ϊ98.4us
{
    uint32_t i,j;
    
    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 290; j++)//600M--1us
        __NOP();//asm("NOP");   
    }
    return ;
} 

void DelayTest(void)
{
    _systime.delay_ms(900);
}

    uint32_t fullCameraBufferAddr;     
  unsigned char * image;
  uint64_t time;
  uint64_t now;
int main(void)
{        
  
    BOARD_ConfigMPU();                   /* ��ʼ���ڴ汣����Ԫ */      
    BOARD_InitSDRAMPins();               /* SDRAM��ʼ�� */
    BOARD_BootClockRUN();                /* ��ʼ��������ʱ�� */   
    BOARD_InitDEBUG_UARTPins();          //UART���ԿڹܽŸ��ó�ʼ�� 
    BOARD_InitDebugConsole();            //UART���Կڳ�ʼ�� ����ʹ�� PRINTF����          
    LED_Init();                          //��ʼ�����İ�Ϳ������ϵ�LED�ӿ�
    LQ_UART_Init(LPUART1, 115200);       //����1��ʼ�� ����ʹ�� printf����
    _systime.init();                     //����systick��ʱ��
    NVIC_SetPriorityGrouping(2);/*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ� 
                                 *1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ� 
                                 *3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
                                 */
    Test_PIT();

    /****************��ӡϵͳʱ��******************/
    PRINTF("\r\n");
    PRINTF("*****LQ_1052*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("Video:           %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
    PRINTF("USB1PLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
    PRINTF("USB1PLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
    PRINTF("USB1PLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
    PRINTF("USB1PLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
    PRINTF("USB1PLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
    
//-----------------------------------------------------------------------------------------  
//  ���Ժ���������ѭ����ÿ��ֻ�ܿ���һ�����ۺ�Ӧ�������е��ø�ģ���ʼ������
//-----------------------------------------------------------------------------------------
//    Test_ADC();          //����ADC����ʾ��TFT1.8��               ��Ŵ���������Դ��ѹ���  
//    Test_UART();         //����UART���ж�                      ������USBתTTL���Լ�ͨ��  
//    Test_LED();          //����GPIO�����     LED��ʾ���������             
//    Test_GPIO_KEY();     //����GPIO����+TFT1.8                   ������⹦�ܲ���ʾ��TFT1.8��              
//    Test_GPIO_ExInt();   //����GPIO���뼰�ⲿ�ж�              �������жϼ�⹦��    
//    Test_PIT();          //����PIT��ʱ�жϹ���  
//    Test_OLED();         //����OLED
//    Test_RNG();          //���������      
//    Test_9AX();          //������� 
//    Test_MPU6050();      //����I2C��6�������ǹ���              �����Ǽ����ٶȹ��� 
//    Test_ahrs();         //����������̬����   
//    Test_ANO_DT();       //������λ���۲����ݲ���  ��ʹ��DMAģʽ���뽫�궨��#define USE_DMA  0  
//    Test_UART_DMA();     //������λ���۲첨�Σ�ʹ��DMA���䣬��ռ��CPUʱ�䣬���������λ����������У׼�����İ�����
//    Test_PWM_nCH();      //����PWM                             PWM1�� ����PWM��  ��ʾ�����۲�
//    Test_QTMR_PWM();     //����QTMR ������PWM                  ʾ�����۲�
//    Test_QTMR_QD();      //����QTMR�������빦��
//    Test_Servo();        //���ֶ������      ע�� ����ʹ�ô˺���ʱ�������Ҫװ���̣���ֹ���������������ֵ����װ������������������ֵ
//    Test_Motor();        //ֱ������������ԣ�                  ������ȫ��������  ע�⣺ ���1�� ʹ�� L5  M5   ���2��ʹ��A8  A9  ���3��ʹ�� C9 B9  ���4��A10 J1
//    Test_ENCoder();      //�������ɼ�
//    Test_Camera_Reprot();  //��λ����ͼ  7725RGB565 / 7725 �Ҷ� / ���ۻҶ�
//    Test_SGP18_OV7725();   //����OV7725RGB ��TFT1.8
//    Test_SGP18_Camera();   //�������� Or 7725 ��ֵ�� + TFT1.8  ע�⣬7725ʹ�ûҶ�ͼ��ʱʹ��YUYV��ʽ ��Ҫ���� cameraConfig = { .pixelFormat = kVIDEO_PixelFormatYUYV }
//----------------------------------------------------------------------------------------- 
    PRINTF("������ %d \r\n", time);
    time = MeasureRunTime_ms(DelayTest);
//�ۺϲ���    
    while(1)
    {
        //LED_Init();
        //LQ_KEY_Init(); 
        TFTSPI_Init();                 //TFT1.8��ʼ��  
        TFTSPI_CLS(u16BLUE);           //����
        
        ///*GPIO �ⲿ�ж����ÿ�ʼ*/
        //CLOCK_EnableClock(kCLOCK_Iomuxc);                         // IO��ʱ��ʹ��
        //IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0U);       // ���ùܽŸ��ù���                          
        //IOMUXC_SetPinConfig(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0xF080);// ���ùܽ�
        //
        //gpio_pin_config_t GPIO_Input_Config =                     // GPIO��ʼ���ṹ��
        //{
        //    kGPIO_DigitalInput,                                   // GPIOΪ���뷽��
        //    1,                                                    // �ߵ�ƽ
        //    kGPIO_IntFallingEdge,                                 // �½��ش����ж�
        //};
        //GPIO_PinInit(GPIO5, 0, &GPIO_Input_Config);               // GPIO����ڣ��ж�
        //
        //GPIO_PortEnableInterrupts(GPIO5,1<<0);			          // GPIO5_00�ж�ʹ��
        ////���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
        //NVIC_SetPriority(GPIO5_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
        //
        //EnableIRQ(GPIO5_Combined_0_15_IRQn);			          //ʹ��GPIO5_0~15IO���ж�  
        //
      
        //IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0U);          //�ܽ�L6
        //IOMUXC_SetPinMux(IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02,0U);   //�ܽ�L7
        //IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_GPIO2_IO28,0U);           //�ܽ�D13
        //IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03,0U);        //�ܽ�G11
        //
        //IOMUXC_SetPinConfig(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0xF080); 
        //IOMUXC_SetPinConfig(IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02,0xF080);
        //IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_GPIO2_IO28,0xF080);
        //IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03,0xF080);
        //
        //gpio_pin_config_t GPIO_Config = {kGPIO_DigitalInput,    //GPIOΪ���뷽��
        //                           1,                    //�ߵ�ƽ
        //                           kGPIO_NoIntmode,      //�������ж�
        //                           };
        //
        //GPIO_PinInit(GPIO5,  0, &GPIO_Config);        // GPIO����ڣ����ж�
        //GPIO_PinInit(GPIO5, 02, &GPIO_Config);        // GPIO����ڣ����ж�
        //GPIO_PinInit(GPIO1, 03, &GPIO_Config);        // GPIO����ڣ����ж�
        //GPIO_PinInit(GPIO2, 28, &GPIO_Config);        // GPIO����ڣ����ж�
        //
        //LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B,  200);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz //Ĭ����M3  M4��
        //LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA_B, 12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     A8 A9
        //LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA_B, 12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     B9 C9
        //LQ_PWM_Init(PWM2, kPWM_Module_2, kPWM_PwmB,   12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     A10
        //LQ_PWM_Init(PWM1, kPWM_Module_1, kPWM_PwmA,   12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     J1
        //LQ_PWM_Init(PWM1, kPWM_Module_3, kPWM_PwmA_B, 12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     L5 M5
        //
        //LQ_ENC_Init(ENC1);   //���������ʼ��
        //LQ_ENC_Init(ENC2);
        //LQ_ENC_Init(ENC3);   //���������ʼ��
        //LQ_ENC_Init(ENC4);

        LQ_Camera_Init();
        short velocity1, velocity2, velocity3, velocity4;
        uint16_t color = 0;
        while(1)
        {                                          
         
          
          
        }        
    }
    
    
    
    
    
    
}
