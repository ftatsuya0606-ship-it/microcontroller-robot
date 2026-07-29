#include "iodefine.h"
/*-------------------------------------------------------------------------------
	関数名		: set_main_clk
	内容	 	: CPU CLOCK の初期化(HOCO 32MHz/ICLK:32MHz,PCLKB:32MHz)
	引数		: 無し
	戻値		: 無し
	更新日		: 2024/12/24
-------------------------------------------------------------------------------*/
void set_main_clk(void){

	unsigned int i; 		
	// クロック発生 ( HOCO 32MHz発振 )
	SYSTEM.PRCR.WORD = 0xA507;	//①プロテクト解除
	SYSTEM.HOCOCR2.BYTE   = 0x00;	//③32MHzを選択
	SYSTEM.HOCOWTCR2.BYTE = 0x14;	//④発振安定待機時間の設定（180cyc）
	SYSTEM.HOCOCR.BYTE    = 0x00;	//⑤HOCOを動作
	for( i = 0 ; i<1750 ; i++ );	//⑥発振安定待機時間（350μs）
	// 動作電力制御モード/クロック分周比/システムクロック切換え
	SYSTEM.OPCCR.BYTE  = 0x02;	//⑦動作電力制御モード設定（中速動作モード1A）	
	while( SYSTEM.OPCCR.BIT.OPCMTSF );//⑧動作電力制御モード遷移完了待ち													
	SYSTEM.SCKCR.LONG  = 0x10821015;  //⑨内部クロック分周比を設定/PCLKB:32MHz
	while( SYSTEM.SCKCR.LONG!=0x10821015);//⑩内部クロック分周比の書き換え完了待ち			
	SYSTEM.SCKCR3.WORD = 0x0100;	  //⑪システムクロックを切り替え（HOCO）
	SYSTEM.PRCR.WORD   = 0xA500;	  //⑫プロテクト設定		
}
/*-------------------------------------------------------------------------------
	関数名		: set_AD
	内容	 	: AD変換器の初期化シーケンス
	引数		: 無し
	戻値		: 無し
	更新日		: 2026/07/09
-------------------------------------------------------------------------------*/
void set_AD(void){
	//モジュールストップビットの解除
	SYSTEM.PRCR.WORD = 0xA502; //1:プロテクト解除
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0; //0:AD変換器のモジュールストップ解除
	SYSTEM.PRCR.WORD = 0xA500; //0:プロテクト
	//P40をAN00端子として設定
	PORT4.PDR.BIT.B0 = 0; //P40を入力設定(注)
	MPC.PWPR.BIT.B0WI = 0; //書き込みプロテクト許可
	MPC.PWPR.BIT.PFSWE = 1; //書き込みプロテクト許可
	MPC.P40PFS.BYTE = 0x80; //P40をAN000アナログ端子として使用
	MPC.PWPR.BIT.PFSWE = 0; //書き込みプロテクト禁止
	MPC.PWPR.BIT.B0WI = 1; //書き込みプロテクトの禁止
	PORT4.PMR.BIT.B0 = 0; //P40を周辺機能として使用(注)
	//AD変換器の設定
	S12AD.ADANSA.BIT.ANSA0 = 1; //AN000を選択
	S12AD.ADCSR.BIT.ADCS = 2; //2:連続スキャンモード
	S12AD.ADCSR.BIT.ADST = 0x01; //AD変換スタート
}
/*-------------------------------------------------------------------------------
	関数名		: set_CMT1
	内容	 	: CMT1の初期化シーケンス
	引数		: 無し
	戻値		: 無し
	更新日		: 2026/07/09
-------------------------------------------------------------------------------*/
void set_CMT1(void){
	
	SYSTEM.PRCR.WORD = 0xA502;	//レジスタ設定-許可
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;	//モジュールストップの解除(CMT0,CMT1)
	SYSTEM.PRCR.WORD = 0xA500;	//レジスタ設定-禁止
	//IEN(CMT1,CMT1)=0;		//CMT1割り込み要求-禁止
	ICU.IER[3].BIT.IEN5=0;	//構造体
	//IPR(CMT1,CMT1)=1;		//CMT1の割り込み優先度レベル1
	ICU.IPR[5].BIT.IPR = 1;	//構造体
	CMT1.CMCR.BIT.CKS = 3;		//クロック設定 512分周
	CMT1.CMCNT = 0;			//CMCNTのクリア
	CMT1.CMCOR = 31250-1;		//CMCORに周期値設定(0.5s)
	CMT1.CMCR.BIT.CMIE = 1;		//CMT1コンペアマッチ割り込み許可
	//IEN(CMT1,CMT1)=1;		//CMT1割り込み要求-許可
	ICU.IER[3].BIT.IEN5 = 1;//構造体
}
/*-------------------------------------------------------------------------------
	関数名		: set_MTU0
	内容	 	: ハードウェアセットアップ用関数
	引数		: 無し
	戻値		: 無し
	更新日		: 2024/12/24
-------------------------------------------------------------------------------*/
void set_MTU0(void){
	
	SYSTEM.PRCR.WORD = 0xA502;	//レジスタ設定-許可
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;	//モジュールストップの解除(MTU0-MTU5)
	SYSTEM.PRCR.WORD = 0xA500;	//レジスタ設定-禁止
	//割り込みｺﾝﾄﾛｰﾗの設定(※追加)
	IEN(MTU0,TGIA0) =0;//割り込み禁止(※追加)
	IPR(MTU0,TGIA0) =15;//割り込み優先度(15)設定(※追加)
	//PB3をMTIOC0As端子を出力設定
	PORTB.PDR.BIT.B3=1;	//PB3を出力設定
	MPC.PWPR.BIT.B0WI = 0;	//書き込みプロテクトの許可
	MPC.PWPR.BIT.PFSWE = 1;	//書き込みプロテクトの許可
	MPC.PB3PFS.BIT.PSEL = 1;//PB3をMTIOC0Aとして使用
	MPC.PWPR.BIT.PFSWE= 0;	//書き込みプロテクトの禁止
	MPC.PWPR.BIT.B0WI = 1;	//書き込みプロテクトの禁止
	PORTB.PMR.BIT.B3=1;	//PB3を周辺機能として使用
	//MTU0の設定
	MTU0.TCR.BIT.TPSC =2;	//[1]ﾀｲﾏﾌﾟﾘｽｹｰﾗ選択ﾋﾞｯﾄ PCLK/16
	MTU0.TCR.BIT.CCLR =2;	//[2]TCNTｶｳﾝﾀｸﾘｱﾋﾞｯﾄTRGB
	MTU0.TMDR.BIT.MD  =2;	//[3]PWMモード1に設定
	MTU0.TIORH.BIT.IOA=1;	//[4]初期はLow出力でLow出力
	MTU0.TIORH.BIT.IOB=2;	//[4]初期はLow出力でHigh出力
	//MTU0/TGIEAの割り込み許可(※追加)
	MTU0.TIER.BIT.TGIEA=1;//TGRA0割り込み許可(※追加)
	IEN(MTU0,TGIA0) =1;//割り込み許可(※追加)
	//TGRA,TGRB,TCNTの設定
	MTU0.TGRB=40000-1;	//[5]周期20ms設定
	MTU0.TCNT=0;		//[5]MTU0-TCNTの初期化
	MTU0.TGRA=3000-1;	//[5]PWM分周比1500us(1.5ms/STOP)
	//MTU0.TGRA=1600-1;//[5]PWM分周比800us(0.8ms/CW-HIGH)
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
/*-------------------------------------------------------------------------------
	関数名		: set_SCI5
	内容	 	: SCI0の初期化設定シーケンス
	引数		: 無し
	戻値		: 無し
	更新日		: 2026/07/14
-------------------------------------------------------------------------------*/
void set_SCI5(void){
	
	/* SCI0モジュール有効化 */
	SYSTEM.PRCR.WORD = 0xA502;	//プロテクト解除
	SYSTEM.MSTPCRB.BIT.MSTPB26 = 0;	//SCI5モジュールストップ解除
	SYSTEM.PRCR.WORD = 0xA500;	//プロテクト設定
	
	/* シリアルコントロールレジスタ初期化 */
	SCI5.SCR.BYTE = 0x00;//SCR.TE,RIE,TE,RE,TEIEを'0'に設定
	/* 追記:割り込み許可レジスタ 0:禁止 */
	IEN(SCI5,ERI5) = 0;	//ERI5 受信エラー割り込み禁止
	IEN(SCI5,RXI5) = 0;	//RXI5 受信割り込み禁止
	/*[1]IOポートの機能設定*/
	PORTC.PDR.BIT.B2 = 0;	//ポート設定 RXD5 入力(PC2)
	PORTC.PDR.BIT.B3 = 1;	//ポート設定 TXD5 出力(PC3)
	
	MPC.PWPR.BIT.B0WI = 0;	//PWPRプロテクトレジスタB0WI許可
	MPC.PWPR.BIT.PFSWE = 1;	//PWPRプロテクトレジスタPFSWE許可
	MPC.PC2PFS.BIT.PSEL = 0xA; //PC2をRXD0端子として使用する
	MPC.PC3PFS.BIT.PSEL = 0xA; //PC3をTXD0端子として使用する
	MPC.PWPR.BIT.PFSWE = 0;	//PWPRプロテクトレジスタPFSWE禁止
	MPC.PWPR.BIT.B0WI = 1;	//PWPRプロテクトレジスタB0WI禁止
	
	PORTC.PMR.BIT.B2 = 1;	//ポート設定RXD5周辺機器ポート
	PORTC.PMR.BIT.B3 = 1;	//ポート設定TXD5周辺機器ポート
	
	/*[2]SCRにクロック選択*/
	SCI5.SMR.BIT.CKS =0;	//内臓ポートジェネレーター PCLKクロック 32MHz n 設定
	
	/*[3]I2C,SPIの選択は、省略*/
	
	/*[4]SMRを設定/SCMR,SEMR設定は省略*/
	SCI5.SCR.BIT.CKE = 0;	//内臓ジェネレータ使用、出力無し
	SCI5.SMR.BIT.STOP = 0;	//ストップビット長 = 1
	SCI5.SMR.BIT.PM = 0;	//パリティ偶数
	SCI5.SMR.BIT.PE = 0;	//パリティなし
	SCI5.SMR.BIT.CHR = 0;	//データ長 8bit
	SCI5.SMR.BIT.CM = 0;	//調歩同期
	/*[5]BRRにビットレート設定*/
	SCI5.BRR = 8;	//:115200bps設定
	/*追記:割り込みプライオリティレジスタの設定*/
	IPR(SCI5,) = 15;	//SCI5割り込み優先レベル最高 15
	/*追記 受信割り込みの設定 */
	SCI5.SCR.BIT.RIE = 1;	//受信と受信エラー割り込み有効
	/*[6]SCRに送受信の許可*/
	SCI5.SCR.BIT.TE = 1;	//送信許可
	SCI5.SCR.BIT.RE = 1;	//受信許可
	/*追記:受信エラーフラグクリア*/
	SCI5.SSR.BIT.ORER = 0;	//ORERエラーをクリア
	SCI5.SSR.BIT.FER = 0;	//FERエラーをクリア
	SCI5.SSR.BIT.PER = 0;	//PERエラーをクリア
	/*追記:割り込み許可レジスタ 1:許可 */
	IEN(SCI5,ERI5) = 1;	//ERI0受信エラー割り込み許可
	IEN(SCI5,RXI5) = 1;	//RXI0受信割り込み許可
}
/*-------------------------------------------------------------------------------
	関数名		: hw_setup
	内容	 	: ハードウェアセットアップ用関数
	引数		: 無し
	戻値		: 無し
	更新日		: 2024/12/24
-------------------------------------------------------------------------------*/
void HardwareSetup(void){
	
	set_main_clk();	 // クロック設定
	set_AD(); // S12AD設定(AN000+AN001)
	//set_CMT1();
	set_MTU0();
	set_MTU2();
	set_SCI5();

	
}
