#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "driver/i2s.h"
#define SDA 13
#define SCL 14
#define DELETE " "
#define BUTTON 2
#define JUMP 15
LiquidCrystal_I2C lcd(0x27,16,2);

boolean blank = false;
TaskHandle_t playerObject = NULL;
TaskHandle_t startUpObject = NULL;
TaskHandle_t obstacleObject = NULL;
TaskHandle_t obstacle2Object = NULL;

SemaphoreHandle_t lcdMutex;

void player(void * parameters);
void obstacle(void * parameters);
void gameOver(void* parameters);
void obstacle2(void* parameters);

void startup(void * parameters){
    boolean check = false;
    lcd.setCursor(0,0);
    lcd.print("CLICK THE BUTTON");
    lcd.setCursor(0,1);
    while(true){
      if(digitalRead(BUTTON) == LOW){
        break;
      }
      vTaskDelay(200/portTICK_PERIOD_MS);
    }
    
    xTaskCreate(
        player,
        "PLAYER",
        4096,
        NULL,
        1,
        &playerObject
    );
    
    xTaskCreate(
        obstacle,
        "Obstacle Generation",
        4096,
        NULL,
        1,
        &obstacleObject
    );
    xTaskCreate(
        obstacle2,
        "Obstacle Generation",
        4096,
        NULL,
        1,
        &obstacle2Object
    );

    
    vTaskSuspend(NULL);
 
  }

boolean checkCollision(boolean* location){
  if(*location == true){
    return false;
  }
  else{
    return true;
  }
  
}


void player(void* parameters){
    if(xSemaphoreTake(lcdMutex,portMAX_DELAY)){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("o");
    xSemaphoreGive(lcdMutex);
    }
    
    for(;;){
      if(digitalRead(JUMP) == LOW){
        //Check the delays
        lcd.setCursor(0,1);
        lcd.print(" ");
        blank = true;
        delay(100);
        lcd.setCursor(0,0);
        lcd.print("o");
        delay(225);
        lcd.setCursor(0,0);
        lcd.print(" ");
        delay(225);
        lcd.setCursor(0,1);
        lcd.print("o");
        blank = false; 
      }
      vTaskDelay(15/portTICK_PERIOD_MS);
    }
}


void obstacle(void* parameters){
  boolean val = false;
  unsigned int SPEED = 320;
  delay(2000);
  if(xSemaphoreTake(lcdMutex,portMAX_DELAY)){
  lcd.setCursor(15,1);
  lcd.print("i");
  xSemaphoreGive(lcdMutex);
  }
  int count = 50;
  while(count!=0){
    for(int i = 15; i>=0; i--){
    if(i == 0){
      val = checkCollision(&blank);
      if(val){
        xTaskCreate(
          gameOver,
          "Close Game",
          2048,
          NULL,
          1,
          NULL
        );
        vTaskSuspend(playerObject);
        vTaskSuspend(obstacle2Object);
        vTaskSuspend(NULL);
      }
    }
    if(xSemaphoreTake(lcdMutex,portMAX_DELAY)){
    delay(50);
    lcd.setCursor(i+1,1);
    lcd.print(" ");
    lcd.setCursor(i,1);    
    lcd.print("i");
    }
    xSemaphoreGive(lcdMutex);
    vTaskDelay(SPEED/portTICK_PERIOD_MS); //This controls speed
  }
  count--;
  }
  for(;;){
    vTaskDelay(20/portTICK_PERIOD_MS);
  }
}

void obstacle2(void* parameters){
  boolean val = false;
  unsigned int SPEED = 320;
  srand(time(NULL)); 
  int lower = 4000;
  int upper = 5500;
  int random = (rand() % (upper - lower + 1)) + lower;
  delay(random);
  if(xSemaphoreTake(lcdMutex,portMAX_DELAY)){
  lcd.setCursor(15,1);
  lcd.print("i");
  xSemaphoreGive(lcdMutex);
  }
  int count = 50;
  while(count!=0){
    for(int i = 15; i>=0; i--){
    if(i == 0){
      val = checkCollision(&blank);
      if(val){
        xTaskCreate(
          gameOver,
          "Close Game",
          2048,
          NULL,
          1,
          NULL
        );
        vTaskSuspend(playerObject);
        vTaskSuspend(obstacleObject);
        vTaskSuspend(NULL);
      }
    }
    if(xSemaphoreTake(lcdMutex,portMAX_DELAY)){
    delay(50);
    lcd.setCursor(i+1,1);
    lcd.print(" ");
    lcd.setCursor(i,1);    
    lcd.print("i");
    }
    xSemaphoreGive(lcdMutex);
    vTaskDelay(SPEED/portTICK_PERIOD_MS); //This controls speed
  }
  count--;
  }
  for(;;){
    vTaskDelay(20/portTICK_PERIOD_MS);
  }
}
void gameOver(void* parameters){
  if (xSemaphoreTake(lcdMutex, portMAX_DELAY)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GAME OVER");
    xSemaphoreGive(lcdMutex);
  }
  for(;;){
  
  if(digitalRead(BUTTON) == LOW){
    vTaskResume(startUpObject);
    vTaskSuspend(NULL);
  }
  vTaskDelay(20/portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SDA,SCL);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();  
  pinMode(BUTTON,INPUT_PULLUP);
  lcdMutex = xSemaphoreCreateMutex();
  pinMode(JUMP,INPUT_PULLUP);

  xTaskCreate(
    startup,
    "START SCREEN",
    4096,
    NULL,
    1,
    &startUpObject
  );

}

void loop() {

}




