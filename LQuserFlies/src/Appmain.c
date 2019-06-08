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
//#include "AHRS.h"    //ʹ����̬����ʱ��Ҫ��Ӹ�ͷ�ļ�

uint32_t fullCameraBufferAddr;     
unsigned char * image;
uint64_t time;
uint64_t now;

void Camera1PinInit();
void Camera2PinInit();

int16_t Speed[4] = { 0 };
extern float ControlValue_Closeloop[4];
extern long SpeedCount[4];
extern long Speed_get[4];

///<summary>�ٶȻ�����</summary>
float P_Set[4] = {22, 19, 10, 10};//{22, 19, 22, 21}{35, 39, 40, 39}
float D_Set[4] = {0, 0, 0, 0};//{5, 5, 5, 4}{13, 19, 20, 19}
float I_Set[4] = {0,0,0,0};//{5, 5, 5, 5}{5, 9, 9, 9}
float DeadBand_Set[4] = {0,0,0,0};//{739-22, 659-20, 743-23, 720-21}
float I_limit = 8000;
float Max_output = 9500;

extern float ControlValue_Closeloop[4];
extern long SpeedCount[4];



short aacx,aacy,aacz;	        //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;        //������ԭʼ���� 
float GX = 0;

void InitAll();
uint64_t TimeTest_us = 0;
void RunTimeTest()
{
    //TFT_showint8(0, 0, 251, BLACK, WHITE);
    //dsp_single_colour(BLACK);
  //Get_Gyro(&GYRO_OriginData);
  //Uart_SendString_DMA("Hello!",6);
  SEND(12,1321,2354,20.2321);
  //GetSpeed(0);
}

float Angle_Z = 0;
float dt = 0.0551;
int main(void)
{         
    InitAll();
    //PID_Speedloop_init(P_Set, D_Set, I_Set, I_limit, Max_output, DeadBand_Set);
    
    _systime.delay_ms(200);

    //TFTSPI_CLS(u16WHITE);
    while(1)
    {
      	//�õ����ٶȴ���������
      TimeTest_us = MeasureRunTime_us(&RunTimeTest);
      if(GYRO_OriginData.Z >= 1 || GYRO_OriginData.Z <= -1)
        Angle_Z += 0.07 * GYRO_OriginData.Z * dt;
      
      //SendRemoteCMDData();
        //LQ_PWMA_B_SetDuty(PWM1, PWMChannel_Use_W1, 0, (uint16_t)(ControlValue[i]));
        //TimeTest_us = MeasureRunTime_us(&RunTimeTest);
        _systime.delay_ms(5);
//        LQ_PWMA_B_SetDuty(PWMType_Use1, Wheels_PWMChannel[0], 1000, 2600);
//        LQ_PWMA_B_SetDuty(PWMType_Use2, Wheels_PWMChannel[1], 3000, 4600);
//        LQ_PWMA_B_SetDuty(PWMType_Use3, Wheels_PWMChannel[2], 5000, 6600);
//        LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[3], 7000, 8600);
    }             
}
float adc_V = 0;

