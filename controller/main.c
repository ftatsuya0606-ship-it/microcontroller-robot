#include "iodefine.h"


unsigned short val;


/* 100msフラグ */
volatile unsigned char send_flag = 0;



/* SCI5送信 */
void sci5_send(char data)
{
    while(SCI5.SSR.BIT.TDRE == 0)
    {
        ;
    }

    SCI5.TDR = data;

    while(SCI5.SSR.BIT.TEND == 0)
    {
        ;
    }
}



/* 停止 */
void stop_send(void)
{
    sci5_send('0');
}


/* 前進 */
void forward_send(void)
{
    sci5_send('1');
}


/* 左 */
void left_send(void)
{
    sci5_send('2');
}


/* 右 */
void right_send(void)
{
    sci5_send('3');
}


/* 後退 */
void reverse_send(void)
{
    sci5_send('4');
}



/* ジョイスティック */
void joystick_control(void)
{

    val = S12AD.ADDR0;


    if(val < 500)
    {
        left_send();
    }

    else if(val > 3000)
    {
        right_send();
    }

    else
    {
        forward_send();
    }

}



/* CMT0割り込みから呼ぶ */



/* 送信処理 */
void send_control(void)
{

    if(PORTB.PIDR.BIT.B1 == 0)
    {
        reverse_send();
    }

    else if(PORTB.PIDR.BIT.B0 == 0)
    {
        joystick_control();
    }

    else
    {
        stop_send();
    }

}
void intr_cmt0(void)
{
    send_flag = 1;
}


/* main */
void main(void)
{

    while(1)
    {

        if(send_flag)
        {

            send_flag = 0;

            send_control();

        }

    }

}
