int sensorValue1 = 0;
int sensorValue2 = 0;
long bekleme = 0L;
int besleme = 0;
long zaman = 0L;
long geceSuresi = 0L;
long gunduzSuresi = 0L;
bool geceBitis = LOW;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);



void setup() {
  // put your setup code here, to run once:

  pinMode(14, INPUT);  //Butonlar
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(15, INPUT);
  
  pinMode(A0, INPUT); //Pot

  pinMode(D3, OUTPUT); //Surucu
  pinMode(D4, OUTPUT);

  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  
  Serial.begin(9600);

  lcd.begin();
  lcd.home();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("EVCIL HAYVAN");
  lcd.setCursor(2, 1);
  lcd.print("MAMA PROJESI");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("BUTONLAR");
  lcd.setCursor(0, 1);
  lcd.print("A    B    C    D");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zamanlama yapmak");
  lcd.setCursor(0 ,1);
  lcd.print("icin: A,");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("zamanlama iptali");
  lcd.setCursor(0 ,1);
  lcd.print("icin: B,");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("manuel itme");
  lcd.setCursor(0 ,1);
  lcd.print("icin: C,");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("manuel geri itme");
  lcd.setCursor(0 ,1);
  lcd.print("icin: D");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("butonlarini");
  lcd.setCursor(0 ,1);
  lcd.print("kullanin.");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zamanlama");
  lcd.setCursor(0 ,1);
  lcd.print("yapilmadi");
  delay(1000);
  Serial.println("sa");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("as");

  if(digitalRead(13) == HIGH || digitalRead(15) == HIGH)
  {
    
    if(digitalRead(13) == HIGH)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ileri itiliyor..");
  
      
      //analogWrite(5, 1);
      //analogWrite(6, 0);

      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
      delay(250);
    }
    
    if(digitalRead(15) == HIGH)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Geri itiliyor...");
      
      //analogWrite(5, 0);
      //analogWrite(6, 1);

      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH);
      delay(250);
    }
    
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zamanlama yapmak");
    lcd.setCursor(0 ,1);
    lcd.print("icin:[A]");
    delay(50);
    
    //analogWrite(5, 0);
    //analogWrite(6, 0);

    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
  }
 


  if(digitalRead(14) == HIGH)  // zamanlamayı kaydetme
  {
    delay(1000);
    while(HIGH) // gece suresi kaydetme
    {
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gece suresini");
      lcd.setCursor(0 ,1);
      lcd.print("ayarlayin:");
      lcd.setCursor(10 ,1);
      geceSuresi = analogRead(A0);
      geceSuresi = map(geceSuresi, 1024, 1, 0, 60);
      lcd.print(geceSuresi);
      lcd.setCursor(12 ,1);
      lcd.print("[A]");
      delay(50);
      
      
      if(digitalRead(14) == HIGH)
      {
        geceSuresi = analogRead(A0);
        geceSuresi = map(geceSuresi, 1024, 1, 0, 60000);
        gunduzSuresi = 30000 - geceSuresi;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gece suresi");
        lcd.setCursor(0 ,1);
        lcd.print("kaydedildi.");
        delay(2000);
        break;
      }
    }
    
    while(HIGH) // bekleme suresi kaydetme
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Bekleme suresini");
      lcd.setCursor(0 ,1);
      lcd.print("ayarlayin:");
      lcd.setCursor(10 ,1);
      bekleme = analogRead(A0);
      bekleme = map(bekleme, 1024, 1, 0, 60);
      lcd.print(bekleme);
      lcd.setCursor(12 ,1);
      lcd.print("[A]");
      delay(50);
      
      if(digitalRead(14) == HIGH)
      {
        bekleme = analogRead(A0);
        bekleme = map(bekleme, 1024, 1, 0, 60);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bekleme suresi");
        lcd.setCursor(0 ,1);
        lcd.print("kaydedildi.");
        delay(2000);
        break;
      }
    }
    
    while(HIGH) // besleme suresi kaydetme
    {
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Besleme suresini");
      lcd.setCursor(0 ,1);
      lcd.print("ayarlayin:");
      lcd.setCursor(10 ,1);
      besleme = analogRead(A0);
      besleme = map(besleme, 1024, 1, 0, 60);
      lcd.print(besleme);
      lcd.setCursor(12 ,1);
      lcd.print("[A]");
      delay(50);
      
      if(digitalRead(14) == HIGH)
      {
        besleme = analogRead(A0);
        besleme = map(besleme, 1024, 1, 0, 60);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Besleme suresi");
        lcd.setCursor(0 ,1);
        lcd.print("kaydedildi.");
        delay(2000);
        break;
      }
    }
  }
  
        if(besleme != 0 || bekleme != 0)  //bu satırdan sonrası zamanlamalı besleme
        {

          while(HIGH)
          {
              geceBitis = LOW;
            
              //digitalWrite(4, LOW);  //GREEN
                //digitalWrite(0, HIGH);
                //digitalWrite(2, LOW);

              //analogWrite(5, 100);  // beslemeyi baslat
              //analogWrite(6, 0);
              digitalWrite(D3, HIGH);
              digitalWrite(D4, LOW);
              for(int i = 0; i < besleme; i++)
              {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Besleme");
                  lcd.setCursor(0 ,1);
                  lcd.print("yapiliyor:");
                  lcd.setCursor(10 ,1);
                  lcd.print(i + 1);
                  lcd.setCursor(12 ,1);
                  lcd.print("[sn]");
                  
                  delay(1000);
                  zaman += 1000;
                  if(digitalRead(12) == HIGH) // zamanlama iptal
                  {
                    bekleme = 0;
                    besleme = 0;
                    break;
                  }
              }
              //analogWrite(5, 0);  // beslemeyi bitir
              //analogWrite(6, 0);
            
              digitalWrite(D3, LOW);
              digitalWrite(D4, LOW);

              if(besleme == 0 || bekleme == 0)
              {
                //analogWrite(5, 0);
                //analogWrite(6, 0);
                break;
              }

              for(int i = 0; i < bekleme; i++)
              {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Bekleniyor...");
                  lcd.setCursor(0 ,1);
                  lcd.print(i + 1);
                  lcd.setCursor(2 ,1);
                  lcd.print("[sn]");
                
                  delay(1000);
                  zaman += 1000;
                  if(digitalRead(12) == HIGH) // zamanlama iptal
                  {
                    bekleme = 0;
                    besleme = 0;
                    break;
                  }
                  if(zaman >= gunduzSuresi)
                  {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Gece");
                    lcd.setCursor(0 ,1);
                    lcd.print("Bekleniyor:");
                    lcd.setCursor(10 ,1);
                    lcd.print(geceSuresi/1000);
                    lcd.setCursor(12 ,1);
                    lcd.print("[sa]");
                    delay(geceSuresi);
                    geceBitis = HIGH;
                    zaman = 0L;
                  }
                  if(geceBitis == HIGH)
                  {
                  break;
                  }
              }

              if(besleme == 0 || bekleme == 0)
              {
                //analogWrite(5, 0);
                //analogWrite(6, 0);

                digitalWrite(D3, LOW);
                digitalWrite(D4, LOW);
                break;
              }

          }

        }
  
  delay(10);

        

}
