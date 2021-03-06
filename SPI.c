/**************************************************************************************************
Target Hardware:		PIC24FJ256GA1xx
Code assumptions:
Purpose:
Notes:

Version History:
vnext	Y-M-D	Craig Comberbach
Compiler: XC16 v1.11	IDE: MPLABx 2.20	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 7 64 bit Home Premium SP1
	First version
**************************************************************************************************/
/*************    Header Files    ***************/
#include "Config.h"
#include "SPI.h"

/************* Library Definition ***************/
/************* Semantic Versioning***************/
#if SPI_MAJOR != 0
	#error "SPI has had a change that loses some previously supported functionality"
#elif SPI_MINOR != 0
	#error "SPI has new features that your code may benefit from"
#elif SPI_PATCH != 0
	#error "SPI has had a bug fix, you should check to see that you weren't relying on a bug for functionality"
#endif

/************Arbitrary Functionality*************/
#define FCY	(FOSC_HZ/2)

/*************   Magic  Numbers   ***************/
/*************    Enumeration     ***************/
/***********  Structure Definitions  ************/
/***********      Data Structures    ************/
//SPIn Status and Control Register
union SPInSTATbits
{
	volatile unsigned int *Register;
	struct
	{
		int SPIRBF:1;
		int SPITBF:1;
		int SISEL:3;
		int SRXMPT:1;
		int SPIROV:1;
		int SRMPT:1;
		int SPIBEC:3;
		int :2;
		int SPISIDL:1;
		int :1;
		int SPIEN:1;
	};
};

//SPIn Control Register 1
union SPInCON1bits
{
	volatile unsigned int *Register;
	struct
	{
		int PPRE:2;
		int SPRE:3;
		int MSTEN:1;
		int CKP:1;
		int SSEN:1;
		int CKE:1;
		int SMP:1;
		int MODE16:1;
		int DISSDO:1;
		int DISSCK:1;
		int :3;
	};
};

//SPIn Control Register 2
union SPInCON2bits
{
	volatile unsigned int *Register;
	struct
	{
		int SPIBEN:1;
		int SPIFE:1;
		int :11;
		int SPIFPOL:1;
		int SPIFSD:1;
		int FRMEN:1;
	};
};

/***********State Machine Definitions************/
/*************  Global Variables  ***************/
/*************Interrupt Prototypes***************/
/*************Function  Prototypes***************/
void SPI_Write(unsigned int module, int value);
int SPI_Read(unsigned char module);
int SPI_Busy(unsigned int module);
int SPI_Error(unsigned int module);
int SPI_Wait_To_Finish(unsigned int module, unsigned int timeLimit);
int SPI_Buffer_Status(unsigned int module);
void SPI_Clear_Buffer(unsigned int module);

/************* Device Definitions ***************/
/************* Module Definitions ***************/
union SPInSTATbits SPInSTAT[NUMBER_OF_SPI_MODULES];
union SPInCON1bits SPInCON1[NUMBER_OF_SPI_MODULES];
union SPInCON2bits SPInCON2[NUMBER_OF_SPI_MODULES];
volatile unsigned int *SPInBUF[NUMBER_OF_SPI_MODULES];

/************* Other  Definitions ***************/

void SPI_Initialize(unsigned int module, volatile unsigned int *STAT, volatile unsigned int *CON1, volatile unsigned int *CON2, volatile unsigned int *BUF, unsigned int frequency, enum FREQUENCY_UNITS units)
{
	//Range checking
	if(module >= NUMBER_OF_SPI_MODULES)
		return;

	//Capture the modules associated registers
	SPInSTAT[module].Register = STAT;
	SPInCON1[module].Register = CON1;
	SPInCON2[module].Register = CON2;
	SPInBUF[module] = BUF;

	//TODO - Use the power of automagic to determine closest achievable frequency to the target frequency
	#warning "Use the power of automagic to determine closest achievable frequency to the target frequency"

	//Initialize the module
	//SPIn Control Register 2
	SPInCON2[module].SPIBEN = 0;	//
	SPInCON2[module].SPIFE = 0;		//
	SPInCON2[module].SPIFPOL = 0;	//
	SPInCON2[module].SPIFSD = 0;	//
	SPInCON2[module].FRMEN = 0;		//

	//SPIn Control Register 1
	SPInCON1[module].PPRE = 0;		//
	SPInCON1[module].SPRE = 0;		//
	SPInCON1[module].MSTEN = 0;		//
	SPInCON1[module].CKP = 0;		//
	SPInCON1[module].SSEN = 0;		//
	SPInCON1[module].CKE = 0;		//
	SPInCON1[module].SMP = 0;		//
	SPInCON1[module].MODE16 = 0;	//
	SPInCON1[module].DISSDO = 0;	//
	SPInCON1[module].DISSCK = 0;	//

	//SPIn Status Register
	SPInSTAT[module].SPIRBF = 0;	//
	SPInSTAT[module].SPITBF = 0;	//
	SPInSTAT[module].SISEL = 0;		//
	SPInSTAT[module].SRXMPT = 0;	//
	SPInSTAT[module].SPIROV = 0;	//
	SPInSTAT[module].SRMPT = 0;		//
	SPInSTAT[module].SPIBEC = 0;	//
	SPInSTAT[module].SPISIDL = 0;	//
	SPInSTAT[module].SPIEN = 0;		//

	return;
}

void SPI_Write(unsigned int module, int value)
{
	if(module < NUMBER_OF_SPI_MODULES)
		*SPInBUF[module] = value;
	return;
}

int SPI_Read(unsigned char module)
{
	if(module < NUMBER_OF_SPI_MODULES)
		return *SPInBUF[module];
	else
		return 0;
}

int SPI_Busy(unsigned int module)
{
	if(module < NUMBER_OF_SPI_MODULES)
	{
		if(!SPInSTAT[module].SRMPT)
			return 0;//Not busy
		else
			return 1;//Busy
	}
	else
		return 2;//Module can't exist
}

int SPI_Error(unsigned int module)
{
	if(module < NUMBER_OF_SPI_MODULES)
	{
		if(SPInSTAT[module].SPIROV)
		{
			SPInSTAT[module].SPIROV = 0;
			return 1;//Receive overflowed
		}
		else
			return 0;//Receive is fine
	}
	else
		return 2;//Module can't exist
}
