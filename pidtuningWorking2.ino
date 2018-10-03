
/* 
  TMPIDLFR V1
  
  
           __
 _(\    |@@|
(__/\__ \--/ __
   \___|----|  |   __
       \ }{ /\ )_ / _\
       /\__/\ \__O (__
      (--/\--)    \__/
      _)(  )(_
     `---''---`


  LINE FOLLOWER ROBOT USING 6 IR SENSOR ARRAYS,ARDUINO UNO AND L298N MOTOR DRIVER
  
  THE CODE WRITTEN BELOW AND THE ALGORITHMS WAS SOLELY DEVELOPED BY TECHNOMANCERS ROBOTIC CLUB
  ALL RIGHTS RESERVED TO TECHNOMANCERS CLUB 2018
  YOU MAY NOT USE/EDIT/DISTRIBUTE CONTENTS OF THIS CODE WITHOUT PERMISSION FROM TECHNOMANCERS ROBOTIC CLUB !!
  
    TRUTH TABLE VALUES FOR THE ALGORITHMS
______________________________________		_ERROR___	
SENSOR PINS:	0	1	2	3	4	5           
______________________________________		_________
VALUES     :	0	0	0	0	0	1			5
           ____________________________		_________
				0	0	0	0	1	1			4
	       ____________________________		_________
				0	0	0	0	1	0			3
		   ____________________________		_________
				0	0	0	1	1	0			2
		   ____________________________     _________
				0	0	0	1	0	0			1
		   ____________________________     _________
				0	0	1	1	0	0			0
           ____________________________     _________
				0	0	1	0	0	0			-1
		   ____________________________     _________
				0	1	1	0	0	0			-2
		   ____________________________     _________
				0	1	0	0	0	0			-3
		   ____________________________     _________
				1	1	0	0	0	0			-4
		   ____________________________     _________
				1	0	0	1	0	0			-5
				
				
 */






//Motors and enb pins

const int leftMotor1=2;
const int leftMotor2=4;
const int enbA=3;                        // ith mattunnundenkil vere PWM pin il thanne connect cheyyanam. Otherwise DONT CHANGE!!
const int rightMotor1=7;
const int rightMotor2=8;
const int enbB=6;                        // ith mattunnundenkil vere PWM pin il thanne connect cheyyanam. Otherwise DONT CHANGE!!
const int baseSpeed=180;                 //EXPERIMENTAL: njan parayunnathu vare DONT CHANGE THIS!
const int maxSpeed=255;                  //DONT CHANGE THIS!!
//sensors only 6 used

const int sensorPins[6]={A0,A1,A2,A3,A4,A5};               //2nd pin from left = A0 AND 3rd pin is A1 ........etc...... 2nd pin from right=A5


float Kp=40.0;						  //EXPERIMENTAL: njan parayunnathu vare DONT CHANGE THIS!                						
float Ki=0.0;						  //EXPERIMENTAL: njan parayunnathu vare DONT CHANGE THIS!
float Kd=0.0;                       //EXPERIMENTAL: njan parayunnathu vare DONT CHANGE THIS!
float i=0.0;                        //DONT CHANGE THIS!
float error=0.0;                    //DONT CHANGE THIS!
float p=0.0;							 //DONT CHANGE THIS!
float d=0.0;								 //DONT CHANGE THIS!
float previousError=0.0;					 //DONT CHANGE THIS!
int pIDValue=0.0;						 //DONT CHANGE THIS!

int sensorValues[6]={0,0,0,0,0,0};			//CHANGE THIS WHEN CHANGING NUMBER OF SENSORS!!


int mode=0;    //0 => found line, 1=>no line found  2=>Finishpoint/all sensors detect black

//FUNCTION PROTOTYPES, ITH EDIT CHEYYANDA!!
void getError();
void getPIDValue();
void setMotorSpeed();

