#include "mma8451.h"

static void pause(void)
{
    int n;
    for(n=0; n<40; n++)
        __asm("nop");
//    for(n=0; n<100; n++);
}

void mma_set(uint8_t addr, uint8_t data)
{
		I2C0->C1 |=  I2C_C1_MST_MASK;
		I2C0->C1 |= I2C_C1_TX_MASK;
		
    WriteToI2C(MMA8451Q_I2C_ADDRESS|I2C_WRITE);
		I2CWait();
		I2C_Get_Ack();
	
    WriteToI2C(addr);
		I2CWait();
		I2C_Get_Ack();
	
    WriteToI2C(data);
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 &= ~I2C_C1_MST_MASK;
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		pause();
}

void mma_init(void)
{
		InitI2C();
		
    mma_set(MMA_CTRL_REG1,0x00); 
    mma_set(MMA_CTRL_REG1,0x01);
}

signed short mma_read(uint8_t addr)
{
		uint8_t result;
		I2C0->C1  |=  I2C_C1_MST_MASK;
		I2C0->C1 |= I2C_C1_TX_MASK;
    WriteToI2C(MMA8451Q_I2C_ADDRESS|I2C_WRITE);
	
		I2CWait();
		I2C_Get_Ack();
	
	  WriteToI2C(addr);
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 |= 0x04;
		WriteToI2C(MMA8451Q_I2C_ADDRESS|I2C_READ);
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		I2C0->C1 |= I2C_C1_TXAK_MASK;
		result = I2C0->D;
		I2CWait();

		I2C0->C1  &= ~I2C_C1_MST_MASK;
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		result = I2C0->D;
		pause();
    return result;
}