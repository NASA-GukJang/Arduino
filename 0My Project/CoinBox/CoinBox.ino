#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // SDA - A4, SCL - A5

int PI_10 = 2;
int PI_50 = 3;
int PI_100 = 4;
int PI_500 = 5;
int right_Pin = 8;
int left_Pin = 9;
int confirm_Pin = 10;
int return_Pin = 11;

int w10 = 0;
int w50 = 0;
int w100 = 0;
int w500 = 0;

int t1 = 0; // target coin variables
int t2 = 0;
int t3 = 0;
int t4 = 0;
int t5 = 0;
int t6 = 0;

float total = 0;
float target = 0;

boolean targetFlag = false;
int menuFlag = 0;

int up_buttonState = 0;
int up_lastButtonState = 0;
int down_buttonState = 0;
int down_lastButtonState = 0;
int confirm_buttonState = 0;
int confirm_lastButtonState = 0;
int return_buttonState = 0;
int return_lastButtonState = 0;
boolean buttonPress = false;

int systemPhase = 0;
int initializePhase = 0;
int mainPhase = 0;
int menuPhase = 1;
int menuSystemPhase = 0; // Third Menu

void setup(){
  lcd.begin();
  lcd.backlight();
  
  Serial.begin(9600);
  
  pinMode(PI_10, INPUT_PULLUP);
  pinMode(PI_50, INPUT_PULLUP);
  pinMode(PI_100, INPUT_PULLUP);
  pinMode(PI_500, INPUT_PULLUP);

  display_setCoin();
  display_CoinStatus();
}

void loop(){
  if(systemPhase == 0){
    if(!targetFlag) setCoin();
    else setTarget();
  }
  else if(systemPhase == 1){
    main_CoinBank();
  }
  else if(systemPhase == 2){
    if(menuFlag == 0) callMenu();
    else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 1) menu_AdjustStatus();
    else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 2) menu_AdjustTarget();
    else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 3) menu_System();
  }
}

void display_setCoin(){
  lcd.backlight();
  lcd.setCursor(6,0);
  lcd.print("Set");
  lcd.setCursor(2,1);
  lcd.print("Coin Number!");
  delay(3000);
}

void display_CoinStatus(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("10W:");
  lcd.print(w10);
  lcd.setCursor(8,0);
  lcd.print("100W:");
  lcd.print(w100);
  lcd.setCursor(0,1);
  lcd.print("50W:");
  lcd.print(w50);
  lcd.setCursor(8,1);
  lcd.print("500W:");
  lcd.print(w500);
}

void blinkCursor(int x, int y){
  Serial.println("Blink Started");
  lcd.setCursor(x,y);
  lcd.blink();
}

void setCoin(){
  Serial.println("setCoin Started");
  Serial.print("initializePhase : ");
  Serial.println(initializePhase);
  
  if(initializePhase == 0){ // 10
    if(w10 >= 0 && w10 < 10) blinkCursor(4,0);
    else if(w10 >= 10 && w10 < 100) blinkCursor(5,0);
    else blinkCursor(6,0);
  }
  else if(initializePhase == 1){ // 50
    if(w50 >= 0 && w50 < 10) blinkCursor(4,1);
    else if(w50 >= 10 && w50 < 100) blinkCursor(5,1);
    else blinkCursor(6,1);
  }
  else if(initializePhase == 2){ // 100
    if(w100 >= 0 && w100 < 10) blinkCursor(13,0);
    else if(w100 >= 10 && w100 < 100) blinkCursor(14,0);
    else blinkCursor(15,0);
  }
  else if(initializePhase == 3){ // 500
    if(w500 >= 0 && w500 < 10) blinkCursor(13,1);
    else if(w500 >= 10 && w500 < 100) blinkCursor(14,1);
    else blinkCursor(15,1);
  }
  
  buttonUp();
  buttonDown();

  if(buttonPress){
    buttonPress = false;
    if(initializePhase == 0){ // 10
      lcd.setCursor(4,0);
      lcd.print("   ");
      lcd.setCursor(4,0);
      lcd.print(w10);
    }
    else if(initializePhase == 1){ // 50
      lcd.setCursor(4,1);
      lcd.print("   ");
      lcd.setCursor(4,1);
      lcd.print(w50);
    }
    else if(initializePhase == 2){ // 100
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(13,0);
      lcd.print(w100);
    }
    else if(initializePhase == 3){ // 500
      lcd.setCursor(13,1);
      lcd.print("   ");
      lcd.setCursor(13,1);
      lcd.print(w500);
    }
  }
  buttonConfirm();
  buttonReturn();

  if(initializePhase == 4 && systemPhase == 0){
    targetFlag = true;
    lcd.clear();
    total = 10*w10 + 50*w50 + 100*w100 + 500*w500;
    display_setTarget();
  }
  else if(initializePhase == 4 && systemPhase == 2){
    lcd.clear();
    menuFlag = 0;
    initializePhase = 0;
  }
}

