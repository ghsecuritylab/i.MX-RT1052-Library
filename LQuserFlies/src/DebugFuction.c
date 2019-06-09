/*!
* @file       DebugFuction.c
* @brief      ���ڵ����õĺ�����
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-26
* @to do
*             
*/
# include "include.h"
/*************************************************************/
/*****************����ʾ��������************/
/*************************************************************/
float OutData[4] = { 0 };//����Ὣ�����е��ĸ������͸�����ʾ����
//float temp = 0.0;
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
      int temp[4] = {0};
      unsigned int temp1[4] = {0};
      unsigned char databuf[10] = {0};
      unsigned char i;
      unsigned short CRC16 = 0;
      for(i=0;i<4;i++)
      {
        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];
    
    }

    for(i=0;i<4;i++) 
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

    UART_Put_Buff(CRC_Uart_Port, databuf, 10);
}
/// <summary>
///������ʾ��������a,b,c,dȡ��֮���ֵ
///<para>example:  SEND(a,b,c,d);</para>
///</summary>
/// <param name="a">�����͵ı���1</param>
/// <param name="b">�����͵ı���2</param>
/// <param name="c">�����͵ı���3</param>
/// <param name="d">�����͵ı���4</param>
void SEND(float a,float b,float c,float d)
{       
 OutData[0] = a;                   
 OutData[1] = b;                    
 OutData[2] = c;                
 OutData[3] = d;
 OutPut_Data();
}
/*************************************************************/
/*****************���ݲɼ�����************/
/*************************************************************/
/*===================================================================
���ܣ����ڷ�����������Matlab���ݴ���
===================================================================*/
void DATA_SEND(long num)
{
    int weishu = 0;
    long num_buff = num;
    long buff = 1;
    int index = 0;

    if (num < 0)
    {
        UART_Put_Buff(CRC_Uart_Port, "-", 1);
        num = -num;
    }

    if (num_buff == 0)
    {
        weishu = 1;
        buff = 10;
    }

    while (num_buff != 0)
    {
        num_buff /= 10;
        weishu++;
        buff *= 10;
    }
    buff /= 10;

    for (index = 0; index < weishu; index++)
    {
        //UART_Put_Buff(CRC_Uart_Port, ((num / buff) % 10) + '0', 1);

        buff /= 10;
    }
    UART_Put_Buff(CRC_Uart_Port, " ", 1);
}

/*************************************************************/
/*****************ң�ز���************/
/*************************************************************/
/// <summary>
///��ʼ��ң�����Ĵ���
///</summary>
void RemoteInit()
{
    UART_Init(Remote_Uart_Port, 9600);
}
/// <summary>
///��ʼ��ģ��ң�����Ľ�������
///</summary>
ReceiveCMDData RemoteData;
void RemoteData_init(void)
{
    RemoteData.Left_Y = 126;
    RemoteData.Left_X = 126;
    RemoteData.Right_Y = 126;
    RemoteData.Right_X = 126;
  
}
int ReceiveIndex = 0;
char ReceiveBuff[3] = {0};
unsigned char StartReceive = 0;
#ifdef Remote_UseDigitalReceive
RemoteCMDMode RunMode;//ң��ģʽ
/// <summary>
///����ң��ָ�����Ӧ�����Ӧ�Ĵ����ж���
///</summary>
void ReceiveCMD_Remote(void)
{
    char buff = 0;
    buff = UART_Get_Char(Remote_Uart_Port);
    if (buff == 0xFF && StartReceive == 0)
    {
        StartReceive = 1;
        return;
    }
    else
    {
      if(StartReceive == 0)
       ReceiveIndex = 0;
    }
    if (StartReceive == 1)
    {
        if (ReceiveIndex < 3)
        {
            ReceiveBuff[ReceiveIndex] = buff;
            ReceiveIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
                StartReceive = 0;
                ReceiveIndex = 0;
                    if (ReceiveBuff[0] == 0x00)//��ҡ��
                    {
                        switch (ReceiveBuff[1])
                        {
                            case 0x00:
                            RunMode = Left_Return0;
                            break;
                            case 0x01:
                            RunMode = Left_Up;
                            break;
                            case 0x02:
                            RunMode = Left_Down;
                            break;
                            case 0x03:
                            RunMode = Left_Left;
                            break;
                            case 0x04:
                            RunMode = Left_Right;
                            break;
                            default:
                            break;
                        }
                    }
                    else if (ReceiveBuff[0] == 0x35)//��ҡ��
                    {
                        switch (ReceiveBuff[1])
                        {
                            case 0x00:
                            RunMode = Right_Return0;
                            break;
                            case 0x01:
                            RunMode = Right_Up;
                            break;
                            case 0x02:
                            RunMode = Right_Down;
                            break;
                            case 0x03:
                            RunMode = Right_Left;
                            break;
                            case 0x04:
                            RunMode = Right_Right;
                            break;
                            default:
                            break;
                        }
                    }
                    else if (ReceiveBuff[0] == 0xAA)
                    {
                        RunMode = Start;
                    }
                }
            }
        }

}
#else
/// <summary>
///����ң��ָ�����ģ����
///</summary>
Remote_State Remote_CMD_ReceiveStatus = Sleep;
long count_error_left = 0;
long count_error_right = 0;

