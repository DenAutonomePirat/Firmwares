/*!
AWI_Ceech_LTC2946:
Energy Monitor for Ceech LTC2946 Board. Based on the original Linear Technology Corp. Library
LTC2946: 12-Bit Wide Range Power, Charge and Energy Monitor

Uses: I2C interface,
Libraries needed: LTC2946, LTC_I2C (.h & .cpp)

Functions implemented (20150530): 
- Continuous reading of V_IN, A_IN, Power
- (On board GPIO & LEDs)
*/

#include <Arduino.h>
#include <stdint.h>
#include "LTC2946.h"
#include <Wire.h>
#include <SPI.h>


unsigned long DISPLAY_DELAY = 1000;   //  60 sec sleep time between reads (seconds * 1000 milliseconds)

// LTC 2946 board settings and values
const float resistor = .02;                         //!< resistor value on Ceech board
const float CLK_FREQ = 4E6;                         //!< CLK Frequency (Board uses internal LTC2946 clock, so uses default values)
// LSB Weights: calculated for board
const float LTC2946_ADIN_lsb = 5.001221E-04;                      //!< Typical ADIN lsb weight in volts 
const float LTC2946_DELTA_SENSE_lsb = 2.5006105E-05;              //!< Typical Delta lsb weight in volts
const float LTC2946_VIN_lsb = 2.5006105E-02;                      //!< Typical VIN lsb weight in volts
const float LTC2946_Power_lsb = 6.25305E-07;                      //!< Typical POWER lsb weight in V^2 VIN_lsb * DELTA_SENSE_lsb
const float LTC2946_ADIN_DELTA_SENSE_lsb = 1.25061E-08;           //!< Typical sense lsb weight in V^2  *ADIN_lsb * DELTA_SENSE_lsb
const float LTC2946_INTERNAL_TIME_lsb = 4101.00/250000.00;        //!< Internal TimeBase lsb. Use LTC2946_TIME_lsb if an external CLK is used.
//
static float LTC2946_TIME_lsb = 16.39543E-3;                       //!< Static variable which is based off of the default clk frequency of 250KHz.

// Error string
const char ack_error[] = "Error: No Acknowledge. Check I2C Address."; //!< Error message

// Global variables, (board uses internal clock)
static bool internalCLK = true;
// default control register values for LTC2946
// CTRLA = configuration register for:
//    CHANNEL_CONFIG_V_C_3 : voltage current ADC alternate (011), sense_plus (11), OFFSET_CAL_EVERY = calculate offset every conversion(00), Adc with respect to ground
static uint8_t CTRLA = LTC2946_CHANNEL_CONFIG_A_V_C_3|LTC2946_SENSE_PLUS|LTC2946_OFFSET_CAL_EVERY|LTC2946_ADIN_GND;  //! Set Control A register to default value.
// CTRLB = configuration register for 
static uint8_t CTRLB = LTC2946_DISABLE_ALERT_CLEAR&LTC2946_DISABLE_SHUTDOWN&LTC2946_DISABLE_CLEARED_ON_READ&LTC2946_DISABLE_STUCK_BUS_RECOVER&LTC2946_ENABLE_ACC&LTC2946_DISABLE_AUTO_RESET; //! Set Control B Register to default value
// GPIO = configuration register for GPIO (GPIO2 not used on Ceech board)
static uint8_t GPIO_CFG = LTC2946_GPIO1_OUT_LOW |LTC2946_GPIO2_OUT_LOW|LTC2946_GPIO3_OUT_LOW;  //! Set GPIO_CFG Register to all output low (not Default values)
// GPIO3_CTRL = special extra config for port 3
static uint8_t GPIO3_CTRL = LTC2946_GPIO3_OUT_HIGH_Z;                                           //! Set GPIO3_CTRL to Default Value 
// Variable for method of voltage sampling
static uint8_t VOLTAGE_SEL = LTC2946_SENSE_PLUS;                                                //! Set Voltage selection to default value.
static uint8_t shutdown = 0;                                                                    //! Set Shutdown = 1 to put part in shutdown. This is done through menu item 4.

// For ADC input, no voltage divider on this board, so scale = 1
static float scale = 1 ;                                      //! Stores division ration for resistive divider on ADIN pin.  Configured inside "Settings" menu.

// display: init & connections
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
char buf[21];                           // Buffer for printing readings to the LCD.

