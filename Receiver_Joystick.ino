/*--------------------------------------
 * 신호 수신 --> 모터 제어
 --------------------------------------*/
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

int value_x = 0;
int value_y = 0;
unsigned int val_x = 0;
unsigned int val_y = 0;

void setup()
{
    Serial.begin(9600);

    Mirf.cePin = 9;   //CE핀
    Mirf.csnPin = 10;   //CS핀
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();    // nRF24L01 초기화
    Mirf.setRADDR((byte *)"TX_01");   //수신측 어드레스 설정
    Mirf.payload = 4;     //송수신에 사용할 바이트 수 지정 sizeof(unsigned int) 는 2바이트
    Mirf.channel = 3;     //송수신 채널 지정(0~128), 송수신 모듈이 동일한 채널을 사용해야함
    Mirf.config();
}

void ReceiveData(void)
{    
    byte data[Mirf.payload];    //수신할 데이터 버퍼 정의

    if(Mirf.dataReady())    //데이터수신 대기
    {
        Mirf.getData(data);    //수신 데이터를 data에 저장
        //데이터 버퍼를 통합
        val_x = (unsigned int)((data[1] << 8) | data[0]);
        val_y = (unsigned int)((data[3] << 8) | data[2]);
    }
}

void loop()
{
    ReceiveData();
    value_x = val_x - 502;
    value_y = val_y - 505;
    
    Serial.print("JS X = "); Serial.print(value_x);
    Serial.print("     JS Y = "); Serial.println(value_y);
    delay(500);
}
