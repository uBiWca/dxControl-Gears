#include <EEPROM.h>
#include "storage.h"
#include "globals.h"
#if defined (MCU_STM32F103C8)
#include "Adafruit_FRAM_SPI.h"
#endif

/*
Takes the current configuration (config pages and maps)
and writes them to EEPROM as per the layout defined in storage.h
*/
void writeConfig(uint8_t thePage)
{
  /*
  We only ever write to the EEPROM where the new value is different from the currently stored byte
  This is due to the limited write life of the EEPROM (Approximately 100,000 writes)
  */
  
 
  //Create a pointer to the config page
  
  void* pnt_configPage;//This only stores the address of the value that it's pointing to and not the max size
  
  if(EEPROM.read(0) != data_structure_version) { EEPROM.write(0,data_structure_version); }   //Write the data structure version
 
    switch (thePage)
          {

          case 1:
          /*---------------------------------------------------
          | Config page 1 (See storage.h for data layout)
          | 128 byte long config table
          -----------------------------------------------------*/
          pnt_configPage = (uint8_t *)&configPage1; //Create a pointer to Page 1 in memory
          for(uint16_t x=EEPROM_CONFIG1_START; x<EEPROM_CONFIG1_END; x++) 
            { 
              if(EEPROM.read(x) != *((uint8_t *)pnt_configPage + (uint8_t)(x - EEPROM_CONFIG1_START))) { EEPROM.write(x, *((uint8_t *)pnt_configPage + (uint8_t)(x - EEPROM_CONFIG1_START))); }
            }
  
          break;

          case 2:
          /*---------------------------------------------------
          | Config page 2 (See storage.h for data layout)
          | 256 byte long config table
          -----------------------------------------------------*/
          pnt_configPage = (uint8_t *)&configPage2; //Create a pointer to Page 2 in memory
          for(uint16_t x=EEPROM_CONFIG2_START; x<EEPROM_CONFIG2_END; x++) 
            { 
              if(EEPROM.read(x) != *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG2_START))) { EEPROM.write(x, *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG2_START))); }
            }
  
          break;

          case 3:
          /*---------------------------------------------------
          | Config page 3 (See storage.h for data layout)
          | 128 byte long config table
          -----------------------------------------------------*/
          pnt_configPage = (uint8_t *)&configPage3; //Create a pointer to Page 3 in memory
          for(uint16_t x=EEPROM_CONFIG3_START; x<EEPROM_CONFIG3_END; x++) 
            { 
              if(EEPROM.read(x) != *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG3_START))) { EEPROM.write(x, *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG3_START))); }
            }
  
          break;
       
          case 4:
          /*---------------------------------------------------
          | Config page 4 (See storage.h for data layout)
          | 256 byte long config table
          -----------------------------------------------------*/
          #if defined (CORE_AVR)
          pnt_configPage = (uint8_t *)&configPage4; //Create a pointer to Page 4 in memory
          #elif defined (CORE_STM32)
          pnt_stm32_configPage = (uint8_t *)&configPage4; //Create a pointer to Page 4 in memory
          #endif
  
          for(uint16_t x=EEPROM_CONFIG4_START; x<EEPROM_CONFIG4_END; x++) 
            { 
            #if defined (CORE_AVR)
              if(EEPROM.read(x) != *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG4_START))) { EEPROM.write(x, *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG4_START))); }
            #elif defined (CORE_STM32)//(MCU_STM32F103C8)
              //if(NVMEMread(x) != *((uint8_t *)pnt_stm32_configPage + (uint8_t)(x - EEPROM_CONFIG4_START)))
              // {     
              //  NVMEMwrite(x, *((uint8_t *)pnt_stm32_configPage + (uint8_t)(x - EEPROM_CONFIG4_START)),0); 
              // }
            #endif
            }
  
          break;
          }
  BIT_SET(currentStatus.systembits, BIT_SYSTEM_BURN_GOOD); //set burn_good flag
}


void loadConfig()
{
  //Create a pointer to the config page
  void* pnt_configPage;

  pnt_configPage = (uint8_t *)&configPage1; //Create a pointer to Page 1 in memory
  for(uint16_t x=EEPROM_CONFIG1_START; x<EEPROM_CONFIG1_END; x++)        // x=100;x<164
     { 
      *((uint8_t *)pnt_configPage + (uint8_t)(x - EEPROM_CONFIG1_START)) = EEPROM.read(x);
     }
  //That concludes the reading of config1

  pnt_configPage = (uint8_t *)&configPage2; //Create a pointer to Page 2 in memory
  for(uint16_t x=EEPROM_CONFIG2_START; x<EEPROM_CONFIG2_END; x++)        // x=200;x<454
     { 
      *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG2_START)) = EEPROM.read(x);
     }
  //That concludes the reading of config2

  pnt_configPage = (uint8_t *)&configPage3; //Create a pointer to Page 3 in memory
  for(uint16_t x=EEPROM_CONFIG3_START; x<EEPROM_CONFIG3_END; x++)        // x=200;x<454
     { 
      *((uint8_t *)pnt_configPage + (uint16_t)(x - EEPROM_CONFIG3_START)) = EEPROM.read(x);
     }
  //That concludes the reading of config3
  
  pnt_configPage = (uint8_t *)&configPage4; //Create a pointer to Page 4 in memory
  for(uint16_t x=EEPROM_CONFIG4_START; x<EEPROM_CONFIG4_END; x++)        // x=500;x<756
     { 
     #if defined (CORE_AVR)
        *((uint8_t *)pnt_configPage + (uint8_t)(x - EEPROM_CONFIG4_START)) = EEPROM.read(x);
     #elif defined (CORE_STM32)    //(MCU_STM32F103C8)
         // *((uint8_t *)pnt_configPage + (uint8_t)(x - EEPROM_CONFIG4_START)) = (uint8_t)NVMEMread(x);
      #endif
      }
  
  //That concludes the reading of config4
  
}

uint8_t NVMEMread(uint16_t address)
{
#if defined (CORE_AVR)
  uint8_t theValue = EEPROM.read(address);
#elif defined (CORE_STM32)  //(MCU_STM32F103C8)
  #if defined (USE_FRAM)
    uint8_t theValue = fram.read8(address);
  #elif defined (USED_EXT_FLASH)
  
  #endif  
#endif  
  return theValue;
}

void NVMEMwrite(uint16_t address, uint8_t dataByte)
{
#if defined (CORE_AVR)
  EEPROM.write(address,dataByte);
#elif defined (CORE_STM32)//(MCU_STM32F103C8)  
  #if defined (USE_FRAM)
    fram.writeEnable(true);
    fram.write8(address, dataByte);
    fram.writeEnable(false);
  #elif defined (USED_EXT_FLASH)
  
  #endif
#endif  
}