void display_setTarget(){
  lcd.setCursor(2,0);
  lcd.print("*Set Target*");
  lcd.setCursor(8,1);
  lcd.print("W");
  lcd.setCursor(10,1);
  lcd.print("000000");
}

void setTarget(){
  Serial.println("setTarget Started");
  
  if(initializePhase == 4) blinkCursor(10,1);
  else if(initializePhase == 5) blinkCursor(11,1);
  else if(initializePhase == 6) blinkCursor(12,1);
  else if(initializePhase == 7) blinkCursor(13,1);
  else if(initializePhase == 8) blinkCursor(14,1);
  else if(initializePhase == 9) blinkCursor(15,1);

  buttonUp();
  buttonDown();
  
  if(buttonPress){
    buttonPress = false;
    if(initializePhase == 4){ // 100,000
      lcd.setCursor(10,1);
      lcd.print(" ");
      lcd.setCursor(10,1);
      lcd.print(t1);
    }
    else if(initializePhase == 5){ // 10,000
      lcd.setCursor(11,1);
      lcd.print(" ");
      lcd.setCursor(11,1);
      lcd.print(t2);
    }
    else if(initializePhase == 6){ // 1,000
      lcd.setCursor(12,1);
      lcd.print(" ");
      lcd.setCursor(12,1);
      lcd.print(t3);
    }
    else if(initializePhase == 7){ // 100
      lcd.setCursor(13,1);
      lcd.print(" ");
      lcd.setCursor(13,1);
      lcd.print(t4);
    }
    else if(initializePhase == 8){ // 10
      lcd.setCursor(14,1);
      lcd.print(" ");
      lcd.setCursor(14,1);
      lcd.print(t5);
    }
    else if(initializePhase == 9){ // 1
      lcd.setCursor(15,1);
      lcd.print(" ");
      lcd.setCursor(15,1);
      lcd.print(t6);
    }
  }

  buttonConfirm();
  buttonReturn();

  if(initializePhase == 10 && systemPhase == 0){
    lcd.clear();
    lcd.noBlink();
    target = t1*100000 + t2*10000 + t3*1000 + t4*100 + t5*10 + t6*1;
    systemPhase = 1;
    initializePhase = 0;
  }
  else if(initializePhase == 10 && systemPhase == 2){
    lcd.clear();
    menuFlag = 0;
    initializePhase = 0;
  }
}

void buttonUp(){
  up_buttonState = digitalRead(right_Pin);

  if(up_buttonState != up_lastButtonState){
    if(up_buttonState == LOW){
      buttonPress = true;
      if(systemPhase == 0){
        if(initializePhase == 0) w10++;
        else if(initializePhase == 1) w50++;
        else if(initializePhase == 2) w100++;
        else if(initializePhase == 3) w500++;
        else if(initializePhase == 4){
          t1++;
          if(t1 > 9) t1 = 0;
        }
        else if(initializePhase == 5){
          t2++;
          if(t2 > 9) t2 = 0;
        }
        else if(initializePhase == 6){
          t3++;
          if(t3 > 9) t3 = 0;
        }
        else if(initializePhase == 7){
          t4++;
          if(t4 > 9) t4 = 0;
        }
        else if(initializePhase == 8){
          t5++;
          if(t5 > 9) t5 = 0;
        }
        else if(initializePhase == 9){
          t6++;
          if(t6 > 9) t6 = 0;
        }
      }
      else if(systemPhase == 1){
        mainPhase++;
        if(mainPhase == 3){
          lcd.clear();
          mainPhase = 0;
        }
      }
      else if(systemPhase == 2 && menuFlag == 0){
        menuPhase++;
        if(menuPhase == 4) menuPhase = 1;
      }
      else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 1){
        if(initializePhase == 0) w10++;
        else if(initializePhase == 1) w50++;
        else if(initializePhase == 2) w100++;
        else if(initializePhase == 3) w500++;
      }
      else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 2){
        if(initializePhase == 4){
          t1++;
          if(t1 > 9) t1 = 0;
        }
        else if(initializePhase == 5){
          t2++;
          if(t2 > 9) t2 = 0;
        }
        else if(initializePhase == 6){
          t3++;
          if(t3 > 9) t3 = 0;
        }
        else if(initializePhase == 7){
          t4++;
          if(t4 > 9) t4 = 0;
        }
        else if(initializePhase == 8){
          t5++;
          if(t5 > 9) t5 = 0;
        }
        else if(initializePhase == 9){
          t6++;
          if(t6 > 9) t6 = 0;
        }
      }
    }
    delay(50);
  }
  up_lastButtonState = up_buttonState;
}

