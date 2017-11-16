#define BLU 4
#define RED 3
#define GRN 2

#define MID_S A0
#define LEFT_S A1
#define RIGHT_S A2

#define M125 10
#define M120 11

int rbias, mbias, lbias;


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

int prev_error = 0;
int sum_error = 0;
int KP = 50;
int KI = 1;
int KD = 25;

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RED, LOW);
  digitalWrite(GRN, LOW);
  digitalWrite(BLU, LOW);
  
  int left = analogRead(LEFT_S)-lbias;
  int right = analogRead(RIGHT_S)-rbias;
  int mid = analogRead(MID_S)-mbias;
//  Serial.print(left);
//  Serial.print(" ");
//  Serial.print(mid);
//  Serial.print(" ");
//  Serial.println(right);
  
  int error = left*2-right;
  int P,I,D;
  P = KP*error;
  I = KI*sum_error;
  D = KD*(error-prev_error);
  int changespeed = (P+D)/50;
  Serial.println(changespeed);

  if(changespeed<-5){ // turn right
    digitalWrite(RED, HIGH);
    //changespeed = -15;
  }else if(changespeed>5){ // turn left
    digitalWrite(BLU, HIGH);
    //changespeed = 15; // either bump up change or make it variable somehow
  }else{ // go straight
    digitalWrite(GRN, HIGH);
    //changespeed = 0;
  }
  //changespeed = 0;
  analogWrite(M125,177+changespeed);
  analogWrite(M120,75+changespeed);

  prev_error = error;
  if(error<-3)
    error /= 2;
  if(error>2)
    error *= 3;
  
  sum_error += error;
  delay(10);
}

void startMotors(){
  analogWrite(M125,180);
  analogWrite(M120,90);
  delay(500);
}

