# include "include.h"

Enum_UltrasoundMeassure UltrasoundMeassure;
/// <summary>
///��ʼ��������ģ��
///</summary>
void UltrasoundInit()
{
    UART_Init(Ultrasound_Uart_Port, 9600);
    LQ_UART_PutChar(Ultrasound_Uart_Port, 0xe8);//ģ���ַ
    LQ_UART_PutChar(Ultrasound_Uart_Port, 0x02);//�Ĵ���2
    LQ_UART_PutChar(Ultrasound_Uart_Port, 0x75);//6����Դ����
    
    UltrasoundMeassure = Meassured;
}
/// <summary>
///��������ģ�鷢�Ͳ�ѯָ��
///</summary>
void SendCMDToUltrasound()
{
    if (UltrasoundMeassure == Meassured)
    {
        UltrasoundMeassure = Meassuring;
        LQ_UART_PutChar(Ultrasound_Uart_Port, 0xe8);//ģ���ַ
        LQ_UART_PutChar(Ultrasound_Uart_Port, 0x02);//�Ĵ���2
        LQ_UART_PutChar(Ultrasound_Uart_Port, MaxMeassureRangeSetCMD_100cm);//д���ָ��
    }
}

uint8_t DisReceiveIndex = 0;
float Distance_Meassured = 0;//��λcm
uint8_t ReceiveBuff_Dis[2];
uint16_t MeasureTime_Dis = 0;
/// <summary>
///���ܾ������ݷ�����Ҫ�ŵ���Ӧ�����ж�����
///</summary>
void ReceiveDistance()
{
    uint8_t buff = 0;

    if (UltrasoundMeassure == Meassuring)
    {
        ReceiveBuff_Dis[DisReceiveIndex] = UART_Get_Char(Ultrasound_Uart_Port);
        DisReceiveIndex++;

        if (DisReceiveIndex == 2)
        {
            DisReceiveIndex = 0;
            UltrasoundMeassure = Meassured;
            MeasureTime_Dis = ReceiveBuff_Dis[0] * 256 + ReceiveBuff_Dis[0];
            Distance_Meassured = MeasureTime_Dis * 0.0173;
        }
    }
    else
    {
      buff = UART_Get_Char(Ultrasound_Uart_Port);
    }

}
