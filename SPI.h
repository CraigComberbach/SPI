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

struct SPI_MASTER_STATE_MACHINE SPInStateMachine[x];
 */

/************* Semantic Versioning***************/
#define SPI_LIBRARY

/*************   Magic  Numbers   ***************/
/***********      Data Structures    ************/
struct SPI_MASTER_STATE_MACHINE
{
	char *Write;
	char *Read;
};

/*************    Enumeration     ***************/
/*************Function  Prototypes***************/
void SPI_Initialize(unsigned int module, volatile unsigned int *STAT, volatile unsigned int *CON1, volatile unsigned int *CON2, volatile unsigned int *BUF, unsigned int frequency, enum FREQUENCY_UNITS units);
void SPI_Change_Master_State_Machine(unsigned int module, struct SPI_MASTER_STATE_MACHINE *stateMachine, unsigned char size);

#endif