void buttonDown(){
  down_buttonState = digitalRead(left_Pin);

  if(down_buttonState != down_lastButtonState){
    if(down_buttonState == LOW){
      buttonPress = true;
      if(systemPhase == 0 | systemPhase == 2){
        if(initializePhase == 0){
          w10--;
          if(w10 < 0) w10 = 0;
          else if(w10 > 999) w10 = 999;
        }
        else if(initializePhase == 1){
          w50--;
          if(w50 < 0) w50 = 0;
          else if(w50 > 999) w50 = 999;
        }
        else if(initializePhase == 2){
          w100--;
          if(w100 < 0) w100 = 0;
          else if(w100 > 999) w100 = 999;
        }
        else if(initializePhase == 3){
          w500--;
          if(w500 < 0) w500 = 0;
          else if(w500 > 999) w500 = 999;
        }
        else if(initializePhase == 4){
          t1--;
          if(t1 < 0) t1 = 9;
        }
        else if(initializePhase == 5){
          t2--;
          if(t2 < 0) t2 = 9;
        }
        else if(initializePhase == 6){
          t3--;
          if(t3 < 0) t3 = 9;
        }
        else if(initializePhase == 7){
          t4--;
          if(t4 < 0) t4 = 9;
        }
        else if(initializePhase == 8){
          t5--;
          if(t5 < 0) t5 = 9;
        }
        else if(initializePhase == 9){
          t6--;
          if(t6 < 0) t6 = 9;
        }
      }
    }
    delay(50);
  }
  down_lastButtonState = down_buttonState;
}

void buttonConfirm(){
  confirm_buttonState = digitalRead(confirm_Pin);

  if(confirm_buttonState != confirm_lastButtonState){
    if(confirm_buttonState == LOW){
      buttonPress = true;
      if(systemPhase == 0) initializePhase++;
      else if(systemPhase == 2 && menuFlag == 0) menuFlag = 1;
      else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 1) initializePhase++;
      else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 2) initializePhase++;
      else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 3){
        lcd.clear();
        menuSystemPhase++;
        if(menuSystemPhase == 2){
          menuSystemPhase = 0;
          menuFlag = 0;
          lcd.clear();
        }
      }
    }
    delay(50);
  }
  confirm_lastButtonState = confirm_buttonState;
}

void buttonReturn(){
  return_buttonState = digitalRead(return_Pin);

  if(return_buttonState != return_lastButtonState){
    if(return_buttonState == LOW){
      buttonPress = true;
      if(systemPhase == 0){
        initializePhase--;
        if(initializePhase < 0) initializePhase = 0;
        else if(initializePhase < 4 && targetFlag == true) initializePhase = 4;
      }
      else if(systemPhase == 1 && mainPhase == 0) mainPhase = 4; // to go menu
      else if(systemPhase == 2 && menuFlag == 0) menuFlag = -1;
      else if(systemPhase == 2 && menuFlag == 1 && menuPhase == 2){
        initializePhase--;
        if(initializePhase < 4) initializePhase = 4;
      }
    }
    delay(50);
  }
  return_lastButtonState = return_buttonState;
}