//INITIALIZATION
void setup()
{
  //motors
  pinMode(leftMotor1,OUTPUT);
  pinMode(leftMotor2,OUTPUT);
  pinMode(enbA,OUTPUT);
  pinMode(rightMotor1,OUTPUT);
  pinMode(rightMotor1,OUTPUT);
  pinMode(enbB,OUTPUT);
  digitalWrite(leftMotor1,HIGH);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,HIGH);
  digitalWrite(rightMotor2,LOW);
  Serial.begin(9600);
  
}

//MAINFUNCTION
void loop() {
	
	
  getError();
  while(mode!=0){
	  Serial.println("Line not found");
	  
	   digitalWrite(enbA,0); 
       digitalWrite(enbB,0);
	   getError();
	  }
  
  getError();
  Serial.println(sensorValues[0]);
  Serial.print(sensorValues[1]);
  Serial.print(sensorValues[2]);
  Serial.print(sensorValues[3]);
  Serial.print(sensorValues[4]);
  Serial.print(sensorValues[5]);
  Serial.println("\n Line Found at sensor");
  Serial.println(error);
  getPIDValue();
  setMotorSpeed();
}

//READ POSITION AND STORE THE OFFSET IN ERROR
void getError(){
	
	  for(int j=0;j<6;j++)
  {
    sensorValues[j]=digitalRead(sensorPins[j]);
  }
	
  
  if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 1)){
    error =5.0;
    mode=0;
  }

  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 1 )&&(sensorValues[5]== 1)){
    error =4.0; 
    mode=0;
  }

  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 1 )&&(sensorValues[5]== 0)) {
    error =3.0;
    mode=0;}
  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 1 )&&(sensorValues[4]== 1 )&&(sensorValues[5]== 0)){ 
  error =2.0;
  mode=0;
  }

  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 1 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) {
  error =1.0;
  mode=0;
  }

  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 1 )&&(sensorValues[3]== 1 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) {
    error =0.0;
	mode=0;
  }

  else if((sensorValues[0]== 0 )&&(sensorValues[1]==0 )&&(sensorValues[2]== 1 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)){ 
    error =-1.0;
	mode=0;
  }

  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 1 )&&(sensorValues[2]== 1 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) {
	  mode=0;
    error =-2.0;
  }
  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 1 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) {
	  mode=0;
    error =-3.0;
  }
  
  else if((sensorValues[0]== 1 )&&(sensorValues[1]== 1 )&&(sensorValues[2]==0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) {
	  mode=0;
    error =-4.0;
  }
  
  else if((sensorValues[0]== 1 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) {
	  mode=0;
    error =-5.0;
	
  }
  
  else if((sensorValues[0]== 1 )&&(sensorValues[1]== 1 )&&(sensorValues[2]== 1 )&&(sensorValues[3]== 1 )&&(sensorValues[4]== 1 )&&(sensorValues[5]== 1)) 
  {
   error = 0.0;
   mode =1;
  
  } 
  
  else if((sensorValues[0]== 0 )&&(sensorValues[1]== 0 )&&(sensorValues[2]== 0 )&&(sensorValues[3]== 0 )&&(sensorValues[4]== 0 )&&(sensorValues[5]== 0)) 
  {
   error = 10.0;
   mode =2;
  
  } 
} 
  
 void getPIDValue(){
	 p=error;
	 i=i+error;
	 d=error-previousError;
	 previousError = error;
	 pIDValue=(int)((Kp*p)+(Ki*i)+(Kd*d)); 
		 
 } 
  
 void setMotorSpeed(){
	 
	 int leftMotorSpeed = (baseSpeed + pIDValue);
	 int rightMotorSpeed = (baseSpeed- pIDValue);
	 
     if(leftMotorSpeed>255)
		 leftMotorSpeed =255;
	 if(rightMotorSpeed>255)
		 rightMotorSpeed=255;
	 if(leftMotorSpeed<0)
		 leftMotorSpeed=0;
	 if(rightMotorSpeed<0)
		 rightMotorSpeed=0;

	 analogWrite(enbA,leftMotorSpeed);   //Positive error decrease speed
     analogWrite(enbB,rightMotorSpeed); //Positive error increase speed
	   digitalWrite(leftMotor1,HIGH);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,HIGH);
  digitalWrite(rightMotor2,LOW);
	 
 }