#include "vs1053.h"


static void vs1053_spiSpeed(uint8_t speed) // 0 - 400kHz; 1 - 25MHz
{
	//CHECK YOUR APB1 FREQ!!!
	int prescaler = SPI_BAUDRATEPRESCALER_256;

	if (speed == 0)
		prescaler = SPI_BAUDRATEPRESCALER_32;
	else if (speed == 1)
		prescaler = SPI_BAUDRATEPRESCALER_2;

	VS1053_SPI.Init.BaudRatePrescaler = prescaler;
	HAL_SPI_Init(&VS1053_SPI);
}

void vs1053_trans(uint8_t data)
{
	HAL_SPI_Transmit(&VS1053_SPI, &data, sizeof(data), 10);
}
uint8_t vs1053_txrx(uint8_t data)
{
	uint8_t rec;
	HAL_SPI_TransmitReceive(&VS1053_SPI, &data, &rec, sizeof(data), 10);
	return rec;
}
//Write to VS10xx register
//SCI: Data transfers are always 16bit. When a new SCI operation comes in 
//DREQ goes low. We then have to wait for DREQ to go high again.
//XCS should be low for the full duration of operation.
void vs1053_write_reg(uint8_t addressbyte, uint8_t highbyte, uint8_t lowbyte){
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_RESET); //Select control

  //SCI consists of instruction byte, address byte, and 16-bit data word.
  vs1053_trans(0x02); //Write instruction
  vs1053_trans(addressbyte);
  vs1053_trans(highbyte);
  vs1053_trans(lowbyte);
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating command is complete
  HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_SET); //Deselect Control
}

//Read the 16-bit value of a VS10xx register
uint16_t vs1053_read_reg (uint8_t addressbyte){
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_RESET); //Select control

  //SCI consists of instruction byte, address byte, and 16-bit data word.
  vs1053_trans(0x03); //Write instruction
  vs1053_trans(addressbyte);

  uint8_t response1 = vs1053_txrx(0xFF); //Read the first byte
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating command is complete
  uint8_t response2 = vs1053_txrx(0xFF); //Read the second byte
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating command is complete

	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_SET); //Deselect Control
	
  uint16_t resultvalue = response1 << 8;
  resultvalue |= response2;
  return resultvalue;
}

void sine_test()
{	
	while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_RESET); //Select control
	
	uint8_t data[] = {0x53, 0xef, 0x6e, 0x44, 0x00, 0x00, 0x00, 0x00};
	
	uint8_t i = 0;
	for(i = 0; i<8; i++)
		vs1053_trans(data[i]);
	
	while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) ; //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_SET); //Select control
}

//VS10xx SCI Registers
#define SCI_MODE 0x00
#define SCI_STATUS 0x01
#define SCI_BASS 0x02
#define SCI_CLOCKF 0x03
#define SCI_DECODE_TIME 0x04
#define SCI_AUDATA 0x05
#define SCI_WRAM 0x06
#define SCI_WRAMADDR 0x07
#define SCI_HDAT0 0x08
#define SCI_HDAT1 0x09
#define SCI_AIADDR 0x0A
#define SCI_VOL 0x0B
#define SCI_AICTRL0 0x0C
#define SCI_AICTRL1 0x0D
#define SCI_AICTRL2 0x0E
#define SCI_AICTRL3 0x0F

void VS1053_Init(void)
{
	printf("VS1053 initing\n");
	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(VS1053_xRST, GPIO_PIN_RESET);
	
	vs1053_spiSpeed(0);
	vs1053_trans(0xFF);
	
	HAL_Delay(10);
	
	HAL_GPIO_WritePin(VS1053_xRST, GPIO_PIN_SET);
	
	vs1053_write_reg(SCI_VOL, 60, 60);
	
	int MP3Mode = vs1053_read_reg(SCI_MODE);
	int MP3Status = vs1053_read_reg(SCI_STATUS);
	int MP3Clock = vs1053_read_reg(SCI_CLOCKF);
	

  printf("SCI_Mode (0x4800) = 0x%x\n", MP3Mode);

  printf("SCI_Status (0x48) = 0x%x\n", MP3Status);

  int vsVersion = (MP3Status >> 4) & 0x000F; //Mask out only the four version bits
  printf("VS Version (VS1053 is 4) = 0x%x\n", vsVersion); //The 1053B should respond with 4. VS1001 = 0, VS1011 = 1, VS1002 = 2, VS1003 = 3

  printf("SCI_ClockF = 0x%x\n", MP3Clock);
	vs1053_write_reg(SCI_CLOCKF, 0x60, 0x00); //set clock x3
	
	//printf("sine\n");
	//vs1053_write_reg(SCI_MODE, 0x08, 0x20);
	//printf("SCI_Mode (0x4800) = 0x%x\n", vs1053_read_reg(SCI_MODE));
	//vs1053_spiSpeed(1);
	//sine_test();
	//printf("done\n");

}
char* curfile_name;
uint8_t cancel = 0;
void VS1053_play(FIL* file, char* name)
{
	printf("play\n");
	cancel = 0;
	UINT br;
	uint8_t need = 1;
	
	curfile_name = name;
	
	gui_draw();
	
	uint8_t buf[32] = {0};

	uint32_t lst =  HAL_GetTick(), cur, vv = 0, delt = 0;
	while(1)
	{
		HAL_GetTick();
		while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET)
		{
			if(need)
			{
				if(f_read(file, &buf, 32, &br) != FR_OK)
					break;				
				cur = HAL_GetTick();
				lst = cur;
				need = 0;
				vv++;
			}
			gui_draw();
		}			//Wait for DREQ to go high indicating IC is available
		if(need)
		{
			if(f_read(file, &buf, 32, &br) != FR_OK)
				break;
			cur = HAL_GetTick();
			lst = cur;
			vv++;
			need = 0;
		}
		
		HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_RESET); //Select control
		HAL_SPI_Transmit(&VS1053_SPI, buf, 32, 1);
		HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_SET); //Select control
		need = 1;
		keyboard_update();
		if(cancel)
		{
			printf("canc\n");
			cancel = 0;
			break;
		}
	}
	curfile_name = 0;
	printf("end\n");
}
void VS1053_draw(void)
{
	if(curfile_name != 0)
		gui_lable(curfile_name, 0, 20, 128, 20, 1, 1);
}
uint8_t VS1053_input(uint8_t key)
{
	cancel = 1;
	gui_closeMessage();
	return 1;
}