//! Initialize board
void setup()
{
  Serial.begin(115200);

  Serial.print("AWI_Ceech Started ");
   

  // Init LTC2946 default mode (not really needed)
  uint8_t LTC2946_mode;
  int8_t ack = 0;
  LTC2946_mode = CTRLA;                                                         //! Set the configuration of the CTRLA Register.
  ack |= LTC2946_write(LTC2946_I2C_ADDRESS, LTC2946_CTRLA_REG, LTC2946_mode);   //! Sets the LTC2946 to continuous mode
  // Initialize
  Serial.println(F("Resetting registers, Starting measurement: "));
  resetAccumulators(CTRLB);     // clear accumulated values
 }

//! Repeat loop
void loop()
{
  int8_t ack = 0;                               // I2C acknowledge indicator
  // Initialize & Start
  continuous_mode(CTRLA, VOLTAGE_SEL, scale);  //! Continuous Mode Measurement, default measurement, scaled by factor...
  // configure_GPIO(&GPIO_CFG, &GPIO3_CTRL); // GPIO setting
  printRegisterValues(); // print register values
  delay(DISPLAY_DELAY);             // Delay for display & send
 }

// Function Definitions
// reset accumulators & min/max values
int8_t resetAccumulators(uint8_t CTRLB)
{
  int8_t ack = 0;
  int8_t LTC2946_mode;
  LTC2946_mode = CTRLB | LTC2946_RESET_ALL;                           //! Set the configuration of the CTRLB Register( [1:0] contain reset)
  ack |= LTC2946_write(LTC2946_I2C_ADDRESS, LTC2946_CTRLB_REG, LTC2946_mode);             //! Sets the LTC2946 to continuous mode
  return(ack);
}

//print register values for debug purposes
int8_t printRegisterValues()            
 //! @return Returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
 {
   int8_t ack = 0;
   int8_t LTC2946_mode;
   //ack |= LTC2946_write(LTC2946_I2C_ADDRESS, LTC2946_CLK_DIV_REG, B00010000 ); //Set Clock division);
   Serial.println();
   Serial.print(F(" Internal Clock: "));
   Serial.print(internalCLK, BIN);
   Serial.println(F(" binary"));
   
   
   uint8_t CTRL_A;
   ack |= LTC2946_read(LTC2946_I2C_ADDRESS, LTC2946_CTRLA_REG, &CTRL_A);
   Serial.println();
   Serial.print(F(" Register A: "));
   Serial.print(CTRL_A, BIN);
   Serial.println(F(" binary"));
   
   uint8_t CLK_code;
   ack |= LTC2946_read(LTC2946_I2C_ADDRESS, LTC2946_CLK_DIV_REG, &CLK_code);
   Serial.println();
   Serial.print(F(" CLK_DIV: "));
   Serial.print(CLK_code, BIN);
   Serial.print(F(" binary"));
   
   uint8_t VIN_code;
   ack |= LTC2946_read(LTC2946_I2C_ADDRESS, LTC2946_VIN_LSB_REG, &VIN_code);
   Serial.println();
   Serial.print(F(" VIN MSB: "));
   Serial.print(VIN_code, BIN);
   Serial.print(F(" binary"));
   return(ack);
 }

//! Continuous Mode, print values.
int8_t continuous_mode(uint8_t CTRLA,                    //!< CTRLA Register sets the mode in which Continious measurements are made. Configured in "Settings" menu.
                       uint8_t VOLTAGE_SEL,              //!< VOLTAGE_SEL variable represents the voltage channel selected. Scaling is done if ADIN channel is selected and resistive dividers are present.
                       float scale)                       //!< Stores division ratio for resistive divider on ADIN pin.  Configured inside "Settings" menu.
