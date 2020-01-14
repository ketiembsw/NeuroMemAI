# 라즈베리파이와 과기대 보드 SPI 통신

## 환경 셋업

### 라즈베리파이 인스톨
1. 32GB 이상의 MicroSD 카드 준비  
2. SD카드 리더기를 이용하여 MicroSD카드를 PC에서 접근  
3. https://www.raspberrypi.org/downloads/raspbian/ 에서 이미지 다운로드
![image](https://user-images.githubusercontent.com/11454375/72321563-89796100-36e7-11ea-8cc3-c9ea8174ed92.png)
4. https://www.raspberrypi.org/documentation/installation/installing-images/README.md 를 참조하여 SD카드에 이미지 플래싱  
5. 라즈베리파이에 SD카드 꼽고 부팅  
6. 부팅 다 되면 패키지 업그레이드
```
$ sudo apt-get update && sudo apt-get upgrade
```

7. 라즈베리안과 gcc버전 확인
```
pi@raspberrypi:~ $ gcc --version
gcc (Raspbian 8.3.0-6+rpi1) 8.3.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

pi@raspberrypi:~ $ cat /etc/rpi-issue
Raspberry Pi reference 2019-09-26
Generated using pi-gen, https://github.com/RPi-Distro/pi-gen, 80d486687ea77d31fc3fc13cf3a2f8b464e129be, stage5
```

7. SPI 기능 Enable
```
$ sudo raspi-config
5. Interfacing Option 선택
P4 SPI 선택 하고 Enable 선택
```

8. 다음 핀맵 참고하여 과기대 보드와 연결
![image](https://user-images.githubusercontent.com/11454375/72321795-26d49500-36e8-11ea-82ee-7398960c7743.png)

9. 다음과 같이 본경로의 C 파일을 컴파일 하여 통신 확인
```
$ gcc spi_done_well_wSTU.c && ./a.out
```
통신이 잘 되는 것을 확인  

10. 새로운 프로토콜에 대한 C 파일 컴파일
```
$ gcc spi_new_protocol_wSTU.c && ./a.out
```
