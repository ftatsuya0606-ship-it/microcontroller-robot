#include "iodefine.h"

void HardwareSetup(void);

char c_in;

#define RIGHT_FORWARD (2700-1)
#define LEFT_FORWARD  (3300-1)

#define RIGHT_REVERSE (3300-1)
#define LEFT_REVERSE  (2700-1)

#define STOP          (3000-1)



void forward(void)
{
    MTU0.TGRA = RIGHT_FORWARD;
    MTU2.TGRA = LEFT_FORWARD;
}



void left(void)
{
    MTU0.TGRA = RIGHT_FORWARD;
    MTU2.TGRA = STOP;
}



void right(void)
{
    MTU0.TGRA = STOP;
    MTU2.TGRA = LEFT_FORWARD;
}



void stop(void)
{
    MTU0.TGRA = STOP;
    MTU2.TGRA = STOP;
}



void reverse(void)
{
    MTU0.TGRA = RIGHT_REVERSE;
    MTU2.TGRA = LEFT_REVERSE;
}



/* SCI5受信割込み */

void intr_receive(void)
{

    c_in = SCI5.RDR;



    if(c_in == '1')
    {
        forward();
    }


    else if(c_in == '2')
    {
        left();
    }


    else if(c_in == '3')
    {
        right();
    }


    else if(c_in == '4')
    {
        reverse();
    }


    else if(c_in == '0')
    {
        stop();
    }

}



/* 受信エラー */

void intr_err_receive(void)
{

    SCI5.SCR.BIT.RIE = 0;


    SCI5.SSR.BIT.ORER = 0;
    SCI5.SSR.BIT.FER = 0;
    SCI5.SSR.BIT.PER = 0;


    SCI5.SCR.BIT.RIE = 1;

}




void main(void)
{

    HardwareSetup();



    MTU.TSTR.BIT.CST0 = 1;
    MTU.TSTR.BIT.CST2 = 1;



    stop();



    while(1)
    {

    }

}
