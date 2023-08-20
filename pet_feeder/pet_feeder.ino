/*************************************************************** START OF FILE *******************************************************************/
#include <Preferences.h>
#include <Wire.h>
#include <DS3231.h>

#define DRIVER_PIN    10

DS3231 set;
RTClib myRTC;
Preferences pref;
char data;
byte pageNumbers[3] = {0, 0, 0};
int feed_counter, interval, value = -1, lineSelector = 0;

typedef struct
{
  byte hr;
  byte min;
}
_time;

void setup () 
{
    Serial.begin(9600);
    Wire.begin();
    // setTime(2023, 8, 14, 11, 58, 40);
    // pref.begin("pet_feeder", false);
    // pinMode(DRIVER_PIN, OUTPUT);
    // /* check if there is feed times saved */
    // byte *pDummy;
    // if(!getFeedTimes(pDummy, pDummy, pDummy, pDummy))
    // {
    //   //TODO
    // }
    // else
    // {
    //   byte temp = checkMisFeeding();
    //   if(temp != -1)
    //   {
    //     misFeedingHandler(temp);
    //   }
    // }

    //pinMode(D6, OUTPUT);
    delay(100);
    showMenu();
}

_time morning, night, extra;

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

/************************************************************** FEED TIME FUNCTIONS *************************************************************/

bool getFeedTimes(byte *pCounter, byte *pHours, byte *pMinutes, byte *pChecks)
{
    *pCounter = pref.getInt("feed_counter", 0);
    if(feed_counter > 0)
    {
      byte feed_hours[*pCounter];
      byte feed_minutes[*pCounter];
      byte feed_checks[*pCounter];

      pref.getBytes("feed_hours", feed_hours, *pCounter);
      pref.getBytes("feed_minutes", feed_minutes, *pCounter);
      pref.getBytes("feed_checks", feed_checks, *pCounter);

      pHours = &feed_hours[0];
      pMinutes = &feed_minutes[0];
      pChecks = &feed_checks[0];
    }
    else
    {
      return false;
    }
    return true;
}

bool calculateFeedTimes()
{
  if(interval > 0)
  {
    byte temp_hr, temp_min;
    temp_hr = morning.hr;
    temp_min = morning.min;

    byte feed_counter2 = (night.hr - morning.hr) * 60 + (night.min - morning.min);

    byte feed_hours2[feed_counter2];
    byte feed_minutes2[feed_counter2];
    byte feed_checks2[feed_counter2];

    for(byte i = 0; i < feed_counter2; i++)
    {
      /* save values to arrays */
      feed_hours2[i] = temp_hr;
      feed_minutes2[i] = temp_min;
      feed_checks2[i] = 0;

      /* calculate next feeding time */
      temp_min += interval;
      /* if temp_min is more than 60 * x, increase hour */
      if(temp_min > (60 * (temp_hr - morning.hr + 1)))
      {
        temp_hr++;
      }
    }

    /* save values to database */
    pref.putBytes("feed_hours", feed_hours2, feed_counter2);
    pref.putBytes("feed_minutes", feed_minutes2, feed_counter2);
    pref.putBytes("feed_checks", feed_checks2, feed_counter2);

    return true;
  }
  else
  {
    return false;
  }
}

void feedTime(byte sec)
{
  digitalWrite(DRIVER_PIN, HIGH);
  delay(sec * 10);
  digitalWrite(DRIVER_PIN, LOW);
}

byte checkMisFeeding()
{
  byte *feed_counter3;
  byte *pFeed_hours;
  byte *pFeed_minutes;
  byte *pFeed_checks;

  DateTime now2 = myRTC.now();
  byte temp_hr2 = now2.hour();
  byte temp_min2 = now2.minute();

  getFeedTimes(feed_counter3, pFeed_hours, pFeed_minutes, pFeed_checks);

  for(byte j = 0; j < *feed_counter3; j++)
  {
    if((temp_hr2 >= *(pFeed_hours + j)) && (temp_min2 >= *(pFeed_minutes + j)))
    {
      if(*(pFeed_checks + j) == 0)
      {
        return j;
      }
      return -1;
    }
  }
  return -1;
}

