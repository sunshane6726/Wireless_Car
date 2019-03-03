/*--------------------------------------
 * 조이스틱 ADC --> 신호 송신
 --------------------------------------*/

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

unsigned int val_x = 0;
unsigned int val_y = 0;

void setup()
{
    Serial.begin(9600);

    Mirf.cePin = 9;   //CE핀
    Mirf.csnPin = 10;   //CS핀
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();  // nRF24L01 초기화
    Mirf.payload = 4;   //송수신에 사용할 바이트 수 지정 sizeof(unsigned int) 는 2바이트
    Mirf.channel = 3;   //송수신 채널 지정(0~128), 송수신 모듈이 동일한 채널을 사용해야함
    Mirf.config();
}

void loop()
{
    //A0, A1의 아날로그 값을 읽어옴
    val_x = analogRead(A0);
    val_y = analogRead(A1);

    //송신할 데이터 버퍼 정의
    byte data[Mirf.payload];

    //아날로그 데이터를 송신할 바이트 어레이에 저장
    data[0] = val_x & 0xFF;   //하위바이트
    data[1] = val_x >> 8;     //상위바이트
    data[2] = val_y & 0xFF;   //하위바이트
    data[3] = val_y >> 8;     //상위바이트

    //수신측 어드레스 지정 및 데이터 송신
    Mirf.setTADDR((byte *)"TX_01");
    Mirf.send(data);
    //데이터가 송신이 완료될 때가지 대기
    while(Mirf.isSending()) {}
    delay(20);
}
