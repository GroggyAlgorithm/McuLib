/**
 * \file L99Sm81.h
 * \author Tim Robbins
 * \brief L99SM81 Stepper controller/driver Register definitions
 */ 
#ifndef L99SM81_H_
#define L99SM81_H_



//Op codes
#define L99SM81_WRITE_OP						0b00
#define L99SM81_READ_OP							0b01
#define L99SM81_READ_CLR_OP						0b10
#define L99SM81_READ_DEVICE_INFO_OP				0b11

//Op Addresses
#define L99SM81_ADVANCED_OP_ADDRESS_START		0x3F
#define L99SM81_GSB_OPTIONS_ADDRESS_START		0x3E
#define L99SM81_WD_TYPE_ADDRESS_START			0x11
#define L99SM81_DEVICE_2_ADDRESS_START			0x03
#define L99SM81_DEVICE_1_ADDRESS_START			0x02
#define L99SM81_DEVICE_FAMILY_ADDRESS_START		0x01
#define L99SM81_COMPANY_CODE_ADDRESS_START		0x00

//Device info registers
#define L99SM81_ADVANCED_OPTIONS				0x3F
#define L99SM81_GSB_OPTIONS						0x3E //Global status bytes
#define L99SM81_SPI_CPHA_TEST					0x20
#define L99SM81_WD_BIT_POS_14					0x1F
#define L99SM81_WD_BIT_POS_2					0x14
#define L99SM81_WD_BIT_POS_1					0x13
#define L99SM81_WD_TYPE_2						0x12
#define L99SM81_WD_TYPE_1						0x11
#define L99SM81_SPI_MODE						0x10
#define L99SM81_SILICON_VERSION					0x0A
#define L99SM81_DEVICE_8_REG					0x09
#define L99SM81_DEVICE_7_REG					0x08
#define L99SM81_DEVICE_6_REG					0x07
#define L99SM81_DEVICE_5_REG					0x06
#define L99SM81_DEVICE_4_REG					0x05
#define L99SM81_DEVICE_3_REG					0x04
#define L99SM81_DEVICE_2_REG					0x03
#define L99SM81_DEVICE_1_REG					0x02
#define L99SM81_DEVICE_FAM_REG				    0x01
#define L99SM81_DEVICE_FAM_COMPANY_CODE			0x00

//Masks
#define L99SM81_GLOBAL_STATUS					(1 << 23)
#define L99SM81_RESET_BIT						(1 << 22)
#define L99SM81_SPI_ERROR						(1 << 21)
#define L99SM81_FUNCTIONAL_ERR					(1 << 19) //Hey, This is set for the stall detection
#define L99SM81_DEVICE_ERR						(1 << 18)
#define L99SM81_GLOBAL_WARNING					(1 << 17)



//Registers

//GSR: Global status register
#define L99SM81_GSR								0x01
#define L99SM81_VS_OVER_ERR						(15)
#define L99SM81_VS_UNDER_ERR					(14)
#define L99SM81_VREG_OVER_ERR					(13)
#define L99SM81_VREG_UNDER_WARNING				(12)
#define L99SM81_CHARGE_PUMP_ERR					(10)
#define L99SM81_5V_REG_UNDER_WARNING			(9)
#define L99SM81_5V_REG_OVER_ERR					(8)
#define L99SM81_5V_REG_UNDER_ERR				(7)
#define L99SM81_THERMAL_WARNING					(6)
#define L99SM81_THERMAL_SHUTDOWN				(5)
#define L99SM81_OPEN_LOAD						(4)
#define L99SM81_OVER_CURRENT					(3)
#define L99SM81_STALL_DETECTION_FLAG			(2)


//Motor and driver status register MSR
#define L99SM81_MSR								0x02
#define L99SM81_OCA1HS							(15)
#define L99SM81_OCA1LS							(14)
#define L99SM81_OCA2HS							(13)
#define L99SM81_OCA2LS							(12)
#define L99SM81_OCB1HS							(11)
#define L99SM81_OCB1LS							(10)
#define L99SM81_OCB2HS							(9)
#define L99SM81_OCB2LS							(8)
#define L99SM81_OLA								(7)
#define L99SM81_OLB								(6)
#define L99SM81_CVULF							(3)
#define L99SM81_CVLLAF							(2)
#define L99SM81_CVLLBF							(1)


//Global config register 1
#define L99SM81_GCR1						    0x03 
#define L99SM81_CHARGE_PUMP_WOBBLE_FREQ_ENABLE	(15)
#define L99SM81_CPWBE							(15)
#define L99SM81_WOBBLE_FREQ_ENABLE				(14)
#define L99SM81_MWBE							(14)
#define L99SM81_AOUT1							(12)
#define L99SM81_AOUT0							(11)
#define L99SM81_5V_REG_ENABLE					(10)
#define L99SM81_V5VE							(10)
#define L99SM81_MX1								(5)
#define L99SM81_MX2								(3)
#define L99SM81_MX3_1							(2)
#define L99SM81_MX3_0							(1)


