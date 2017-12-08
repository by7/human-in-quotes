#define BLU 4
#define RED 3
#define GRN 2

#define MID_S A0
#define LEFT_S A1
#define RIGHT_S A2

#define M125 10
#define M120 11

#define ISLEFT -1
#define ISCENTER 0
#define ISRIGHT 1

#define NOSTOP 10
#define LINEONE 11
#define EMPTYSPACE 12
#define STOP 13

int rbias, mbias, lbias;
int sleft, sright, scenter;
int stopstate = NOSTOP;
int count = 0;

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
  Serial.print(left);
  Serial.print(" ");
  Serial.print(right);
  Serial.print(" ");
  Serial.println(mid);

  sleft = (left<-8);
  sright = (right<-8);
  scenter = (mid<50);
  /*
  if(sleft && sright && scenter){ // sees horizontal line
    if(stopv == 1)
      state = STOP;
    else if(stopv == 0)
      stopv = 1;
  }
  else if(!scenter && state == ISCENTER){ // previously centered but got off line
    if(sleft)
      state = ISRIGHT;
    if(sright)
      state = ISLEFT;
    if(sleft && sright)
      state = ISCENTER;
  }
  else if(state != ISCENTER && sleft){ // turning and sees the line with left
    state = ISRIGHT;
  }
  else if(state != ISCENTER && sright){ // turning and sees the line with right
    state = ISLEFT;
  }
  else if(state != ISCENTER && scenter){ // sees the line again after being off
    state = ISCENTER;
  }

  if(state == ISRIGHT){
    digitalWrite(BLU, HIGH);
    analogWrite(M125,177+20);
    analogWrite(M120,75+20);
  }
  else if(state == ISLEFT){
    digitalWrite(RED, HIGH); 
    analogWrite(M125,177-20);
    analogWrite(M120,75-20);
  }
  else if(state == ISCENTER){
    digitalWrite(GRN, HIGH);
    analogWrite(M125,177);
    analogWrite(M120,75);
  }
  else if(state == STOP){
    digitalWrite(RED, HIGH);
    digitalWrite(BLU, HIGH);
    analogWrite(M125,255);
    analogWrite(M120,0);
  }
  count++;
  if(count%15)
    stopv = 0;
    */
  if(stopstate == NOSTOP){
    if(sleft && scenter && sright){
      stopstate = LINEONE;
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
    analogWrite(M125,177+20);
    analogWrite(M120,75+20);
  }
  if(!sleft && sright){
    //go right
    digitalWrite(RED, HIGH); 
    digitalWrite(GRN, LOW);
    digitalWrite(BLU, LOW);
    analogWrite(M125,177-20);
    analogWrite(M120,75-20);
  }
  if(!sleft && scenter && !sright){
    //go straight
    digitalWrite(GRN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(BLU, LOW);
    analogWrite(M125,177);
    analogWrite(M120,75);
  }
  //delay(10);
}

void startMotors(){
  analogWrite(M125,180);
  analogWrite(M120,90);
  delay(500);
}

