#include "iodefine.h"

/* クロック設定 */
void set_main_clk(void)
{
	unsigned int i;
	
	SYSTEM.PRCR.WORD = 0xA507;
	
	SYSTEM.HOCOCR2.BYTE = 0x00;
	SYSTEM.HOCOWTCR2.BYTE = 0x14;
	SYSTEM.HOCOCR.BYTE = 0x00;
	
	for( i=0; i<1750; i++);
	
	SYSTEM.OPCCR.BYTE = 0x02;
	
	while(SYSTEM.OPCCR.BIT.OPCMTSF);
	
	SYSTEM.SCKCR.LONG = 0x10821015;
	
	while(SYSTEM.SCKCR.LONG != 0x10821015);
	
	SYSTEM.SCKCR3.WORD = 0x0100;
	
	SYSTEM.PRCR.WORD = 0xA500;
}
/* MTU0 右モーター */
void set_MTU0(void)
{

    SYSTEM.PRCR.WORD = 0xA502;

    SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;

    SYSTEM.PRCR.WORD = 0xA500;


    /* PB3 = MTIOC0A */
    PORTB.PDR.BIT.B3 = 1;


    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;

    MPC.PB3PFS.BIT.PSEL = 1;

    MPC.PWPR.BIT.PFSWE = 0;
    MPC.PWPR.BIT.B0WI = 1;


    PORTB.PMR.BIT.B3 = 1;


    MTU0.TCR.BIT.TPSC = 2;
    MTU0.TCR.BIT.CCLR = 2;

    MTU0.TMDR.BIT.MD = 2;


    MTU0.TIORH.BIT.IOA = 1;
    MTU0.TIORH.BIT.IOB = 2;


    /* 周期20ms */
    MTU0.TGRB = 40000-1;

    MTU0.TCNT = 0;


    /* 停止 */
    MTU0.TGRA = 3000-1;

}
/* MTU2 左モーター */
void set_MTU2(void)
{

	SYSTEM.PRCR.WORD = 0xA502;

	/* MTU0～MTU5停止解除 */
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;

	SYSTEM.PRCR.WORD = 0xA500;


	/* PB5 = MTIOC2A */
	PORTB.PDR.BIT.B5 = 1;


	/* MPC設定 */
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;


	MPC.PB5PFS.BIT.PSEL = 1;


	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;


	PORTB.PMR.BIT.B5 = 1;



	/* MTU2 PWM設定 */

	MTU2.TCR.BIT.TPSC = 2;
	MTU2.TCR.BIT.CCLR = 2;

	MTU2.TMDR.BIT.MD = 2;


	MTU2.TIOR.BIT.IOA = 1;
	MTU2.TIOR.BIT.IOB = 2;

	/* 20ms周期 */

	MTU2.TGRB = 40000-1;


	MTU2.TCNT = 0;


	/* 停止位置 */

	MTU2.TGRA = 3000-1;

}
void set_SCI5(void)
{
	/* SCI5モジュール有効化 */
	SYSTEM.PRCR.WORD = 0xA502;
	SYSTEM.MSTPCRB.BIT.MSTPB26 = 0;
	SYSTEM.PRCR.WORD = 0xA500;


	/* PC2 RXD5 */
	PORTC.PDR.BIT.B2 = 0;

	/* PC3 TXD5 */
	PORTC.PDR.BIT.B3 = 1;


	/* MPC設定 */
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;


	MPC.PC2PFS.BIT.PSEL = 0xA;
	MPC.PC3PFS.BIT.PSEL = 0xA;


	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;


	PORTC.PMR.BIT.B2 = 1;
	PORTC.PMR.BIT.B3 = 1;


	/* 通信設定 */
	SCI5.SCR.BYTE = 0x00;


	SCI5.SMR.BIT.CKS = 0;
	SCI5.SMR.BIT.CHR = 0;
	SCI5.SMR.BIT.PE  = 0;
	SCI5.SMR.BIT.STOP = 0;


	/* 115200bps */
	SCI5.BRR = 8;


	/* 送受信許可 */
	SCI5.SCR.BIT.RE = 1;
	SCI5.SCR.BIT.TE = 1;


	/* 受信割り込み許可 */
	SCI5.SCR.BIT.RIE = 1;


	SCI5.SSR.BIT.ORER = 0;
	SCI5.SSR.BIT.FER = 0;
	SCI5.SSR.BIT.PER = 0;


	IEN(SCI5,RXI5)=1;
	IEN(SCI5,ERI5)=1;
}
void set_AD(void)
{
    SYSTEM.PRCR.WORD = 0xA502;

    SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;

    SYSTEM.PRCR.WORD = 0xA500;


    /* AN000(P40)設定 */

    PORT4.PDR.BIT.B0 = 0;


    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;

    MPC.P40PFS.BYTE = 0x80;

    MPC.PWPR.BIT.PFSWE = 0;
    MPC.PWPR.BIT.B0WI = 1;


    PORT4.PMR.BIT.B0 = 0;


    /* AN000選択 */
    S12AD.ADANSA.BIT.ANSA0 = 1;


    /* 連続スキャン */
    S12AD.ADCSR.BIT.ADCS = 2;


    /* 開始 */
    S12AD.ADCSR.BIT.ADST = 1;
}
void set_SW(void)
{
    /* PB0 */
    PORTB.PDR.BIT.B0 = 0;
    PORTB.PCR.BIT.B0 = 1;


    /* PB1 */
    PORTB.PDR.BIT.B1 = 0;
    PORTB.PCR.BIT.B1 = 1;
}
void set_CMT(void)
{

    SYSTEM.PRCR.WORD = 0xA502;

    SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;

    SYSTEM.PRCR.WORD = 0xA500;


    /* CMT0割り込み禁止 */
    IEN(CMT0,CMI0)=0;


    /* 割り込み優先度 */
    IPR(CMT0,CMI0)=1;


    /* クロック設定 */
    CMT0.CMCR.BIT.CKS = 3;


    /* カウンタ初期化 */
    CMT0.CMCNT = 0;


    /* 約0.5秒周期 */
    CMT0.CMCOR = 31250-1;


    /* 割り込み許可 */
    CMT0.CMCR.BIT.CMIE = 1;


    /* CMT0割り込み許可 */
    IEN(CMT0,CMI0)=1;


    /* 開始 */
    CMT.CMSTR0.BIT.STR0 = 1;

}

/* ハードウェア初期化 */
void HardwareSetup(void)
{

    set_main_clk();
    
    set_AD();
    
    set_CMT();

    //set_MTU0();

    //set_MTU2();
    
    set_SW();
    
    set_SCI5();

}