void SendRemoteCMDData(void)
{
    if (Remote_CMD_ReceiveStatus == Sleep)
    {
        Remote_CMD_ReceiveStatus = SendLeftCMD;
        Remote_CMD_ReceiveStatus = ReceivingLeftCMD;
        UART_Put_Char(Remote_Uart_Port, 0xBB);
    }
    if(Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
    {
      count_error_left++;
      count_error_right = 0;
    }
    else if(Remote_CMD_ReceiveStatus == ReceivingRightCMD)
    {
      count_error_right++; 
      count_error_left = 0;
    }
    else
    {
      count_error_left = 0;
      count_error_right = 0;
    }
    
    if(count_error_left >= 500 || count_error_right >= 500)
    {
      RemoteData.Left_Y = 127;
      RemoteData.Left_X = 127;
      RemoteData.Right_Y = 127;
      RemoteData.Right_X = 127;
      Remote_CMD_ReceiveStatus = Sleep;
      count_error_left = 0;
      count_error_right = 0;
    }
}
/// <summary>
///����ң��ָ�����Ӧ�����Ӧ�Ĵ����ж���
///</summary>
void ReceiveCMD_Remote()
{
    char buff = 0;
    buff = UART_Get_Char(Remote_Uart_Port);
    if (buff == 0xFF && StartReceive == 0)
    {
        StartReceive = 1;
        return;
    }
    else
    {
        if (StartReceive == 0)
            ReceiveIndex = 0;
    }
    if (StartReceive == 1)
    {
        if (ReceiveIndex < 3)
        {
            ReceiveBuff[ReceiveIndex] = buff;
            ReceiveIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
                StartReceive = 0;
                ReceiveIndex = 0;
                if (ReceiveBuff[0] == 0xBB)//��ҡ��
                {
                    if (Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
                    {
                        RemoteData.Left_Y = ReceiveBuff[1];
                        RemoteData.Left_X = ReceiveBuff[2];
                        Remote_CMD_ReceiveStatus = ReceivedLeftCMD;
                        Remote_CMD_ReceiveStatus = ReceivingRightCMD;
                        UART_Put_Char(Remote_Uart_Port, 0xCC);          
                    }
                    
                }
                else if (ReceiveBuff[0] == 0xCC)//��ҡ��
                {
                    if (Remote_CMD_ReceiveStatus == ReceivingRightCMD)
                    {
                        RemoteData.Right_Y = ReceiveBuff[1];
                        RemoteData.Right_X = ReceiveBuff[2];
                        Remote_CMD_ReceiveStatus = ReceivedRightCMD;
                        Remote_CMD_ReceiveStatus = Sleep;
                    }
                }
                else
                {
                        RemoteData.Left_Y = 127;
                        RemoteData.Left_X = 127;
                        RemoteData.Right_Y = 127;
                        RemoteData.Right_X = 127;
                  Remote_CMD_ReceiveStatus = Sleep;
                }
            }
        }
    }

}
#endif

/////<summary>������ʼ��</summary>
//void ButtonInit()
//{
//    EXTI_Init(PTE, 10, either);
//    EXTI_Init(PTE, 11, either);
//    EXTI_Init(PTE, 12, either);
//    LCD_P6x8Str(0,0,"Please Press!");
//}
//ButtonStatus Button[3] = { NotPress, NotPress, NotPress };//PTE12,PTE11,PTE10


///<summary>���ͨ��</summary>
uint8 SeriesReceive = 0;
uint8 SeriesIndex = 0;
uint8 Series_ReceiveBuff[4] = { 0 };
uint8 Front_Distance_ReceiveBuff[5] = { 0 };
uint8 Back_Distance_ReceiveBuff[5] = { 0 };
void Series_RX(void)
{
    uint8 buff = 0;
    buff = UART_Get_Char(Series_Uart_Port);
    if (buff == 0xFF && SeriesReceive == 0)
    {
        SeriesReceive = 1;
        return;
    }
    else
    {
      if(SeriesReceive == 0)
       SeriesIndex = 0;
    }
    if (SeriesReceive == 1)
    {
        if (SeriesIndex < 4)
        {
          Series_ReceiveBuff[SeriesIndex] = buff;
          SeriesIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
              FIFO(Front_Distance_ReceiveBuff, 5, Series_ReceiveBuff[1]);
              FIFO(Back_Distance_ReceiveBuff, 5, Series_ReceiveBuff[3]);
              SeriesReceive = 0;
              SeriesIndex = 0;
            }
        }
    }
}
///<summary>����ͨ�ų�ʼ��</summary>
void Series_Receive_init(void)
{
    UART_Init(Series_Uart_Port, 115200);
}


