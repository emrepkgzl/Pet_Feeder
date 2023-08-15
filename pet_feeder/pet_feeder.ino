/*************************************************************** START OF FILE *******************************************************************/

#include <Wire.h>
#include <DS3231.h>

DS3231 set;
RTClib myRTC;
byte pageNumbers[3] = {0, 0, 0};
char data;
int lineSelector = 0;

void setup () 
{
    Serial.begin(9600);
    Wire.begin();
    setTime(2023, 8, 14, 11, 58, 40);
    //pinMode(D6, OUTPUT);
    delay(100);
    showMenu();
}

void loop () 
{
    //showDateTime();
    //showTemperature();
    readChar(); 
}

/*************************************************************** DS3231 FUNCTIONS ***************************************************************/

void setTime(int Year, int Month, int Day, int Hour, int Minute, int Second)
{
    set.setYear(Year - 48);
    set.setMonth(Month);
    set.setDate(Day);
    set.setHour(Hour);
    set.setMinute(Minute);
    set.setSecond(Second);
}

void showDateTime()
{
    DateTime now = myRTC.now();
   
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

void showTemperature()
{
    Serial.print("Temp: ");
    Serial.println(set.getTemperature());
}

/**************************************************************** MENU FUNCTIONS ****************************************************************/

void readChar()
{
  while(Serial.available() > 0)
    {
      data = Serial.read();
      /* increase -> change selected line */
      if(data == 'i')
      {
        lineSelector++;
        showMenu();
      }
      /* okay -> go into menu */
      else if(data == 'o')
      {
        if(pageNumbers[0] == 0)
        {
          pageNumbers[0] = lineSelector + 1;
        }
        else if(pageNumbers[1] == 0)
        {
          pageNumbers[1] = lineSelector + 1;
        }
        else
        {
          pageNumbers[2] = lineSelector + 1;
        }
        lineSelector = 0;
        showMenu();
      }
      /* back -> go back */
      else if(data == 'b')
      {
        if(pageNumbers[1] == 0)
        {
          if(pageNumbers[0] != 0)
          {
            pageNumbers[0] = 0;
          }
        }
        else if(pageNumbers[2] == 0)
        {
          pageNumbers[1] = 0;
        }
        else
        {
          pageNumbers[2] = 0;
        }
        lineSelector = 0;
        showMenu();
      }
    }
}

void showMenu()
{
  if(pageNumbers[2] == 0)
  {
        /* if any sub menu is not selected */
    if(pageNumbers[1] == 0)
    {
      /* main menu */
      if(pageNumbers[0] == 0)
      {
        if(lineSelector == 3)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Date") :      Serial.println("   Set Date");
        lineSelector == 1 ? Serial.println("-> Set Time") :      Serial.println("   Set Time");
        lineSelector == 2 ? Serial.println("-> Set Feed Time") : Serial.println("   Set Feed Time");
        Serial.println("////////////////////////////");
      }

      /* date menu */
      if(pageNumbers[0] == 1)
      {
        if(lineSelector == 3)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Year") :  Serial.println("   Set Year");
        lineSelector == 1 ? Serial.println("-> Set Month") : Serial.println("   Set Month");
        lineSelector == 2 ? Serial.println("-> Set Day") :   Serial.println("   Set Day");
        Serial.println("////////////////////////////");
      }

      /* time menu */
      if(pageNumbers[0] == 2)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Hour") :   Serial.println("   Set Hour");
        lineSelector == 1 ? Serial.println("-> Set Minute") : Serial.println("   Set Minute");
        Serial.println("////////////////////////////");
      }

      /* time menu */
      if(pageNumbers[0] == 3)
      {
        if(lineSelector == 5)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Morning Time") :      Serial.println("   Set Morning Time");
        lineSelector == 1 ? Serial.println("-> Set Night Time") :        Serial.println("   Set Night Time");
        lineSelector == 2 ? Serial.println("-> Set Feeding Interval") :  Serial.println("   Set Feeding Interval");
        lineSelector == 3 ? Serial.println("-> Set Extra Feeding") :     Serial.println("   Set Extra Feeding");
        lineSelector == 4 ? Serial.println("-> Delete Extra Feedings") : Serial.println("   Delete Extra Feedings");
        Serial.println("////////////////////////////");
      }
    }

    /* date menu */
    else if(pageNumbers[0] == 1)
    {
      /* year menu */
      if(pageNumbers[1] == 1)
      {
        if(lineSelector == 30)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 2023");
          Serial.println("   2024");
          Serial.println("   2025");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   2023");
          Serial.println("-> 2024");
          Serial.println("   2025");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   2023");
          Serial.println("   2024");
          Serial.println("-> 2025");
        }
        else
        {
          Serial.println("   " + String(2021 + lineSelector));
          Serial.println("   " + String(2022 + lineSelector));
          Serial.println("-> " + String(2023 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }

      /* month menu */
      if(pageNumbers[1] == 2)
      {
        if(lineSelector == 12)
        {
          lineSelector = 0;
        }
        if(lineSelector == 0)
        {
          Serial.println("-> 01");
          Serial.println("   02");
          Serial.println("   03");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   01");
          Serial.println("-> 02");
          Serial.println("   03");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   01");
          Serial.println("   02");
          Serial.println("-> 03");
        }
        else
        {
          (lineSelector - 2) < 9 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector - 1) < 9 ? Serial.println("   0" + String( 0 + lineSelector)) : Serial.println("   " + String( 0 + lineSelector));
          (lineSelector    ) < 9 ? Serial.println("-> 0" + String( 1 + lineSelector)) : Serial.println("-> " + String( 1 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }

      /* day menu */
      if(pageNumbers[1] == 3)
      {
        if(lineSelector == 31)
        {
          lineSelector = 0;
        }
        if(lineSelector == 0)
        {
          Serial.println("-> 01");
          Serial.println("   02");
          Serial.println("   03");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   01");
          Serial.println("-> 02");
          Serial.println("   03");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   01");
          Serial.println("   02");
          Serial.println("-> 03");
        }
        else
        {
          (lineSelector - 2) < 9 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector - 1) < 9 ? Serial.println("   0" + String( 0 + lineSelector)) : Serial.println("   " + String( 0 + lineSelector));
          (lineSelector    ) < 9 ? Serial.println("-> 0" + String( 1 + lineSelector)) : Serial.println("-> " + String( 1 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }
    }

    /* time menu */
    else if(pageNumbers[0] == 2)
    {
      /* hour menu */
      if(pageNumbers[1] == 1)
      {
        if(lineSelector == 24)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   01");
          Serial.println("   02");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 01");
          Serial.println("   02");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   01");
          Serial.println("-> 02");
        }
        else
        {
          (lineSelector - 2) < 10 ? Serial.println("   0" + String(-2 + lineSelector)) : Serial.println("   " + String(-2 + lineSelector));
          (lineSelector - 1) < 10 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector    ) < 10 ? Serial.println("-> 0" + String( 0 + lineSelector)) : Serial.println("-> " + String( 0 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }

      /* minute menu */
      if(pageNumbers[1] == 2)
      {
        if(lineSelector == 60)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   01");
          Serial.println("   02");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 01");
          Serial.println("   02");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   01");
          Serial.println("-> 02");
        }
        else
        {
          (lineSelector - 2) < 10 ? Serial.println("   0" + String(-2 + lineSelector)) : Serial.println("   " + String(-2 + lineSelector));
          (lineSelector - 1) < 10 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector    ) < 10 ? Serial.println("-> 0" + String( 0 + lineSelector)) : Serial.println("-> " + String( 0 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }
    }

    /* feed menu */
    else if(pageNumbers[0] == 3)
    {
      /* morning time menu */
      if(pageNumbers[1] == 1)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Hour") :   Serial.println("   Set Hour");
        lineSelector == 1 ? Serial.println("-> Set Minute") : Serial.println("   Set Minute");
        Serial.println("////////////////////////////");
      }

      /* night time menu */
      if(pageNumbers[1] == 2)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Hour") :   Serial.println("   Set Hour");
        lineSelector == 1 ? Serial.println("-> Set Minute") : Serial.println("   Set Minute");
        Serial.println("////////////////////////////");
      }

      /* time interval menu */
      if(pageNumbers[1] == 3)
      {
        if(lineSelector == 24)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 10");
          Serial.println("   20");
          Serial.println("   30");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   10");
          Serial.println("-> 20");
          Serial.println("   30");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   10");
          Serial.println("   20");
          Serial.println("-> 30");
        }
        else
        {
          Serial.println("   " + String(-10 + (lineSelector * 10)));
          Serial.println("   " + String(  0 + (lineSelector * 10)));
          Serial.println("-> " + String( 10 + (lineSelector * 10)));
        }
        Serial.println("////////////////////////////");
      }

      /* extra feeding time menu */
      if(pageNumbers[1] == 4)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Set Hour") :   Serial.println("   Set Hour");
        lineSelector == 1 ? Serial.println("-> Set Minute") : Serial.println("   Set Minute");
        Serial.println("////////////////////////////");
      }

      /* delete extra feedings menu */
      if(pageNumbers[1] == 5)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        lineSelector == 0 ? Serial.println("-> Keep extra feedings") :   Serial.println("   Keep extra feedings");
        lineSelector == 1 ? Serial.println("-> Delete extra feedings") : Serial.println("   Delete extra feedings");
        Serial.println("////////////////////////////");
      }
    }
  }
  else if(pageNumbers[0] == 3)
  {
    /* set morning time */
    if(pageNumbers[1] == 1)
    {
      /* set morning time hour */
      if(pageNumbers[2] == 1)
      {
        if(lineSelector == 24)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   01");
          Serial.println("   02");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 01");
          Serial.println("   02");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   01");
          Serial.println("-> 02");
        }
        else
        {
          (lineSelector - 2) < 10 ? Serial.println("   0" + String(-2 + lineSelector)) : Serial.println("   " + String(-2 + lineSelector));
          (lineSelector - 1) < 10 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector    ) < 10 ? Serial.println("-> 0" + String( 0 + lineSelector)) : Serial.println("-> " + String( 0 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }

      /* set morning time minute */
      else if(pageNumbers[2] == 2)
      {
        if(lineSelector == 6)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   10");
          Serial.println("   20");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 10");
          Serial.println("   20");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   10");
          Serial.println("-> 20");
        }
        else
        {
          Serial.println("   " + String(-20 + (lineSelector * 10)));
          Serial.println("   " + String(-10 + (lineSelector * 10)));
          Serial.println("-> " + String(  0 + (lineSelector * 10)));
        }
        Serial.println("////////////////////////////");
      }
    }

    /* set night time */
    else if(pageNumbers[1] == 2)
    {
      /* set night time hour */
      if(pageNumbers[2] == 1)
      {
        if(lineSelector == 24)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   01");
          Serial.println("   02");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 01");
          Serial.println("   02");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   01");
          Serial.println("-> 02");
        }
        else
        {
          (lineSelector - 2) < 10 ? Serial.println("   0" + String(-2 + lineSelector)) : Serial.println("   " + String(-2 + lineSelector));
          (lineSelector - 1) < 10 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector    ) < 10 ? Serial.println("-> 0" + String( 0 + lineSelector)) : Serial.println("-> " + String( 0 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }

      /* set night time minute */
      else if(pageNumbers[2] == 2)
      {
        if(lineSelector == 6)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   10");
          Serial.println("   20");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 10");
          Serial.println("   20");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   10");
          Serial.println("-> 20");
        }
        else
        {
          Serial.println("   " + String(-20 + (lineSelector * 10)));
          Serial.println("   " + String(-10 + (lineSelector * 10)));
          Serial.println("-> " + String(  0 + (lineSelector * 10)));
        }
        Serial.println("////////////////////////////");
      }
    }

    /* set extra feeding time */
    else if(pageNumbers[1] == 4)
    {
      /* set extra feeding time hour */
      if(pageNumbers[2] == 1)
      {
        if(lineSelector == 24)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   01");
          Serial.println("   02");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 01");
          Serial.println("   02");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   01");
          Serial.println("-> 02");
        }
        else
        {
          (lineSelector - 2) < 10 ? Serial.println("   0" + String(-2 + lineSelector)) : Serial.println("   " + String(-2 + lineSelector));
          (lineSelector - 1) < 10 ? Serial.println("   0" + String(-1 + lineSelector)) : Serial.println("   " + String(-1 + lineSelector));
          (lineSelector    ) < 10 ? Serial.println("-> 0" + String( 0 + lineSelector)) : Serial.println("-> " + String( 0 + lineSelector));
        }
        Serial.println("////////////////////////////");
      }

      /* set extra feeding time minute */
      else if(pageNumbers[2] == 2)
      {
        if(lineSelector == 6)
        {
          lineSelector = 0;
        }
        Serial.println("////////////////////////////");
        if(lineSelector == 0)
        {
          Serial.println("-> 00");
          Serial.println("   10");
          Serial.println("   20");
        }
        else if(lineSelector == 1)
        {
          Serial.println("   00");
          Serial.println("-> 10");
          Serial.println("   20");
        }
        else if(lineSelector == 2)
        {
          Serial.println("   00");
          Serial.println("   10");
          Serial.println("-> 20");
        }
        else
        {
          Serial.println("   " + String(-20 + (lineSelector * 10)));
          Serial.println("   " + String(-10 + (lineSelector * 10)));
          Serial.println("-> " + String(  0 + (lineSelector * 10)));
        }
        Serial.println("////////////////////////////");
      }
    }
  }
}

/*********************************************************************** END OF FILE ************************************************************************/