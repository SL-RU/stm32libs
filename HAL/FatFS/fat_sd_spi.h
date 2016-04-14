#include "diskio.h"
#include "spi.h"
#include "gpio.h"

#define FAT_SD_SPI hspi1
#define FAT_SD_CS GPIOA, GPIO_PIN_4


#define spi_cs_low() HAL_GPIO_WritePin(FAT_SD_CS, GPIO_PIN_RESET)
#define spi_cs_high() HAL_GPIO_WritePin(FAT_SD_CS, GPIO_PIN_SET)

#define CAP_VER2_00	(1<<0)
#define CAP_SDHC	(1<<1)

struct hwif {
	int initialized;
	int sectors;
	int erase_sectors;
	int capabilities;
};
typedef struct hwif hwif;

extern hwif hw;


enum sd_speed { SD_SPEED_INVALID, SD_SPEED_400KHZ, SD_SPEED_25MHZ };


static void spi_set_speed(enum sd_speed speed);


int hwif_init(hwif* hw);
int sd_read(hwif* hw, uint32_t address, uint8_t *buf);
int sd_write(hwif* hw, uint32_t address,const uint8_t *buf);


#define sd_get_r3 sd_get_r7

static const char *r1_strings[7] = {
	"in idle",
	"erase reset",
	"illegal command",
	"communication crc error",
	"erase sequence error",
	"address error",
	"parameter error"
};
static const char *r2_strings[15] = {
	"card is locked",
	"wp erase skip | lock/unlock cmd failed",
	"error",
	"CC error",
	"card ecc failed",
	"wp violation",
	"erase param",
	"out of range | csd overwrite",
	"in idle state",
	"erase reset",
	"illegal command",
	"com crc error",
	"erase sequence error",
	"address error",
	"parameter error",
};
