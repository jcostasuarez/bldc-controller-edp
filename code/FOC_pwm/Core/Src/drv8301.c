#include "drv8301.h"

// Private functions
static uint16_t spi_exchange(uint16_t x);
static void spi_transfer(uint16_t *in_buf, const uint16_t *out_buf, int length);
static void spi_begin(void);
static void spi_end(void);
static void spi_delay(void);

/**
 * Get the middle value of three values
 *
 * @param a
 * First value
 *
 * @param b
 * Second value
 *
 * @param c
 * Third value
 *
 * @return
 * The middle value
 */
int utils_middle_of_3_int(int a, int b, int c) {
	int middle;

	if ((a <= b) && (a <= c)) {
		middle = (b <= c) ? b : c;
	} else if ((b <= a) && (b <= c)) {
		middle = (a <= c) ? a : c;
	} else {
		middle = (a <= b) ? a : b;
	}
	return middle;
}

void drv8301_init(void) {

	// Cotti Enable driver
	int rega = -1, regb = -1;
	rega++; regb++;	// Avoid unused warning
	HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_SET);

	HAL_Delay(100);

	int fault = drv8301_read_faults();
	drv8301_reset_faults();
	fault = drv8301_read_faults();

	// Disable OC
	drv8301_write_reg(2, 0x0430);
	drv8301_write_reg(2, 0x0430);

	drv8301_set_current_amp_gain(CURRENT_AMP_GAIN);

	// Cotti


	drv8301_set_oc_mode(DRV8301_OC_REPORT_ONLY);

	fault = drv8301_read_faults();
	drv8301_reset_faults();
	fault = drv8301_read_faults();

	// Make sure that the control reg changes before and after writing the register:

	drv8301_set_oc_adj(31);
	rega = drv8301_read_reg(2);
	drv8301_set_oc_adj(0);
	regb = drv8301_read_reg(2);
}

/**
 * Set the threshold of the over current protection of the DRV8301. It works by measuring
 * the voltage drop across drain-source of the MOSFETs and activates when it is higher than
 * a set value. Notice that this current limit is not very accurate.
 *
 * @param val
 * The value to use. Range [0 31]. A lower value corresponds to a lower current limit. See
 * the drv8301 datasheet for how to convert these values to currents.
 */
void drv8301_set_oc_adj(int val) {
	int reg = drv8301_read_reg(2);
	reg &= 0x003F;
	reg |= (val & 0x1F) << 6;
	drv8301_write_reg(2, reg);
}

/**
 * Set the over current protection mode of the DRV8301.
 *
 * @param mode
 * The over current protection mode.
 */
void drv8301_set_oc_mode(drv8301_oc_mode mode) {
	int reg = drv8301_read_reg(2);
	reg &= 0xFFCF;
	reg |= (mode & 0x03) << 4;
	drv8301_write_reg(2, reg);
}

void drv8301_set_current_amp_gain(int gain) {
    int reg = drv8301_read_reg(3);
    reg &= ~(0x03 << 4);

    switch(gain) {
    case 10:
        reg |= (0 & 0x03) << 2;
        break;
    case 20:
        reg |= (1 & 0x03) << 2;
        break;
    case 40:
        reg |= (2 & 0x03) << 2;
        break;
    case 80:
        reg |= (3 & 0x03) << 2;
        break;
    default:
        //gain not supported
        break;
    }

    drv8301_write_reg(3, reg);
}
/**
 * Read the fault codes of the DRV8301.
 *
 * @return
 * The fault codes, where the bits represent the following:
 * b0: FETLC_OC
 * b1: FETHC_OC
 * b2: FETLB_OC
 * b3: FETHB_OC
 * b4: FETLA_OC
 * b5: FETHA_OC
 * b6: OTW
 * b7: OTSD
 * b8: PVDD_UV
 * b9: GVDD_UV
 * b10: FAULT
 * b11: GVDD_OV
 *
 */
int drv8301_read_faults(void) {
	int r0 = drv8301_read_reg(0);
	int r1 = drv8301_read_reg(1);
	return (r0 & 0x3FF) | ((r1 & 0x80) << 4);
}

/**
 * Reset all latched faults.
 */
void drv8301_reset_faults(void) {

	HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_RESET);

	HAL_Delay(100);

	HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_SET);

	int reg = drv8301_read_reg(2);
	reg |= 1 << 2;
	drv8301_write_reg(2, reg);
	drv8301_set_current_amp_gain(CURRENT_AMP_GAIN);


}


unsigned int drv8301_read_reg(int reg) {
	uint16_t out = 0;
	out |= (1 << 15);
	out |= (reg & 0x0F) << 11;
	out |= 0x807F;

	if (reg != 0) {
		spi_begin();
		spi_exchange(out);
		spi_end();
	}

	spi_begin();
	uint16_t res = spi_exchange(0xFFFF);
	spi_end();

	return res;
}

void drv8301_write_reg(int reg, int data) {
	uint16_t out = 0;
	out |= (reg & 0x0F) << 11;
	out |= data & 0x7FF;

	spi_begin();
	spi_exchange(out);
	spi_end();
}

// Software SPI
static uint16_t spi_exchange(uint16_t x) {
	uint16_t rx;
	spi_transfer(&rx, &x, 1);
	return rx;
}

static void spi_transfer(uint16_t *in_buf, const uint16_t *out_buf, int length) {
	for (int i = 0;i < length;i++) {
		uint16_t send = out_buf ? out_buf[i] : 0xFFFF;
		uint16_t recieve = 0;

		for (int bit = 0;bit < 16;bit++) {
			HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port, SPI3_MOSI_Pin, send >> 15);
			send <<= 1;

			HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port, SPI3_SCK_Pin, GPIO_PIN_SET);
			spi_delay();

			HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port, SPI3_SCK_Pin, GPIO_PIN_RESET);

			int r1, r2, r3;
			r1 = HAL_GPIO_ReadPin(SPI3_MISO_GPIO_Port, SPI3_MISO_Pin);
			__NOP();
			r2 = HAL_GPIO_ReadPin(SPI3_MISO_GPIO_Port, SPI3_MISO_Pin);
			__NOP();
			r3 = HAL_GPIO_ReadPin(SPI3_MISO_GPIO_Port, SPI3_MISO_Pin);

			recieve <<= 1;
			if (utils_middle_of_3_int(r1, r2, r3)) {
				recieve |= 1;
			}

			spi_delay();
		}

		if (in_buf) {
			in_buf[i] = recieve;
		}
	}
}

static void spi_begin(void) {
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
}

static void spi_end(void) {
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
}

static void spi_delay(void) {
	for (volatile int i = 0;i < 10;i++) {
		__NOP();
	}
}