//Global config register 2
#define L99SM81_GCR2							0x04 
#define L99SM81_DOUT11							(11)
#define L99SM81_DOUT10							(10)
#define L99SM81_DOUT21							(8)
#define L99SM81_DOUT20							(7)



//MOTOR CONTROL REGISTER 1
#define L99SM81_MCR1							0x05
#define L99SM81_ME								(15)
#define L99SM81_HOLDM							(14)
#define L99SM81_ASM2							(13)
#define L99SM81_ASM1							(12)
#define L99SM81_ASM0							(11)
#define L99SM81_SM2								(10)
#define L99SM81_SM1								(9)
#define L99SM81_SM0								(8)
#define L99SM81_DIR								(7)
#define L99SM81_PH5								(6)
#define L99SM81_PH4								(5)
#define L99SM81_PH3								(4)
#define L99SM81_PH2								(3)
#define L99SM81_PH1								(2)
#define L99SM81_PH0								(1)

//Alternative Step Mode (active step mode if MX1 is set and MX3 = 01b and CTRL3 is high)
#define L99SM81_ALT_16th_MICRO_STEP	(0b111 << L99SM81_ASM0)
#define L99SM81_ALT_8th_MICRO_STEP  (0b001 << L99SM81_ASM0)
#define L99SM81_ALT_MINI_STEP		(0b010 << L99SM81_ASM0)
#define L99SM81_ALT_HALF_STEP		(0b011 << L99SM81_ASM0)
#define L99SM81_ALT_FULL_STEP		(0b100 << L99SM81_ASM0)

//Step modes
#define L99SM81_16th_MICRO_STEP	(0b111 << L99SM81_SM0)
#define L99SM81_8th_MICRO_STEP  (0b001 << L99SM81_SM0)
#define L99SM81_MINI_STEP		(0b010 << L99SM81_SM0)
#define L99SM81_HALF_STEP		(0b011 << L99SM81_SM0)
#define L99SM81_FULL_STEP		(0b100 << L99SM81_SM0)

//MOTOR CONTROL REGISTER 2
#define L99SM81_MCR2							0x06
#define L99SM81_FREQ1							(15)
#define L99SM81_FREQ0							(14)
#define L99SM81_FTOCE							(13)
#define L99SM81_TBE								(12)
#define L99SM81_FT1								(11)
#define L99SM81_FT0								(10)
#define L99SM81_SR1								(9)
#define L99SM81_SR0								(8)
#define L99SM81_DMR1							(7)
#define L99SM81_DMR0							(6)
#define L99SM81_SDAFW							(5)
#define L99SM81_SDBFW							(4)
#define L99SM81_OLDLY							(3)
#define L99SM81_DMH								(2)


//MOTOR CONTROL REGISTER 3
#define L99SM81_MCR3							0x07
#define L99SM81_CVE								(15)
#define L99SM81_D4								(13)
#define L99SM81_D3								(12)
#define L99SM81_D2								(11)
#define L99SM81_D1								(10)
#define L99SM81_D0								(9)
#define L99SM81_SD2								(8)
#define L99SM81_SD1								(7)
#define L99SM81_SD0								(6)
#define L99SM81_CVLUR1							(5)
#define L99SM81_CVLUR0							(4)
#define L99SM81_AHMSD							(3)



//MOTOR CURRENT REFERENCE REGISTER
#define L99SM81_MCREF							0x08
#define L99SM81_HC3								(15)
#define L99SM81_HC2								(14)
#define L99SM81_HC1								(13)
#define L99SM81_HC0								(12)
#define L99SM81_CA3								(4)
#define L99SM81_CA2								(3)
#define L99SM81_CA1								(2)
#define L99SM81_CA0								(1)



//MOTOR COIL VOLTAGE 0 DEGREES REG
#define L99SM81_MCVA							0x09
#define L99SM81_CV9								(10)
#define L99SM81_CV8								(9)
#define L99SM81_CV7								(8)
#define L99SM81_CV6								(7)
#define L99SM81_CV5								(6)
#define L99SM81_CV4								(5)
#define L99SM81_CV3								(4)
#define L99SM81_CV2								(3)
#define L99SM81_CV1								(2)
#define L99SM81_CV0								(1)



