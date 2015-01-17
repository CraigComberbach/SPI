#ifndef SPI_C
#define SPI_C

/***********Add to config file header************/
/*
//* Library
#define SPI_MAJOR	0
#define SPI_MINOR	0
#define SPI_PATCH	0

//Frequency Definitions
enum FREQUENCY_UNITS
{
	Hz,
	kHz,
	MHz,
};
*/

/***************Add to config file***************/
/*
#ifndef SPI_LIBRARY
	#error "You need to include the SPI library for this code to compile"
#endif

//SPI Modules
enum
{
	SPI1,	//SPI1
	SPI2,	//SPI2
	SPI3,	//SPI3
	SPI4,	//SPI4
	NUMBER_OF_SPI_MODULES,
};
 */

/************* Semantic Versioning***************/
#define SPI_LIBRARY

/*************   Magic  Numbers   ***************/
/*************    Enumeration     ***************/
/*************Function  Prototypes***************/
void SPI_Initialize(unsigned int module, volatile unsigned int *STAT, volatile unsigned int *CON1, volatile unsigned int *CON2, volatile unsigned int *BUF, unsigned int frequency, enum FREQUENCY_UNITS units);


#endif