void InitAll()
{
    BOARD_ConfigMPU();                   /* ��ʼ���ڴ汣����Ԫ */
    BOARD_InitSDRAMPins();               /* SDRAM��ʼ�� */
    BOARD_BootClockRUN();                /* ��ʼ��������ʱ�� */
    BOARD_InitDEBUG_UARTPins();          //UART���ԿڹܽŸ��ó�ʼ�� 
    BOARD_InitDebugConsole();            //UART���Կڳ�ʼ�� ����ʹ�� PRINTF����          
    //LED_Init();                          //��ʼ�����İ�Ϳ������ϵ�LED�ӿ�
    //LQ_UART_Init(LPUART1, 115200);       //����1��ʼ�� ����ʹ�� printf����
    UART_DMA_Init();
    _systime.init();                     //����systick��ʱ��
    NVIC_SetPriorityGrouping(2);/*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ�
                                *1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ�
                                *3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
                                */
    //TFTSPI_Init();                 //TFT1.8��ʼ��
    MPU6050_Init();
    //Motor_init();
    //BatteryVoltageCollect_Init(1);
    //adc_V = GetBatteryVoltage(0);
    //EncoderMeasure_Init();
    //RemoteInit();
    //camera_init_1();
    //Series_Receive_init();
    //LQ_PIT_Init(kPIT_Chnl_0, 3000);//3000us
}
volatile int16_t PIT0_Flag = 0;
long Speed_watch[4];
extern struct PIDControl Car_Speed_Rotate;
extern RemoteCMDMode RunMode;
extern float Rotate_sp;
extern uint8 length[5];
void PIT_IRQHandler(void)
{
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_0)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*��������Ӵ���*/
        if (PIT0_Flag != 4)
        {
            SpeedClean();
            int i = 0;
            for (i = 0; i < 4; i++)
            {
                GetSpeed(i);
                Speed_get[i] += SpeedCount[i];
            }
            PIT0_Flag += 1;
        }
        else
        {

            //ң�ز��Գ���
            #ifdef Remote_UseDigitalReceive
            SetSpeed_FromRemote(RunMode);//������
            #else
            SetSpeed_FromRemote_Analog();//ģ����
            #endif
            //SEND(ControlValue_Closeloop[0], ControlValue_Closeloop[1], ControlValue_Closeloop[2], ControlValue_Closeloop[3]);
            ////����ͨ��//
            //Series_deviation_received = Series_ReceiveBuff[0];
            //Series_distance_received = One_loop_bubblesort(length);
            //�˶�����//
            //if (Series_distance_received>18)
            //    yaw_angel = 40;
            //else
            //    yaw_angel = 0;
            //if (Series_deviation_received  < 94)
            //    PID_SetTarget(&Car_Speed_Rotate, 94 - yaw_angel);
            //else
            //    PID_SetTarget(&Car_Speed_Rotate, 94 + yaw_angel);

            //Series_Control(Series_deviation_received);

            //�������۲�
            int j = 0;
            for (j = 0; j < 4; j++)
            {
                Speed_watch[j] = Speed_get[j];
                Speed_get[j] = 0;//�ٶȼ�����
            }
            //SEND(Speed_watch[0],Speed_watch[1],Speed_watch[2],Speed_watch[3]);
            PIT0_Flag = 0;
        }
        SendRemoteCMDData();
    }
}
void Camera1PinInit()
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
    CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_00_GPIO2_IO00,           /* GPIO_B0_00 is configured as GPIO2_IO00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_01_GPIO2_IO01,           /* GPIO_B0_01 is configured as GPIO2_IO01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_02_GPIO2_IO02,           /* GPIO_B0_02 is configured as GPIO2_IO02 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_03_GPIO2_IO03,           /* GPIO_B0_03 is configured as GPIO2_IO03 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_04_GPIO2_IO04,           /* GPIO_B0_04 is configured as GPIO2_IO04 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_05_GPIO2_IO05,           /* GPIO_B0_05 is configured as GPIO2_IO05 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_06_GPIO2_IO06,           /* GPIO_B0_06 is configured as GPIO2_IO06 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_07_GPIO2_IO07,           /* GPIO_B0_07 is configured as GPIO2_IO07 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_41_GPIO3_IO27,          /* GPIO_EMC_41 is configured as GPIO3_IO27 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_01_GPIO3_IO13,        /* GPIO_SD_B0_01 is configured as GPIO3_IO13 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,     /* GPIO_SD_B0_03 is configured as XBAR1_INOUT07 */
        0U);

    IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
        (~(IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
        | IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL(0x01U) /* QTIMER1 TMR0 input select: input from XBAR */
        | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U) /* IOMUXC XBAR_INOUT6 function direction select: XBAR_INOUT as input */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U) /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U) /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U) /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
        );

    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputQtimer1Tmr0Input);

}

void Camera2PinInit()
{
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_00_GPIO2_IO16,           /* GPIO_B1_00 is configured as GPIO2_IO16 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_01_GPIO2_IO17,           /* GPIO_B1_01 is configured as GPIO2_IO17 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_02_GPIO2_IO18,           /* GPIO_B1_02 is configured as GPIO2_IO18 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_03_GPIO2_IO19,           /* GPIO_B1_03 is configured as GPIO2_IO19 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_04_GPIO2_IO20,           /* GPIO_B1_04 is configured as GPIO2_IO20 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_05_GPIO2_IO21,           /* GPIO_B1_05 is configured as GPIO2_IO21 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_06_GPIO2_IO22,           /* GPIO_B1_06 is configured as GPIO2_IO22 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_07_GPIO2_IO23,           /* GPIO_B1_07 is configured as GPIO2_IO23 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02,   /* PMIC_STBY_REQ is configured as GPIO5_IO02 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_15_GPIO2_IO15,           /* GPIO_B0_15 is configured as GPIO2_IO15 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,        /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
        0U);

    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputQtimer2Tmr0Input);
}
