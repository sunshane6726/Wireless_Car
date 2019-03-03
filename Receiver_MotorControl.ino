/*--------------------------------------
 * 신호 수신 --> 모터 제어
 --------------------------------------*/
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
int IN1 = 2;
int IN2 = 4;
int IN3 = 5;
int IN4 = 7;
int EN_A = 3;
int EN_B = 6;
int value_x = 0;
int value_y = 0;
int SpeedValue = 0;
int TurnValue = 0;
int Speed_A = 0;
int Speed_B = 0;
unsigned int val_x = 0;
unsigned int val_y = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(EN_A, OUTPUT);
    pinMode(EN_B, OUTPUT);

    Mirf.cePin = 9;   //CE핀
    Mirf.csnPin = 10; //CS핀
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();  // nRF24L01 초기화
    Mirf.setRADDR((byte *)"TX_01");   //수신측 어드레스 설정
    Mirf.payload = 4;   //송수신에 사용할 바이트 수 지정 sizeof(unsigned int) 는 2바이트
    Mirf.channel = 3;   //송수신 채널 지정(0~128), 송수신 모듈이 동일한 채널을 사용해야함
    Mirf.config();
}

void ReceiveData(void)
{
    //수신할 데이터 버퍼 정의
    byte data[Mirf.payload];

    if(Mirf.dataReady())    //데이터수신 대기
    {
        Mirf.getData(data);    //수신 데이터를 data에 저장
        //데이터 버퍼를 통합
        val_x = (unsigned int)((data[1] << 8) | data[0]);
        val_y = (unsigned int)((data[3] << 8) | data[2]);
    }
}

void ControlMotor(void)
{
  //=============== 전진 =================
    if(value_y < -50){
      SpeedValue = (-(value_y)*55/502)+200;
      if(SpeedValue > 255) SpeedValue = 255;
      
    //=============== 전진-우회전 (6) =================
      if(value_x < -50){
        TurnValue = (-(value_y)*55/505)+200;
        if(TurnValue > 255) TurnValue = 255;
        
        if(SpeedValue > TurnValue){
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          Speed_A = (SpeedValue + TurnValue) / 2;
          Speed_B = (255 - TurnValue) + SpeedValue;
        }
        else if(SpeedValue < TurnValue){
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          Speed_A = TurnValue;
          Speed_B = SpeedValue;
        }
        else
        {
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          Speed_A = 0;
          Speed_B = SpeedValue;
        }
      }
  
      //=============== 전진-좌회전 (7) =================
      else if(value_x > 50){
        TurnValue = (value_x*55/518)+200;
        if(TurnValue > 255) TurnValue = 255;
        
        if(SpeedValue > TurnValue){
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          Speed_A = (255 - TurnValue) + SpeedValue;
          Speed_B = (SpeedValue + TurnValue) / 2;
        }
        else if(SpeedValue < TurnValue){
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          Speed_A = SpeedValue;
          Speed_B = TurnValue;
        }
        else
        {
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          Speed_A = SpeedValue;
          Speed_B = 0;
        }
        
      }
  
      //=============== 전진 (2) =================
      else{
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        Speed_A = SpeedValue;
        Speed_B = SpeedValue;
      }
    }
  
  //===============후진=================
    else if(value_y > 50){
      SpeedValue = (value_y*55/521)+200;
      if(SpeedValue > 255) SpeedValue = 255;
      
    //=============== 후진-우회전 (8) =================
      if(value_x < -50){
        TurnValue = (-(value_y)*55/505)+200;
        if(TurnValue > 255) TurnValue = 255;
        
        if(SpeedValue > TurnValue){
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          Speed_A = (SpeedValue + TurnValue) / 2;
          Speed_B = (255 - TurnValue) + SpeedValue;
        }
        else if(SpeedValue < TurnValue){
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          Speed_A = TurnValue;
          Speed_B = SpeedValue;
        }
        else
        {
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          Speed_A = 0;
          Speed_B = SpeedValue;
        }
      }
    //=============== 후진-좌회전 (9) =================
      else if(value_x > 50){
        TurnValue = (value_x*55/518)+200;
        if(TurnValue > 255) TurnValue = 255;
        
        if(SpeedValue > TurnValue){
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          Speed_A = (255 - TurnValue) + SpeedValue;
          Speed_B = (SpeedValue + TurnValue) / 2;
        }
        else if(SpeedValue < TurnValue){
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          Speed_A = SpeedValue;
          Speed_B = TurnValue;
        }
        else
        {
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          Speed_A = SpeedValue;
          Speed_B = 0;
        }
      }
    //=============== 후진 (3) =================
      else{
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        Speed_A = SpeedValue;
        Speed_B = SpeedValue;
      }
    }
    
  //===============정지=================
    else{
      SpeedValue = 0;  
  
    //=============== 정지-우회전 (4) =================
      if(value_x < -50){
        TurnValue = (-(value_x)*55/505)+200;
        if(TurnValue > 255) TurnValue = 255;
      
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        Speed_A=SpeedValue;
        Speed_B=TurnValue;
      }
      
    //=============== 정지-좌회전 (5) =================
      else if(value_x > 50){
        TurnValue = (value_x*55/518)+200;
        if(TurnValue > 255) TurnValue = 255;
      
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        Speed_A=TurnValue;
        Speed_B=SpeedValue;
      }
  
    //=============== 정지 (1) =================
      else{
        Speed_A=SpeedValue;
        Speed_B=SpeedValue;
      }
    }
    if(Speed_A > 255) Speed_A = 255;
    if(Speed_B > 255) Speed_B = 255;
    analogWrite(EN_A, Speed_A);
    analogWrite(EN_B, Speed_B);
}

void loop()
{
    ReceiveData();
    value_x = val_x - 502;
    value_y = val_y - 505;
    ControlMotor();
    
    Serial.print("JS X = ");
    Serial.print(value_x);
    Serial.print("     JS Y = ");
    Serial.print(value_y);
    Serial.print("     Speed A = ");
    Serial.print(Speed_A);
    Serial.print("     Speed B = ");
    Serial.println(Speed_B);
}
