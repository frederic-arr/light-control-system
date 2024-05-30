/*****************************************************************************

I2C driver - author org. : VP; adapted : FV (0520)

*****************************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>
#include <stdint.h>

#define I2EN (1<<6)
#define STA (1<<5)
#define STO (1<<4)
#define SI (1<<3)
#define AA (1<<2)

#define R 1
#define W 0
#define I2C_TIMEOUT			2000		// [ms]

// Error codes
#define I2C_NOERROR			0
#define I2C_TIMEOUT_ERROR	0xff


#ifndef WEAK
#define WEAK __attribute__ ((weak))
#endif

/* I2C initialisation routine. Note that RIT timer is used to manage timeouts in the I2C driver.
 * Parameters: port_nb: 0 to 3
 *             scl_freq: I2C clock frequency [Hz]
 */
void init_i2c(int port_nb, uint32_t scl_freq);

/* Write I2C frame as master
 * Parameters: addr: slave address
 *             data: pointer on data to transmit
 *             len: length of data
 * Return:     I2C error code
 */
uint8_t I2C_master_write(uint8_t addr, uint8_t *data, uint16_t data_len);

/* Read I2C frame as master
 * Parameters: addr: slave address
 *             data: pointer on data to receive
 *             len: length of data
 * Return:     I2C error code
 */
uint8_t I2C_master_read(uint8_t addr, uint8_t *data, uint16_t data_len);



#endif /* I2C_H_ */

