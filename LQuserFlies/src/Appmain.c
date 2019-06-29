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
///<summary>�ٶȻ�����</summary>
float P_Set[4] = {22, 19, 22, 21};//{22, 19, 22, 21}{35, 39, 40, 39}
float D_Set[4] = {5, 5, 5, 4};//{5, 5, 5, 4}{13, 19, 20, 19}
float I_Set[4] = {5, 5, 5, 5};//{5, 5, 5, 5}{5, 9, 9, 9}
float DeadBand_Set[4] = {0,0,0,0};//{739-22, 659-20, 743-23, 720-21}
float I_limit = 8000;
float Max_output = 9500;
///<summary>���򻷲���</summary>
float P_Direction_Set_init= 3.6;
float D_Direction_Set_init= 0.020;
float Max_output_Direction= 250;
///<summary>��ʱ����������</summary>
volatile int16_t PIT0_Flag = 0;
long Speed_watch[4];
uint8 Series_deviation_received_front=0;
uint8 Series_distance_received_front=0;
uint8 Series_deviation_received_back=0;
uint8 Series_distance_received_back=0;
uint8 Series_deviation_received=0;
uint8 Series_distance_received=0;
uint8 distance_deviation_relevance_left[31]={94,94,94,94,100,100,105,108,111,114,118,122,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126};
uint8 distance_deviation_relevance_right[31]={94,94,94,94,94,94,94,93,92,91,90,89,88,85,83,78,68,64,60,57,55,51,49,46,42,39,38,36,32,32,32};
extern int direction_flag;
extern uint8 Front_Distance_ReceiveBuff[3];
extern uint8 Back_Distance_ReceiveBuff[3];
extern uint8 Series_ReceiveBuff[4];
extern float ControlValue_Closeloop[4];
extern long SpeedCount[4];
extern long Speed_get[4];
extern struct PIDControl Car_Speed_Rotate;//׷��ת��ջ�


uint64_t TestStartTime_us = 0;
uint64_t TestEndTime_us = 0;

void InitAll();
uint64_t TimeTest_us = 0;
void RunTimeTest()
{
      SEND(ControlValue_Closeloop[0], ControlValue_Closeloop[1], ControlValue_Closeloop[2], ControlValue_Closeloop[3]);

      //TFT_showfloat(0,0,(float)TimeTest_us,7,0,BLACK,WHITE);
}
///<summary>������</summary>
float Battery_V = 0;
int main(void)
{         
    InitAll();
    
    _systime.delay_ms(200);

    //TFTSPI_CLS(u16WHITE);
    while(1)
    {
      /*ɨ�谴����־λ*/
      for (int i = 0; i < 3; i++)
      {
          if (Button[i] == Press)
          {
              ButtonOnceBuffFlag[i] = 1;
          }
          if ((Button[i] == NotPress) && (ButtonOnceBuffFlag[i] == 1))
          {
              ButtonOnceFlag[i] = 1;
              ButtonOnceBuffFlag[i] = 0;
          }
      }
      /*�����л��Ƿ�TFT��ʾ*/
      if (ButtonOnceFlag[2] == 1)
      {
          ButtonOnceFlag[2] = 0;
          /* �ڴ˱�д���°���3�Ĵ������ */
          if(Flag_TFTShow)
            dsp_single_colour(WHITE);
          Flag_TFTShow = !Flag_TFTShow;
          if(Flag_TFTShow)
            dsp_single_colour(WHITE);
      }
      //SEND(Speed_watch[0],Speed_watch[1],Speed_watch[2],Speed_watch[3]);

      TFT_showfloat(1,0,Distance_Meassured, 3,2,BLACK,WHITE);
      Get_Gyro(&GYRO_OriginData);//z��Ϊ�ش���,��ʱ��Ϊ�����򣬴���������D��ʱ��Ϊ����
      //_systime.delay_ms(100);
    }             
}


