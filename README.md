# NeuroMemAI

## Description
This is for intellino spi interface of arduino and linux

## Example code
https://github.com/ketiembsw/NeuroMemAI/blob/master/NeuroDriver.h

## Main API

 - init()
     - initialization
     - int init(int hw_platform, int max_if = 0x4000, int debug_print_val = 1, int sd_card_mode = 1)
     - int hw_platform : 하드웨어 플랫폼 아이디 (과기대 보드 혹은 다른 상용보드,)
     - int max_if : 최대 Influence Filed 
     - int debug_print_val : 디버그 프린트 여부
     - int sd_card_mode : SD 카드 사용 여부
  - convert_vector()
     - 컴패니언 모듈용 벡터로 변환
     - int convert_vector(float in_vector[], int out_vector[], int start_index, int in_length, int out_length, float max)
     - float in_vector[] : 인풋 벡터
     - int out_vector[] : 아웃푹 벡터
     - int start_index : 인풋 벡터의 시작 인덱스
     - int in_length : 인풋 벡터의 벡터 개수
     - int out_length : 아웃풋 벡터의 벡터 개수
     - float max : 각 원소의 최대값
   - learn()
     - 입력된 벡터를 인공지능 연산 모듈에 학습 시킴
     - int learn(int vector[], int length, int category)
     - int vector[] : 학습시킬 벡터
     - int length : 벡터의 길이
     - int category : 벡터의 카테고리
     
   - classify()
     - 입력된 벡터를 인공지능 모듈에서 기 학습된 벡터와 비교연산을 수행하여 가장 근접한 벡터의 카테고리값은 반환함
     - int vector[] : 분류하고자 하는 벡터
     - int length : 벡터의 길이
     - int *distance : 최근접 벡터의 거리
     - int *category : 최근접 벡터의 카테고리
     - int *nid : 최근접 벡터의 뉴론 아이디