//MOTOR COIL VOLTAGE 90 DEGREES REG
#define L99SM81_MCVB							0x0A
#define L99SM81_CV9								(10)
#define L99SM81_CV8								(9)
#define L99SM81_CV7								(8)
#define L99SM81_CV6								(7)
#define L99SM81_CV5								(6)
#define L99SM81_CV4								(5)
#define L99SM81_CV3								(4)
#define L99SM81_CV2								(3)
#define L99SM81_CV1								(2)
#define L99SM81_CV0								(1)


//MOTOR COIL VOLTAGE 180 DEGREES REG
#define L99SM81_MCVC							0x0B
#define L99SM81_CV9								(10)
#define L99SM81_CV8								(9)
#define L99SM81_CV7								(8)
#define L99SM81_CV6								(7)
#define L99SM81_CV5								(6)
#define L99SM81_CV4								(5)
#define L99SM81_CV3								(4)
#define L99SM81_CV2								(3)
#define L99SM81_CV1								(2)
#define L99SM81_CV0								(1)


//MOTOR COIL VOLTAGE 270 DEGREES REG
#define L99SM81_MCVD							0x0C
#define L99SM81_CV9								(10)
#define L99SM81_CV8								(9)
#define L99SM81_CV7								(8)
#define L99SM81_CV6								(7)
#define L99SM81_CV5								(6)
#define L99SM81_CV4								(5)
#define L99SM81_CV3								(4)
#define L99SM81_CV2								(3)
#define L99SM81_CV1								(2)
#define L99SM81_CV0								(1)
					 
//MOTOR COIL VOLTAGE LOW LIMIT B
#define L99SM81_MCVLLB							0x0D
#define L99SM81_CVLLB9							(10)
#define L99SM81_CVLLB8							(9)
#define L99SM81_CVLLB7							(8)
#define L99SM81_CVLLB6							(7)
#define L99SM81_CVLLB5							(6)
#define L99SM81_CVLLB4							(5)
#define L99SM81_CVLLB3							(4)
#define L99SM81_CVLLB2							(3)
#define L99SM81_CVLLB1							(2)
#define L99SM81_CVLLB0							(1)


//MOTOR COIL VOLTAGE LOW LIMIT A
#define L99SM81_MCVLLA							0x0E
#define L99SM81_CVLLA9							(10)
#define L99SM81_CVLLA8							(9)
#define L99SM81_CVLLA7							(8)
#define L99SM81_CVLLA6							(7)
#define L99SM81_CVLLA5							(6)
#define L99SM81_CVLLA4							(5)
#define L99SM81_CVLLA3							(4)
#define L99SM81_CVLLA2							(3)
#define L99SM81_CVLLA1							(2)
#define L99SM81_CVLLA0							(1)


//MOTOR COIL VOLTAGE UPPER LIMIT 
#define L99SM81_MCVUL							0x0F
#define L99SM81_CVUL9							(10)
#define L99SM81_CVUL8							(9)
#define L99SM81_CVUL7							(8)
#define L99SM81_CVUL6							(7)
#define L99SM81_CVUL5							(6)
#define L99SM81_CVUL4							(5)
#define L99SM81_CVUL3							(4)
#define L99SM81_CVUL2							(3)
#define L99SM81_CVUL1							(2)
#define L99SM81_CVUL0							(1)

#define L99SM81_PARITY_BIT (0)


#define L99SM81_DOUT1_OFF						(0b00 << L99SM81_DOUT10)
#define L99SM81_DOUT1_CVRDY						(0b01 << L99SM81_DOUT10)
#define L99SM81_DOUT1_CVLL						(0b10 << L99SM81_DOUT10)
#define L99SM81_DOUT1_CVRUN						(0b11 << L99SM81_DOUT10)
#define L99SM81_DOUT2_OFF						(0b00 << L99SM81_DOUT20)
#define L99SM81_DOUT2_PWM						(0b01 << L99SM81_DOUT20)
#define L99SM81_DOUT2_ERR						(0b10 << L99SM81_DOUT20)
#define L99SM81_DOUT2_EC						(0b11 << L99SM81_DOUT20)


typedef struct L99SM81FRAMES {
	
	uint8_t opCode:2;
	uint8_t address:6;
	uint8_t dataByte1;
	uint8_t dataByte2;
	
} L99SM81_frame_t;


inline L99SM81_frame_t L99SM81_CreateClearAllStatusFrame() {
	L99SM81_frame_t frame;
	frame.opCode    = 0b10;
	frame.address   = 0b111111;
	frame.dataByte1 = 0;
	frame.dataByte2 = 0;
}

inline L99SM81_frame_t L99SM81_CreateResetToDefaultFrame() {
	L99SM81_frame_t frame;
	frame.opCode    = 0b11;
	frame.address   = 0b111111;
	frame.dataByte1 = 0;
	frame.dataByte2 = 0;
}




#endif /* L99SM81_H_ */