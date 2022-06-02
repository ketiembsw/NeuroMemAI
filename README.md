# AI-Neurocell-Module

### int BriskFeatureExtraction(string filepath) 
- BRISK 알고리즘으로 특징 벡터 추출  
```string filepath``` : 특징 추출할 이미지가 저장된 경로  



### int LearnVectorToNeuron(string filepath, int smtError, int neurons, int vector_length)  
- 부품 이미지에서 추출된 특징 벡터를 인공지능 연산 모듈에 학습시킴  
```string filepath``` : 특징 벡터가 저장된 csv 파일 경로  
```int smtError``` : 검사 종류 (오삽, 미삽 등)  
```int neurons``` : 특징 정보를 저장할 뉴런 개수  
```int vector_length``` : 학습할 벡터 길이  



### int InspectionParts(string filepath, int smtError, int vector_length)
- 검사를 위한 특징 벡터를 인공지능 모듈에서 학습된 특징 벡터와 비교연산을 수행하여 유사도를 정확도로 환산하여 반환함  
```string filepath``` : 검사할 이미지 파일이나 특징 벡터 csv 파일 경로  
```int smtError``` : 검사 종류 (오삽, 미삽 등)  
```int vector_length``` : 검사할 벡터 길이 설정  



### int printResult(string filepath, int smt_error, int neurons, int vector_length)
- 검사 결과를 출력  
```string filepath``` : 검사 결과 파일 경로  
```int smt_error``` : 검사 종류 (오삽, 미삽 등)  
```int neurons``` : 검사에 사용된 뉴런 개수  
```int vector_length``` : 검사 결과와 함께 출력할 벡터 길이  


### int CameraCapture(string serial, int transferRate, int width, int height)
- 얼굴 이미지 실시간 카메라 캡처  
```string serial``` : serial 전송 연결할 포트 정보  
```int transferRate``` : serial 전송 속도  
```int width``` : 카메라 이미지 가로 길이 설정  
```int height``` : 카메라 이미지 세로 길이 설정  


### int HaarFeatureExtraction(int frame, string classifier, int neurons, int vector_length)
- Haar 알고리즘을 이용하여 특징 벡터 추출  
```int frame``` : 특징 추출할 카메라 입력 이미지  
```string classifier``` : Haar 특징 추출 정보 xml 파일  
```int neurons``` : 뉴런 개수 설정(학습할 특징 데이터 개수 설정)  
```int vector_length``` : 특징 벡터 길이 설정  



### int LearnHaarFeatures(string imgfile, int neurons, int vector_length, int category)
- Haar 알고리즘으로 추출된 특징 벡터를 뉴런에 학습  
```string imgfile``` : 카메라에 캡처된 이미지 정보  
```int neurons``` : 뉴런 개수 설정(학습할 특징 데이터 개수 설정)  
```int vector_length``` : 특징 벡터 길이 설정  
```int category``` : 특징 벡터 카테고리 정보  



### int FeaturesTransferSPI(int data, uint8_t len) 
- 추출된 특징 벡터를 SPI 통신을 통하여 뉴로셀 칩으로 전송  
```int data``` : 추출된 특징 데이터  
```uint8_t len``` : 한 번에 전송할 데이터 길이  



### int HaarFaceClassifier(int featurVectors, int neurons, int vectorLength)
- Haar 특징 정보로 얼굴 인식 검사 후 결과 반환  
```int featurVectors``` : 검사할 특징 벡터  
```int neurons``` : 검사할 뉴런 개수  
```int vectorLength``` : 검사할 벡터 길이 설정  


### int printResultFaces(int delayTime, string faces)
- 검사를 위한 특징 벡터를 인공지능 모듈에서 학습된 특징 벡터와 비교연산을 수행하여 유사도를 정확도로 환산하여 반환함  
```int delayTime``` : 검사 소요 시간  
```string faces``` : 얼굴 인식 결과 검출 여부  