/////<summary>����ɨ���ж�</summary>
//void ButtonScan_interrupt()
//{
//    //Key1
//    int n = 10;
//    u8 keybuff = 0;
//    if ((PORTE_ISFR & (1 << n)))
//    {
//        PORTE_ISFR |= (1 << n);
//        //�û���������ж��ڳ���
//        keybuff = GPIO_Get(PTE10);
//        if (keybuff == 0)
//        {
//            Button[2] = Press;
//        }
//        else
//        {
//            Button[2] = NotPress;
//        }
//        TFT_showint8(0, 2, Button[2], BLACK, WHITE);
//    }
//    n = 11;
//    if ((PORTE_ISFR & (1 << n)))
//    {
//        PORTE_ISFR |= (1 << n);
//        //�û���������ж��ڳ���
//        keybuff = GPIO_Get(PTE11);
//        if (keybuff == 0)
//        {
//            Button[1] = Press;
//        }
//        else
//        {
//            Button[1] = NotPress;
//        }
//        TFT_showint8(0, 1, Button[1], BLACK, WHITE);
//    }
//    n = 12;
//    if ((PORTE_ISFR & (1 << n)))
//    {
//        PORTE_ISFR |= (1 << n);
//        //�û���������ж��ڳ���
//        keybuff = GPIO_Get(PTE12);
//        if (keybuff == 0)
//        {
//            Button[0] = Press;
//        }
//        else
//        {
//            Button[0] = NotPress;
//        }
//        TFT_showint8(0, 0, Button[0], BLACK, WHITE);
//    }
//}
//
//int ButtonOnceBuffFlag[3] = { 0 };//��������һ�λ����־
//int ButtonOnceFlag[3] = { 0 };//��������һ�εı�־
//int QuitSetFlag = 0;
extern float Battery_V;
///// <summary>
/////�����˵��������ڲ����趨�ȹ��ܣ���������������Ҫ����ǰ
/////<para>ע��һ��Ҫ����TFT��ʼ���������������жϵ�ģ���ʼ����������������������</para>
/////</summary>
//void ButtonMenu()
//{
//    ButtonInit();
//    
//    EnableInterrupts;
//    while (1)
//    {
//        for (int i = 0; i < 3; i++)
//        {
//            if (Button[i] == Press)
//            {
//                ButtonOnceBuffFlag[i] = 1;
//            }
//            if ((Button[i] == NotPress) && (ButtonOnceBuffFlag[i] == 1))
//            {
//                ButtonOnceFlag[i] = 1;
//                ButtonOnceBuffFlag[i] = 0;
//            }
//        }
//
//        if (ButtonOnceFlag[0] == 1)
//        {
//            ButtonOnceFlag[0] = 0;
//            /* �ڴ˱�д���°���1�Ĵ������ */
//
//        }
//        if (ButtonOnceFlag[1] == 1)
//        {
//            ButtonOnceFlag[0] = 0;
//            /* �ڴ˱�д���°���2�Ĵ������ */
//
//        }
//        if (ButtonOnceFlag[2] == 1)
//        {
//            ButtonOnceFlag[0] = 0;
//            /* �ڴ˱�д���°���3�Ĵ������ */
//            QuitSetFlag = 1;
//        }
//        if (QuitSetFlag == 1)
//        {
//            dsp_single_colour(WHITE);
//            TFT_showstr(0, 0, "QuitButtonSet", RED, WHITE);
//            DisableInterrupts;
//            break;
//        }
//        Battery_V = GetBatteryVoltage(7.5);
//        LCD_ShowBatteryVoltage(0,1,Battery_V);
//    }
//}