//! @return Returns the state of the acknowledge bit after the I2C address write. 0=acknowledge, 1=no acknowledge.
{
  int8_t ack = 0;
  char ValA[6], ValB[6], ValC[6] ;            // Temporary string variables for display
  //! Measurement is done by first reading the adc values and then converting it to the respective real values
  if (VOLTAGE_SEL != LTC2946_ADIN) {            // ADC input is selected 
    Serial.println(F("Sense measurements: "));
  }
  else{
  Serial.println(F("ADC measurements: ")); 
  }
  // Power measurement
  uint32_t power_code, max_power_code, min_power_code;
    ack |= LTC2946_read_24_bits(LTC2946_I2C_ADDRESS, LTC2946_POWER_MSB2_REG, &power_code);                 
    ack |= LTC2946_read_24_bits(LTC2946_I2C_ADDRESS, LTC2946_MAX_POWER_MSB2_REG, &max_power_code);        
    ack |= LTC2946_read_24_bits(LTC2946_I2C_ADDRESS, LTC2946_MIN_POWER_MSB2_REG, &min_power_code);       
  // calculate real values from converter values
  float power, max_power, min_power;  // Store power results
    power = LTC2946_code_to_power(power_code, resistor, LTC2946_Power_lsb);                                
  max_power = LTC2946_code_to_power(max_power_code, resistor, LTC2946_Power_lsb);                       
  min_power = LTC2946_code_to_power(min_power_code, resistor, LTC2946_Power_lsb);                       
  
  Serial.println(power);

  // Current measurements
    uint16_t current_code, max_current_code, min_current_code;
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_DELTA_SENSE_MSB_REG, &current_code);
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_MAX_DELTA_SENSE_MSB_REG, &max_current_code);
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_MIN_DELTA_SENSE_MSB_REG, &min_current_code);
  // calculate real values from converter values
    float current, max_current, min_current;
    current = LTC2946_code_to_current(current_code, resistor, LTC2946_DELTA_SENSE_lsb);
    max_current = LTC2946_code_to_current(max_current_code, resistor, LTC2946_DELTA_SENSE_lsb);
    min_current = LTC2946_code_to_current(min_current_code, resistor, LTC2946_DELTA_SENSE_lsb);
  
    Serial.println(current);
 
  // voltage measurements, sense
    uint16_t VIN_code, max_VIN_code, min_VIN_code;
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_VIN_MSB_REG, &VIN_code);
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_MAX_VIN_MSB_REG, &max_VIN_code);
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_MIN_VIN_MSB_REG, &min_VIN_code);
  // calculate real values from converter values
    float VIN, max_VIN, min_VIN;
    VIN = LTC2946_VIN_code_to_voltage(VIN_code , LTC2946_VIN_lsb);
    max_VIN = LTC2946_VIN_code_to_voltage(max_VIN_code, LTC2946_VIN_lsb);
    min_VIN = LTC2946_VIN_code_to_voltage(min_VIN_code, LTC2946_VIN_lsb);
    
    Serial.println(VIN);

  // voltage measurements, ADC
    uint16_t ADIN_code, max_ADIN_code, min_ADIN_code;
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_ADIN_MSB_REG, &ADIN_code);
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_MAX_ADIN_MSB_REG, &max_ADIN_code);
    ack |= LTC2946_read_12_bits(LTC2946_I2C_ADDRESS, LTC2946_MIN_ADIN_MSB_REG, &min_ADIN_code);
  // calculate real values from converter values
    float ADIN, max_ADIN, min_ADIN;
    ADIN = LTC2946_ADIN_code_to_voltage(ADIN_code, LTC2946_ADIN_lsb)*scale;
    max_ADIN = LTC2946_ADIN_code_to_voltage(max_ADIN_code, LTC2946_ADIN_lsb)*scale;
    min_ADIN = LTC2946_ADIN_code_to_voltage(min_ADIN_code, LTC2946_ADIN_lsb)*scale;
  
    Serial.println(ADIN);
  
  // Get Energy, Charge & Time values for calculation
    uint32_t energy_code, charge_code, time_code;
    ack |= LTC2946_read_32_bits(LTC2946_I2C_ADDRESS, LTC2946_ENERGY_MSB3_REG, &energy_code);
    ack |= LTC2946_read_32_bits(LTC2946_I2C_ADDRESS, LTC2946_CHARGE_MSB3_REG, &charge_code);
    ack |= LTC2946_read_32_bits(LTC2946_I2C_ADDRESS, LTC2946_TIME_COUNTER_MSB3_REG, &time_code);
  // calculate real values from converter values
    float energy,charge,time;
    if(internalCLK)             // Time differs form internal or external clock, needs to be rightly adjusted
    {
    energy = LTC2946_code_to_energy(energy_code,resistor,LTC2946_Power_lsb, LTC2946_INTERNAL_TIME_lsb);
    charge = LTC2946_code_to_coulombs(charge_code,resistor,LTC2946_DELTA_SENSE_lsb, LTC2946_INTERNAL_TIME_lsb);
    time = LTC2946_code_to_time(time_code, LTC2946_INTERNAL_TIME_lsb);
    }
    else // External clock
    {
    energy = LTC2946_code_to_energy(energy_code,resistor,LTC2946_Power_lsb, LTC2946_TIME_lsb);
    charge = LTC2946_code_to_coulombs(charge_code,resistor,LTC2946_DELTA_SENSE_lsb, LTC2946_TIME_lsb);
    time = LTC2946_code_to_time(time_code, LTC2946_TIME_lsb);
    }
  // print Energy, Charge and Time values to Serial & LCD
    dtostrf(energy, 5, 2, ValA);
    dtostrf(charge, 5, 2, ValB);
    dtostrf(time, 5, 1, ValC);
    snprintf(buf, sizeof buf, "%5sJ %5sC %5ss", ValA, ValB, ValC);
  Serial.println(buf);
  //lcd.setCursor(0, 3);
    //lcd.print(buf);

  return(ack);
}