void main_CoinBank(){
  buttonUp(); // status
  buttonReturn(); // Menu

  if(mainPhase == 0){
    display_Total();
    main_Counting();
  }

  if(buttonPress){
    buttonPress = false;
    if(mainPhase == 1) main_coinStatus();
    else if(mainPhase == 2) main_target();
    else if(mainPhase == 4){
      lcd.clear();
      systemPhase = 2; // menu
    }
  }
}

void display_Total(){
  total = 10*w10 + 50*w50 + 100*w100 + 500*w500;
  lcd.setCursor(1,0);
  lcd.print("Total : ");
  lcd.print((int)total);
}

void main_Counting(){
  int val_1 = digitalRead(2);
  int val_2 = digitalRead(3);
  int val_3 = digitalRead(4);
  int val_4 = digitalRead(5);
  
  if(val_1 == 1){
    w10++;
    lcd.setCursor(4,1);
    lcd.print("*10 WON*");
    delay(500);
    lcd.setCursor(4,1);
    lcd.print("         ");
  }
  else if(val_2 == 1){
    w50++;
    lcd.setCursor(4,1);
    lcd.print("*50 WON*");
    delay(500);
    lcd.setCursor(4,1);
    lcd.print("         ");
  }
  else if(val_3 == 1){
    w100++;
    lcd.setCursor(3,1);
    lcd.print("*100 WON*");
    delay(500);
    lcd.setCursor(3,1);
    lcd.print("         ");
  }
  else if(val_4 == 1){
    w500++;
    lcd.setCursor(3,1);
    lcd.print("*500 WON*");
    delay(500);
    lcd.setCursor(3,1);
    lcd.print("         ");
  }
}

void main_coinStatus(){
  display_CoinStatus();
}

void main_target(){
  byte bar[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
  target = t1*100000 + t2*10000 + t3*1000 + t4*100 + t5*10 + t6*1;
  float percent = 100*(total / target);
  int a;
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print((int)total);
  lcd.print(" / ");
  lcd.print((int)target);

  lcd.createChar(0, bar);

  if(percent >= 10 && percent <= 100){
    for(a=1; a <= percent / 10; a++){
      Serial.print(a);
      lcd.setCursor(a,1);
      lcd.write(byte(0));
      delay(500);
    } 
  }

  lcd.setCursor(a+1,1);
  lcd.print((int)percent);
  lcd.print("%");
}

void callMenu(){
  lcd.noBlink();
  display_Menu(menuPhase);

  buttonUp();
  buttonConfirm();
  buttonReturn();

  if(buttonPress){
    buttonPress = false;
    if(menuFlag == 0){
      lcd.clear();
      callMenu();
    }
    else if(menuFlag == 1){
      if(menuPhase == 1){ // First Menu
        lcd.clear();
        display_CoinStatus();
      }
      else if(menuPhase == 2){ // Second Menu
        lcd.clear();
        display_setTarget();
        initializePhase = 4;
      }
      else if(menuPhase == 3){
        lcd.clear();
        menu_displaySystem();
      }
    }
    else if(menuFlag == -1){
      systemPhase = 1;
      menuFlag = 0;
      mainPhase = 0;
      lcd.clear();
    }
  }
}

void display_Menu(int i){
  lcd.setCursor(5,0);
  lcd.print("*MENU*");
  
  switch(i){
    case 1 : lcd.setCursor(0,1);
             lcd.print("1. Adjust Status");
             Serial.println("1. Adjust Status");
             break;

    case 2 : lcd.setCursor(0,1);
             lcd.print("2. Adjust Target");
             Serial.println("2. Adjust Target");
             break;

    case 3 : lcd.setCursor(0,1);
             lcd.print("3. System");
             Serial.println("3. System");
             break;
  }
}

void menu_AdjustStatus(){
  setCoin();
}

void menu_AdjustTarget(){
  setTarget();
}

void menu_displaySystem(){
      lcd.setCursor(4,0);
      lcd.print("Coin Box");
      lcd.setCursor(5,1);
      lcd.print("V.1.0");
}

void menu_System(){
  buttonConfirm();

  if(buttonPress){
    buttonPress = false;
    if(menuSystemPhase == 1){
    lcd.setCursor(4,0);
    lcd.print("Coin Box");
    lcd.setCursor(0,1);
    lcd.print("(C) 2019 GukJang");
    }
    else if(menuSystemPhase == 2) lcd.clear();
  }
}