void misFeedingHandler(byte feeding_number)
{
  byte *feed_counter4;
  byte *pFeed_hours2;
  byte *pFeed_minutes2;
  byte *pFeed_checks2;

  getFeedTimes(feed_counter4, pFeed_hours2, pFeed_minutes2, pFeed_checks2);

  /* complete misfeeding */
  //feedTime(10);
  /* save feeding */
  *(pFeed_checks2 + feeding_number) = 1;
  pref.remove("feed_checks");
  pref.putBytes("feed_checks", pFeed_checks2, *feed_counter4);

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
        if((pageNumbers[0] == 0) && (value == -1))
        {
          pageNumbers[0] = lineSelector + 1;
        }
        else if((pageNumbers[1] == 0) && (value == -1))
        {
          pageNumbers[1] = lineSelector + 1;
        }
        else if((pageNumbers[2] == 0) && (value == -1))
        {
          pageNumbers[2] = lineSelector + 1;
        }
        /* save value */
        else if(value >= 0)
        {
          /* save date */
          if(pageNumbers[0] == 1)
          {
            /* save year */
            if(pageNumbers[1] == 1)
            {
              set.setYear(value - 48);
              value = -1;
              pageNumbers[1]++;
            }
            /* save month */
            else if(pageNumbers[1] == 2)
            {
              set.setMonth(value);
              value = -1;
              pageNumbers[1]++;
            }
            /* save day */
            else if(pageNumbers[1] == 3)
            {
              set.setDate(value);
              value = -1;
              pageNumbers[1] = 0;
              pageNumbers[0] = 0;
            }
          }
          /* save time */
          else if(pageNumbers[0] == 2)
          {
            /* save hour */
            if(pageNumbers[1] == 1)
            {
              set.setHour(value);
              value = -1;
              pageNumbers[1]++;
            }
            /* save minute */
            else if(pageNumbers[1] == 2)
            {
              set.setMinute(value);
              value = -1;
              pageNumbers[1] = 0;
              pageNumbers[0] = 0;
            }
          }
          /* save feeding settings */
          else if(pageNumbers[0] == 3)
          {
            /* save morning time */
            if(pageNumbers[1] == 1)
            {
              if(pageNumbers[2] == 1)
              {
                morning.hr = value;
                value = -1;
                pageNumbers[2]++;
              }
              else if(pageNumbers[2] == 2)
              {
                morning.min = value;
                value = -1;
                pageNumbers[2] = 0;
                pageNumbers[1] = 0;
              }
            }
            /* save night time */
            else if(pageNumbers[1] == 2)
            {
              if(pageNumbers[2] == 1)
              {
                night.hr = value;
                value = -1;
                pageNumbers[2]++;
              }
              else if(pageNumbers[2] == 2)
              {
                night.min = value;
                value = -1;
                pageNumbers[2] = 0;
                pageNumbers[1] = 0;
              }
            }
            /* save feeding interval */
            else if(pageNumbers[1] == 3)
            {
              interval = value;
              value = -1;
              pageNumbers[1] = 0;
            }
            /* save extra feeding */
            else if(pageNumbers[1] == 4)
            {
              if(pageNumbers[2] == 1)
              {
                extra.hr = value;
                value = -1;
                pageNumbers[2]++;
              }
              else if(pageNumbers[2] == 2)
              {
                extra.min = value;
                value = -1;
                pageNumbers[2] = 0;
                pageNumbers[1] = 0;
              }
            }
            /* delete extra feeding */
            else if(pageNumbers[1] == 5)
            {
              if(value == 1)
              {
                //TODO
                value = -1;
              }
              else
              {
                pageNumbers[1] = 0;
                value = -1;
              }
            }
          }
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

void showOptionsMenu(byte index, String *pMenu_options)
{
  Serial.println("////////////////////////////");
  if(index == 0)
  {
    Serial.println("-> " + *pMenu_options);
    Serial.println("   " + *(pMenu_options + 1));
  }
  else
  {
    Serial.println("   " + *(pMenu_options + index - 1));
    Serial.println("-> " + *(pMenu_options + index));
  }
  Serial.println("////////////////////////////");
}

void showNumbersMenu(byte index, byte multiplier, int firstNum)
{
  Serial.println("////////////////////////////");
  if(index == 0)
  {
    firstNum < 10 ? Serial.println("-> 0" + String(firstNum)) : Serial.println("-> " + String(firstNum));
    (firstNum + (1 * multiplier)) < 10 ? Serial.println("   0" + String(firstNum + (1 * multiplier))) : Serial.println("   " + String(firstNum + (1 * multiplier)));
  }
  else
  {
    (firstNum + ((index - 1) * multiplier)) < 10 ? Serial.println("   0" + String(firstNum + ((index - 1) * multiplier))) : Serial.println("   " + String(firstNum + ((index - 1) * multiplier)));
    (firstNum + (index * multiplier)) < 10 ? Serial.println("-> 0" + String(firstNum + (index * multiplier))) : Serial.println("-> " + String(firstNum + (index * multiplier)));
  }
  Serial.println("////////////////////////////");
}

void showMenu()
{
  String menu_options[5];
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
        menu_options[0] = "Set Date";
        menu_options[1] = "Set Time";
        menu_options[2] = "Set Feed Time";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
      }

      /* date menu */
      if(pageNumbers[0] == 1)
      {
        if(lineSelector == 3)
        {
          lineSelector = 0;
        }
        menu_options[0] = "Set Year";
        menu_options[1] = "Set Month";
        menu_options[2] = "Set Day";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
      }

      /* time menu */
      if(pageNumbers[0] == 2)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        menu_options[0] = "Set Hour";
        menu_options[1] = "Set Minute";
        menu_options[2] = "";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
      }

      /* time menu */
      if(pageNumbers[0] == 3)
      {
        if(lineSelector == 5)
        {
          lineSelector = 0;
        }
        menu_options[0] = "Set Morning Time";
        menu_options[1] = "Set Night Time";
        menu_options[2] = "Set Feeding Interval";
        menu_options[3] = "Set Extra Feeding";
        menu_options[4] = "Delete Extra Feedings";
        showOptionsMenu(lineSelector, &menu_options[0]);
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
        showNumbersMenu(lineSelector, 1, 2023);
        value = lineSelector + 2023;
      }

      /* month menu */
      if(pageNumbers[1] == 2)
      {
        if(lineSelector == 12)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 1, 1);
        value = lineSelector + 1;
      }

      /* day menu */
      if(pageNumbers[1] == 3)
      {
        if(lineSelector == 31)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 1, 1);
        value = lineSelector + 1;
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
        showNumbersMenu(lineSelector, 10, 0);
        value = lineSelector;
      }

      /* minute menu */
      if(pageNumbers[1] == 2)
      {
        if(lineSelector == 60)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 1, 0);
        value = lineSelector;
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
        menu_options[0] = "Set Hour";
        menu_options[1] = "Set Minute";
        menu_options[2] = "";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
      }

      /* night time menu */
      if(pageNumbers[1] == 2)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        menu_options[0] = "Set Hour";
        menu_options[1] = "Set Minute";
        menu_options[2] = "";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
      }

      /* time interval menu */
      if(pageNumbers[1] == 3)
      {
        if(lineSelector == 24)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 10, 10);
        value = (lineSelector + 1) * 10;
      }

      /* extra feeding time menu */
      if(pageNumbers[1] == 4)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        menu_options[0] = "Set Hour";
        menu_options[1] = "Set Minute";
        menu_options[2] = "";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
      }

      /* delete extra feedings menu */
      if(pageNumbers[1] == 5)
      {
        if(lineSelector == 2)
        {
          lineSelector = 0;
        }
        menu_options[0] = "Keep extra feedings";
        menu_options[1] = "Delete extra feedings";
        menu_options[2] = "";
        menu_options[3] = "";
        menu_options[4] = "";
        showOptionsMenu(lineSelector, &menu_options[0]);
        value = lineSelector;
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
        showNumbersMenu(lineSelector, 1, 0);
        value = lineSelector;
      }

      /* set morning time minute */
      else if(pageNumbers[2] == 2)
      {
        if(lineSelector == 6)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 10, 0);
        value = lineSelector * 10;
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
        showNumbersMenu(lineSelector, 1, 0);
        value = lineSelector;
      }

      /* set night time minute */
      else if(pageNumbers[2] == 2)
      {
        if(lineSelector == 6)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 10, 0);
        value = lineSelector * 10;
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
        showNumbersMenu(lineSelector, 1, 0);
        value = lineSelector;
      }

      /* set extra feeding time minute */
      else if(pageNumbers[2] == 2)
      {
        if(lineSelector == 6)
        {
          lineSelector = 0;
        }
        showNumbersMenu(lineSelector, 10, 0);
        value = (lineSelector + 1) * 10;
      }
    }
  }
  // Serial.print("temp: ");
  // Serial.println(value);
  // Serial.print("pageNumbers: {");
  // Serial.print(pageNumbers[0]);
  // Serial.print(", ");
  // Serial.print(pageNumbers[1]);
  // Serial.print(", ");
  // Serial.print(pageNumbers[2]);
  // Serial.println("}");
}

/*********************************************************************** END OF FILE ************************************************************************/