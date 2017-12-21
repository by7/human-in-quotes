#define BLU 4
#define RED 3
#define GRN 2

#define MID_S A0
#define LEFT_S A1
#define RIGHT_S A2

#define M125 10
#define M120 11

#define NOSTOP 0
#define LINEONE 1
#define EMPTYSPACE 2
#define STOP 3

int rbias, mbias, lbias;
int sleft, sright, scenter;
int stopstate = NOSTOP;

void setup() {
  // put your setup code here, to run once:
  pinMode(RED,OUTPUT);
  pinMode(GRN,OUTPUT);
  pinMode(BLU,OUTPUT);
  pinMode(LEFT_S,INPUT);
  pinMode(RIGHT_S,INPUT);
  pinMode(MID_S,INPUT);
  pinMode(M125,OUTPUT);
  pinMode(M120,OUTPUT);
  
  Serial.begin(9600);

  analogWrite(M125, 255); // init left motor to stop
  
  rbias = mbias = lbias = 0;
  int i=0;
  for(;i<100;i++){
    lbias += analogRead(LEFT_S);
    rbias += analogRead(RIGHT_S);
    mbias += analogRead(MID_S);
  }
  rbias /= i;
  lbias /= i;
  mbias /= i;

  //startMotors();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int left = analogRead(LEFT_S)-lbias;
  int right = analogRead(RIGHT_S)-rbias;
  int mid = analogRead(MID_S)-mbias;
//  Serial.print(left);
//  Serial.print(" ");
//  Serial.print(right);
//  Serial.print(" ");
//  Serial.println(mid);

  sleft = (left<-8);
  sright = (right<-8);
  scenter = (mid<50);
  
  if(stopstate == NOSTOP){
    if(sleft && scenter && sright){
      stopstate = LINEONE;
      stopstate = STOP;
      digitalWrite(RED, HIGH);
      digitalWrite(BLU, HIGH);
      analogWrite(M125,255);
      analogWrite(M120,0);
      delay(99999);
    }
  }else if(stopstate == LINEONE){
    if(!sleft || !scenter || !sright){
      stopstate = EMPTYSPACE;
    }
  }else if(stopstate == EMPTYSPACE){
    if(sleft && scenter && sright){
      stopstate = STOP;
      digitalWrite(RED, HIGH);
      digitalWrite(BLU, HIGH);
      analogWrite(M125,255);
      analogWrite(M120,0);
      delay(99999);
    }
  }
  
  if(sleft && !sright){
    //go left
    digitalWrite(BLU, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GRN, LOW);
    analogWrite(M125,177+30);
    analogWrite(M120,75+25);
  }
  if(!sleft && sright){
    //go right
    digitalWrite(RED, HIGH); 
    digitalWrite(GRN, LOW);
    digitalWrite(BLU, LOW);
    analogWrite(M125,177-25);
    analogWrite(M120,75-30);
  }
  if(!sleft && scenter && !sright){
    //go straight
    digitalWrite(GRN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(BLU, LOW);
    analogWrite(M125,177);
    analogWrite(M120,75);
  }
}


