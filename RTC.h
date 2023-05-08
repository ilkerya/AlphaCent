 
//https://github.com/adafruit/Data-Logger-shield
// https://forum.arduino.cc/index.php?topic=523216.0
#if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
    RTC_PCF8523 rtc; // I2c Addres 0x68
#endif

#ifdef STM32_F407
  #include <STM32RTC.h>
  STM32RTC& rtc = STM32RTC::getInstance();

#endif 
void  RTC_Read_2Buffer(void){
  #ifdef STM32_F407
      rtc.setHours(DateTimeBuf.Hour);
      rtc.setMinutes(DateTimeBuf.Minute);
      rtc.setSeconds(DateTimeBuf.Second);
   //   rtc.setWeekDay(DateTimeBuf.weekDay);
      rtc.setDay(DateTimeBuf.Day);
      rtc.setMonth(DateTimeBuf.Month);
      rtc.setYear(DateTimeBuf.Year-2000);
#endif 
#if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
    rtc.adjust(DateTime(DateTimeBuf.Year, DateTimeBuf.Month, DateTimeBuf.Day, DateTimeBuf.Hour, DateTimeBuf.Minute, DateTimeBuf.Second));
#endif    
}

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//static const char char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesdy", "Wedns.", "Thurs.", "Friday", "Satur."};

 
void RTC_TimeClock(){
    #ifdef STM32_F407
    if(DateTimeBuf.RTC_Update == ON){
      DateTimeBuf.RTC_Update = OFF;
      RTC_Read_2Buffer();
      return;
  }  
  /*
     Serial.print(F("micros(): "));
     uint32_t Time_diff  = micros();  
   Serial.println(micros());
  */
   uint16_t Year = rtc.getYear();
   uint8_t Month = rtc.getMonth();  
   uint8_t Day = rtc.getDay();
   uint8_t Hour = rtc.getHours();
   uint8_t Minute = rtc.getMinutes();
   uint8_t Second = rtc.getSeconds(); 
    Year += 2000;    
  // if(Year < 2020)Year = 2020;
  // if(Year > 2030)Year = 2030;
     Str_Date = String(Year);   
    Str_Date += '-';    
    if(Month<10)Str_Date += '0';  
     Str_Date += String(Month);
     Str_Date += '-';       
    if(Day<10)Str_Date += '0';  
     Str_Date += String(Day);
     
     Str_Time = "";
    if(Hour<10)Str_Time += '0';
     Str_Time += String(Hour);
      Str_Time += ':';    
    if(Minute<10)Str_Time += '0';   
     Str_Time += String(Minute);
    Str_Time += ':';   
    if(Second<10)Str_Time += '0';   
     Str_Time += String(Second);
  /*
 //  Time_diff = millis() - Time_diff;
   Time_diff = micros() - Time_diff;
   Serial.print(F("Time_diff: "));
   Serial.println(Time_diff);
      */ 
      if(DateTimeBuf.Init == ON){
        DateTimeBuf.Init = OFF;
        DateTimeBuf.Year = Year;
        DateTimeBuf.Month = Month;
        DateTimeBuf.Day = Day;
        DateTimeBuf.Hour = Hour;
        DateTimeBuf.Minute = Minute;
        DateTimeBuf.Second = Second;
    }
   #endif 
   
  #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
  if(DateTimeBuf.RTC_Update == ON){
    DateTimeBuf.RTC_Update = OFF;
     RTC_Read_2Buffer();
    return;
  }
    DateTime now = rtc.now();
    Str_Date = String(now.year(),DEC);   
    Str_Date += '-';    
    if(now.month()<10)Str_Date += '0';  
     Str_Date += String(now.month(),DEC);
     Str_Date += '-';       
    if(now.day()<10)Str_Date += '0';  
     Str_Date += String(now.day(),DEC);
     
     Str_Time = "";
    if(now.hour()<10)Str_Time += '0';
     Str_Time += String(now.hour(),DEC);
      Str_Time += ':';    
    if(now.minute()<10)Str_Time += '0';   
     Str_Time += String(now.minute(),DEC);
    Str_Time += ':';   
    if(now.second()<10)Str_Time += '0';   
     Str_Time += String(now.second(),DEC);
   
    if(DateTimeBuf.Init == ON){
      DateTimeBuf.Init = OFF;
      DateTimeBuf.Year = now.year();
      DateTimeBuf.Month = now.month();
      DateTimeBuf.Day = now.day();
      DateTimeBuf.Hour = now.hour();
      DateTimeBuf.Minute = now.minute();
      DateTimeBuf.Second = now.second();
    }
   #endif 
}