///<summary>�����ɼ�</summary>
void BatteryVoltageCollect_Init(int IfUseBeep)
{
    LQADC_Init(BatteryCollectADC);
}
float GetBatteryVoltage(float HintVoltage)
{
    uint16_t adc_value = 0;
    adc_value = ReadADC(BatteryCollectADC, BatteryCollectADCChn);

    float result = adc_value*7.35 / 13490.0;

    return result;
}
//void LCD_ShowBatteryVoltage(unsigned char x, unsigned char y, float num)
//{
//    unsigned char tmp[7], i;
//    tmp[6] = 0;
//    tmp[5] = 'V';
//    num *= 100;
//    if (num>0)
//    {
//        tmp[0] = '+';
//        tmp[4] = (unsigned char)((int)num % 10 + 0x30);
//        tmp[3] = (unsigned char)((int)num / 10 % 10 + 0x30);
//        tmp[2] = '.';
//        tmp[1] = (unsigned char)((int)num / 100 % 10 + 0x30);
//    }
//    else
//    {
//        tmp[0] = '-';
//        num = -num;
//        tmp[4] = (unsigned char)((int)num % 10 + 0x30);
//        tmp[3] = (unsigned char)((int)num / 10 % 10 + 0x30);
//        tmp[2] = '.';
//        tmp[1] = (unsigned char)((int)num / 100 % 10 + 0x30);
//    }
//    LCD_P6x8Str(x, y, tmp);
//}

///<summary>FIFO</summary>
void FIFO(uint8 *head, uint8 depth, uint8 num)
{
  int k;
  for( k = 0;k < depth-1;k++)
  {
    *(head+k) = *(head+k+1);
  }
  *(head+k) = num;
  return;
}

void FIFO_Clean(uint8 *head, uint8 depth)
{
    int i = 0;
    for (i = 0; i < depth; i++)
        *(head+i) = 0;
}


uint8 One_loop_bubblesort(uint8 *lis, uint8 depth)
{
  uint8 biggest = 0;
  uint8 *const arry = (uint8 *)malloc(sizeof(uint8)*depth);//��ϵͳ�����ڴ棬����Ϊ���ݽ������б����
  uint8 temp_exchange = 0;
	for (int i = 0; i <= depth-1; i++)
	{
		arry[i] = lis[i];
	}
	for (int i = 0; i < depth-1; i++)
	{
                if (arry[i] > arry[i + 1])
                {
                        temp_exchange = arry[i + 1];
                        arry[i + 1] = arry[i];
                        arry[i] = temp_exchange;
                }
	}
                 biggest = arry[depth-1];
        return biggest;
}
