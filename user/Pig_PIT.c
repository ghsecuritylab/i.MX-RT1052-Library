# include "include.h"
# include "Pig_PIT.h"

void PIT_Init_us(pit_chnl_t PITn, long TimeInterval, bool enableRunInDebug)
{
    pit_config_t config;
    config.enableRunInDebug = enableRunInDebug;

    /* ��ʼ��PIT */
    PIT_Init(PIT, &config);
    /* ����PITn�ļ�ʱ���ΪTimeInterval΢�� */
    PIT_SetTimerPeriod(PIT, PITn, 75 * TimeInterval);
    /* ʹ��PIT0�ж� */
    PIT_EnableInterrupts(PIT, PITn, kPIT_TimerInterruptEnable);
    //���ȼ����� ��ռ���ȼ�0  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(PIT_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 2));
    /* ���ж���������ʹ�� PIT �ж�*/
    EnableIRQ(PIT_IRQN);
    /* ����PITn��ʱ */
    PIT_StartTimer(PIT, PITn);
}

extern void PIT0Inter();
void PIT_IRQHANDLER(void)
{
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_0)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*�û���������Ӵ���*/
        PIT0Inter();
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_1)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*�û���������Ӵ���*/
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_2)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*�û���������Ӵ���*/
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_3)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*�û���������Ӵ���*/
    }
}