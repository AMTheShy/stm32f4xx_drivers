#ifndef stm32F401Re_I2C_driver.h
#define stm32F401Re_I2C_driver.h








/*I2C Register Bit position structure

*/


/* I2C_CR1 */
#define I2C_CR1_PE              0U
#define I2C_CR1_SMBUS           1U
#define I2C_CR1_SMBTYPE         3U
#define I2C_CR1_ENARP           4U
#define I2C_CR1_ENPEC           5U
#define I2C_CR1_ENGC            6U
#define I2C_CR1_NOSTRETCH       7U
#define I2C_CR1_START           8U
#define I2C_CR1_STOP            9U
#define I2C_CR1_ACK             10U
#define I2C_CR1_POS             11U
#define I2C_CR1_PEC             12U
#define I2C_CR1_ALERT           13U
#define I2C_CR1_SWRST           15U

/* I2C_CR2 */
#define I2C_CR2_FREQ            0U
#define I2C_CR2_ITERREN         8U
#define I2C_CR2_ITEVTEN         9U
#define I2C_CR2_ITBUFEN         10U
#define I2C_CR2_DMAEN           11U
#define I2C_CR2_LAST            12U

/* I2C_SR1 */
#define I2C_SR1_SB              0U
#define I2C_SR1_ADDR            1U
#define I2C_SR1_BTF             2U
#define I2C_SR1_ADD10           3U
#define I2C_SR1_STOPF           4U
#define I2C_SR1_RXNE            6U
#define I2C_SR1_TXE             7U
#define I2C_SR1_BERR            8U
#define I2C_SR1_ARLO            9U
#define I2C_SR1_AF              10U
#define I2C_SR1_OVR             11U
#define I2C_SR1_PECERR          12U
#define I2C_SR1_TIMEOUT         14U
#define I2C_SR1_SMBALERT        15U

/* I2C_SR2 */
#define I2C_SR2_MSL             0U
#define I2C_SR2_BUSY            1U
#define I2C_SR2_TRA             2U
#define I2C_SR2_GENCALL         4U
#define I2C_SR2_SMBDEFAULT      5U
#define I2C_SR2_SMBHOST         6U
#define I2C_SR2_DUALF           7U
#define I2C_SR2_PEC             8U

/* I2C_CCR */
#define I2C_CCR_CCR             0U
#define I2C_CCR_DUTY            14U
#define I2C_CCR_FS              15U

#endif 
