//プロジェクト名：Arduino超音波センサー搭載自動制御ロボットカー
// 開発者：大好洪史
// 開発日：08/02/2023~08/17/2023
// 使用言語：C++
// Status：Complete


#include <Servo.h>
#include <AFMotor.h>
#define echoPin 39 // Echo Pin
#define trigPin 38 // Trigger Pin
#define servoPin 10 //servo Pin
#define point 30 //距離の基準
 
double Duration = 0; //受信した間隔
double Distance[9]; //距離 50°から130までの距離を10°ずつに9個の値を格納

Servo myservo;

AF_DCMotor motorleft(1);
AF_DCMotor motorright(2);

int pos[9]={50,60,70,80,90,100,110,120,130};  //サーボの角度　50°から130°までの9個値を格納

int i=0;  //角度のカウント関数

int N=0;   //90°の時の値
int R=0;   //60°の時の値
int R1=0;  //80°の時の値
int L=0;   //120°の時の値
int L1=0;  //100°の時の値

int q=0;   //状態関数　q=0->静止　q=1->直進

void setup() {
  Serial.begin( 9600 );

  myservo.attach(servoPin);

  motorleft.setSpeed(250);
  motorleft.run(RELEASE);
  motorright.setSpeed(250);
  motorright.run(RELEASE);

  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
}

void loop() {
  
  
  for (i=0; i<=8; i++){  //カウンタ関数を進めて配列に格納
    myservo.write(pos[i]);     //サーボを回転
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite( trigPin, HIGH ); //超音波を出力
    delayMicroseconds( 10 ); //
    digitalWrite( trigPin, LOW );
    Duration = pulseIn( echoPin, HIGH ); //センサからの入力
    if (Duration > 0) {
      Duration = Duration/2; //往復距離を半分にする
      Distance[i] = Duration*340*100/1000000; // 音速を340m/sに設定

      //評価する3つの値をそれぞれ格納
      N=Distance[4];
      R=Distance[1];
      L=Distance[7];
      R1=Distance[3];
      L1=Distance[5];
    
      Serial.print("angle:");
      Serial.print(pos[i]);
      Serial.print("    ");
      Serial.print("Distance:");
      Serial.print(Distance[i]);
      Serial.println(" cm");

      if(i==0) delay(300);   //50°から130°までは距離があるため
      else delay(50);
    }
  }
  
  switch(q){
    case 0 : 
      if(point<N&&point<R1&&point<L1){

      motorleft.run(FORWARD);
      motorright.run(FORWARD);
      q=1;  //状態
      delay(500);
      }

      else if(N<point&&L>R){
        q=2;
      }

      else if(N<point&&R>=L){
        q=5;
      }

      else{
        motorleft.run(BACKWARD);
        motorright.run(BACKWARD);
        delay(500);
        motorleft.run(RELEASE);
        motorright.run(RELEASE);
      }
      break;

  case 1 : 
    if(point<N&&point<R1&&point<L1){
      motorleft.run(FORWARD);
      motorright.run(FORWARD);
      delay(200);
    }

    else if(point>=N||point>=R1||point>=L1){
      motorleft.run(RELEASE);
      motorright.run(RELEASE);
      q=0;
      delay(500);
    }
    break;

  case 2 :     //左回転
  
    motorright.run(FORWARD);
    motorleft.run(BACKWARD);
    delay(700);
    motorright.run(RELEASE);
    motorleft.run(RELEASE);
    q=0;
    break;

  case 5 :   //右回転
 
    motorleft.run(FORWARD);
    motorright.run(BACKWARD);
    delay(700);
    motorleft.run(RELEASE);
    motorright.run(RELEASE);
    q=0;
    break;

  default :
    motorleft.run(BACKWARD);
    motorright.run(BACKWARD);
    delay(1000);
    motorleft.run(RELEASE);
    motorright.run(RELEASE);
    q=0;
    break;
  }
  
}