void  RTC_Init(){
  #ifdef STM32_F407
  // Set the time
  rtc.begin(); // initialize RTC 24H format
  
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // Set the time

   #endif 
  
    #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
  delay(10);
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    //while (1);
  }
  else {
    if (! rtc.initialized()) {
      Serial.println(F("RTC is NOT running!"));
      // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      //   RTC_Test = 18;
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    else{
      Serial.println(F("RTC is Running!"));
    } 

    if(Serial) { // if serial port connected adjust it from computer
    //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
    //  Serial.println("RTC Time Adjusted!");
    //  RTC_Test = 18;
    }
  }
  #endif
}


 void SerialPortRx() {
  //   #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
     
    static uint8_t ndx = 0;
    char endMarker = '\n';
    uint8_t Counter = 0;

        // if (Serial.available() > 0) {
     while (Serial.available() > 0 && newData == false) {
        char rc = Serial.read();
        Counter++; // 2020,05,27,21,14,23 19 characters + \0' // in total 21

        if (rc != endMarker) {
            rxarr[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
              ndx = numChars - 1;
          }
        }
        else {
          rxarr[ndx] = '\0'; // terminate the string
          ndx = 0;
          newData = true;
      }
   }
  //////////////////// end of while loop 
   if (newData == true) {
        //Serial.print(F("This just in .................................. "));
          Serial.println(rxarr);
          newData = false;            
          Serial.print(F("Counter:"));  
          Serial.println(Counter);           
          if((Counter == 21) && (rxarr[4] == ',' )&&(rxarr[7] == ',') && (rxarr[10] == ',')&&  (rxarr[13] == ',' )&& (rxarr[16] == ',' )){
              for(int i = 0; i<32; i ++ ){ // ascii 2 byte
                  rxarr[i] -= 48;          
              }         
            // uint16_t Year=0; uint8_t Month=0; uint8_t Day=0; uint8_t Hour=0; uint8_t Minute=0; uint8_t Second=0;             
              uint16_t Year  = 1000 * rxarr[0];
                Year  += 100 * rxarr[1]; 
                Year  += 10 * rxarr[2];   
                Year  += rxarr[3];         
              uint8_t Month  = 10 * rxarr[5];   Month  += rxarr[6];       
              uint8_t Day  = 10 * rxarr[8];     Day  += rxarr[9];               
              uint8_t Hour  = 10 * rxarr[11];   Hour  += rxarr[12];    
              uint8_t Minute  = 10 * rxarr[14]; Minute  += rxarr[15];       
              uint8_t Second  = 10 * rxarr[17]; Second  += rxarr[18];      

                #ifdef STM32_F407
              rtc.setHours(Hour);
              rtc.setMinutes(Minute);
              rtc.setSeconds(Second);
          //   rtc.setWeekDay(weekDay);
              rtc.setDay(Day);
              rtc.setMonth(Month);
              rtc.setYear(Year-2000);
                #endif
                #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE)   
              rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
                #endif       
              MainMenu = MENU5_SUB7;
              DispExtTimeout();
        }
        // FILEQUE
        if((Counter == 11) && (rxarr[0] == 'F' )&&(rxarr[1] == 'I') && (rxarr[2] == 'L')&&  (rxarr[3] == 'E' )
                                                &&(rxarr[4] == 'Q') && (rxarr[5] == 'U')&&  (rxarr[6] == 'E' )){
            EEProm_Update_FileNo();  
            return;                   
        }  
        if((Counter == 11) && (rxarr[0] == 'D' )&&(rxarr[1] == 'E') && (rxarr[2] == 'V')&& (rxarr[3] == 'I' ) &&(rxarr[4] == 'D' )){   
             EEProm_Update_DevId();                                              
             return;                      
        } 
        // DebugOff
        if((Counter == 9) && (rxarr[0] == 'D' )&&(rxarr[1] == 'e') && (rxarr[2] == 'b')&& (rxarr[3] == 'u' ) &&(rxarr[4] == 'g' )
                          &&(rxarr[5] == 'O') && (rxarr[6] == 'f')&&  (rxarr[7] == 'f' )){     
             EEProm_Update_Debug(OFF); 
             Serial.println(F("SERIAL PORT DEBUG OFF "));
             return; 
 
         }                                
         // DebugOn
        if((Counter == 8) && (rxarr[0] == 'D' )&&(rxarr[1] == 'e') && (rxarr[2] == 'b')&& (rxarr[3] == 'u' ) &&(rxarr[4] == 'g' )
                          &&(rxarr[5] == 'O') && (rxarr[6] == 'n')){     
             EEProm_Update_Debug(ON); 
             Serial.println(F("SERIAL PORT DEBUG ON "));             
             return;  
         }                            
    }
 //  #endif
 }
 void EEProm_Update_FileNo(void){
          NVRam_Write_QueNo(&rxarr[7]);                
          delay(10);
          NVRam_Read_QueNo();
          UpdateLogFileId(); 
          SDCard.LogBootInit = 0; // restart logging from 0           
}
void EEProm_Update_DevId(void){
            NVRam_Write_SerNo(&rxarr[5]); 
            delay(10);
            NVRam_Read_SerNo();
            UpdateLogFileId(); 
            SDCard.LogBootInit = 0; // restart logging from 0  
}
bool DebugState = OFF;
void EEProm_Update_Debug(bool State){
  if(State == ON)DebugState = ON;
  else DebugState = OFF;
}
 
