 
/* 신호 수신 하기 >> 모터 제어 하기 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h> 
 

int IN1 = 2;
int IN2 = 4;
int IN3 = 5; // 왼쪽 바퀴 A 오른쪽 바퀴 B
int IN4 = 7;
int EN_A = 3;
int EN_B = 6;


unsigned int valueX = 0; 
unsigned int valueY = 0;

void setup()

{
  Serial.begin(9600);
 
  pinMode(IN1 , OUTPUT);
  pinMode(IN2 , OUTPUT);
  pinMode(IN3 , OUTPUT);
  pinMode(IN4 , OUTPUT);
  pinMode(EN_A , OUTPUT);
  pinMode(EN_B , OUTPUT);
  
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
         valueX =  (unsigned int)((data[1] << 8) | data[0]);
       valueY = (unsigned int)((data[3] << 8) | data[2]);
    }
}

  void NewControl(void)
  {
    if (valueY > 140 and valueX  > 110 and valueX < 140)
  { 
   

   digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, HIGH);
   analogWrite(EN_A, 255);
   analogWrite(EN_B, 255);


    if(valueX > 140)  //좌
      {
         digitalWrite(IN1, LOW); 
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          analogWrite(EN_A, 30);
          analogWrite(EN_B, 255);
      }
   //우
        else if(valueX < 110)
      {
        digitalWrite(IN1, LOW);     
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(EN_A, 255);
          analogWrite(EN_B, 30);
      }
      else (valueY >140 and valueX  > 110 and valueX < 140); 
      { digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(EN_A, 255);
        analogWrite(EN_B, 255);
      }
   
  }
  
// 후진
    else if (valueY < 110 and valueX > 110 and valueX < 140)
    {
    

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN_A, 255);
    analogWrite(EN_B, 255 );

    
    if(valueY <110 and valueX > 140)
      {
         digitalWrite(IN1, HIGH); 
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          analogWrite(EN_A, 30);
          analogWrite(EN_B, 255);
      }

        else if(valueY < 110 and valueX < 110)
      {
        digitalWrite(IN1, HIGH); 
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          analogWrite(EN_A, 255);
          analogWrite(EN_B, 30);
         
      }else (valueY < 110 and valueX > 110 and valueX < 140);
      {
        digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN_A, 255);
      analogWrite(EN_B, 255 );
      }
    }
// 정지
     else if ( valueY > 110 and valueY < 140)
     {
       digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        analogWrite(EN_A, 0);
        analogWrite(EN_B, 0);
       
       if(valueX > 140)
        {
         digitalWrite(IN1, LOW); 
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          analogWrite(EN_A, 255);
          analogWrite(EN_B, 0);
        }

        else if(valueX < 110)
        {
        digitalWrite(IN1, LOW); 
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          analogWrite(EN_A, 0);
          analogWrite(EN_B, 255);
         
       }
   
     }
  }  

void loop()

{
   ReceiveData();


  NewControl();  
  Serial.print("JS_X : ");
  Serial.print(valueX);
  Serial.print("   ");
  Serial.print("JS_Y : ");
  Serial.print(valueY);
  Serial.print("   ");
  Serial.println("   ");

    
}