void InitAll()
{
    BOARD_ConfigMPU();                   /* ��ʼ���ڴ汣����Ԫ */
    BOARD_InitSDRAMPins();               /* SDRAM��ʼ�� */
    BOARD_BootClockRUN();                /* ��ʼ��������ʱ�� */
    BOARD_InitDEBUG_UARTPins();          //UART���ԿڹܽŸ��ó�ʼ�� 
    BOARD_InitDebugConsole();            //UART���Կڳ�ʼ�� ����ʹ�� PRINTF����          
    //LED_Init();                          //��ʼ�����İ�Ϳ������ϵ�LED�ӿ�
    UART_DMA_Init();       //����1��ʼ�� ����ʹ�� printf����
    _systime.init();                     //����systick��ʱ��
    NVIC_SetPriorityGrouping(2);/*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ�
                                *1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ�
                                *3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
                                */
    Series_Receive_init();
    Motor_init();
    MPU6050_Init();
    TFT_init();
    RemoteData_init();   
    ButtonMenu();
    UltrasoundInit();
    
    PID_Speedloop_init(P_Set, D_Set, I_Set, I_limit, Max_output, DeadBand_Set);
    PID_locationloop_init(P_Direction_Set_init, 0, 0, 0, Max_output_Direction, 0);//λ�û�����     

    EncoderMeasure_Init();
    RemoteInit();
    //camera_init_1();
    
    LQ_PIT_Init(kPIT_Chnl_0, 3000);//3000us
}
///<summary>��ʱ������</summary>
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
            TestStartTime_us = _systime.get_time_us();
    
            SpeedClean();
            int i = 0;
            for (i = 0; i < 4; i++)
            {
                GetSpeed(i);
                Speed_get[i] += SpeedCount[i];
            }
            SendCMDToUltrasound();//�������������
            //ң�ز��Գ���
            #ifdef Remote_UseDigitalReceive
            SetSpeed_FromRemote(RunMode);//������
            #else
            SetSpeed_FromRemote_Analog();//ģ����
            #endif
            //SEND(ControlValue_Closeloop[0], ControlValue_Closeloop[1], ControlValue_Closeloop[2], ControlValue_Closeloop[3]);
            //����ͨ��//
            Series_deviation_received_front = Series_ReceiveBuff[0];
            Series_distance_received_front = One_loop_bubblesort(Front_Distance_ReceiveBuff, 3);
            Series_deviation_received_back= Series_ReceiveBuff[2];
            Series_distance_received_back = One_loop_bubblesort(Back_Distance_ReceiveBuff, 3);
            //ת���˶�����//
//            if(direction_flag == 1)
//            {
//            
//              if(Series_deviation_received_front != 0)
//              {
//                Series_deviation_received = Series_deviation_received_front;
//                Series_distance_received = Series_distance_received_front;
//                direction_flag = 1;
//              }
//              else
//              {
//                Series_deviation_received = Series_deviation_received_back;
//                Series_distance_received = Series_distance_received_back;
//                direction_flag = -1;
//              }
//            }
//            else if(direction_flag == -1)
//            {            
//              if(Series_deviation_received_back != 0)
//              {
//                Series_deviation_received = Series_deviation_received_back;
//                Series_distance_received = Series_distance_received_back;
//                direction_flag = -1;
//              }
//              else
//              {
//                Series_deviation_received = Series_deviation_received_front;
//                Series_distance_received = Series_distance_received_front;
//                direction_flag = 1;
//              } 
//            }
            //ǰ��·��//
            if(Series_distance_received>30)
              Series_distance_received=30;
//            if(Series_deviation_received  < 94)
//              PID_SetTarget(&Car_Speed_Rotate, distance_deviation_relevance_right[Series_distance_received]-11);
//            else 
              PID_SetTarget(&Car_Speed_Rotate,distance_deviation_relevance_left[Series_distance_received]);
            //���������//
            //Series_Control(Series_deviation_received);
            //���ת�����//
//            LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[3], 2000, 0);
            //�������۲�//
            int j = 0;
            for (j = 0; j < 4; j++)
            {
                Speed_watch[j] = Speed_get[j];
                Speed_get[j] = 0;//�ٶȼ�����
            }
#ifndef Remote_UseDigitalReceive
            SendRemoteCMDData();

#endif
            TestEndTime_us = _systime.get_time_us();
            TimeTest_us = TestEndTime_us-TestStartTime_us;
            PIT0_Flag = 0;
        }
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
