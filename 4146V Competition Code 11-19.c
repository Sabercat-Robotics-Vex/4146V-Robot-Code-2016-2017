#pragma config(Sensor, in1,    liftAngle,      sensorPotentiometer)
#pragma config(Sensor, dgtl2,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl4,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port1,           backRightMotor, tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           backLeftMotor, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           frontRightMotor, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           frontLeftMotor, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           rightArmY,     tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           rightArmB,     tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           leftArmY,      tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           leftArmB,      tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           sideDrive,     tmotorVex393TurboSpeed_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/* Arm at floor level = 55
   Maximum height 2416
   Soft stop at 2300
   Carry = 400
   tall fence push = 1500
   low fence push = 1300
*/
#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//Global Variables
float pidArmRequest;


task pidLiftControl(){

	float pidSensorCurrentValue;
 	float pidError;
 	float pidDrive;
	float pid_Kp = 0.5;

 while(true)
 	{
   //get the current sensor value
 		pidSensorCurrentValue = SensorValue[liftAngle];

 		//calculate error
 		pidError = pidArmRequest - pidSensorCurrentValue;

 		//calculate drive
 		pidDrive = (pid_Kp * pidError);

 		//limit drive
 		if(pidDrive > 127 )
 			pidDrive = 127;
 		if(pidDrive < (-127) )
 			pidDrive = (-127);

 		//send to motor
 		motor[rightArmY] = pidDrive;
 		motor[rightArmB] = pidDrive;
 		motor[leftArmY] = pidDrive;
 		motor[leftArmB] = pidDrive;

 		wait1Msec(25);

	}

}



void Arm(int rArmY, int rArmB, int lArmY, int lArmB)
{
	motor[rightArmY] = rArmY;
	motor[rightArmB] = rArmB;
	motor[leftArmY] = lArmY;
	motor[leftArmB] = lArmB;
	wait1Msec(25);
}
void ArmAll( int s ) {
	Arm( s, s, s, s );
}
void Drive(int fLeft, int bLeft, int fRight, int bRight)
{
	motor[frontLeftMotor] = fLeft;
	motor[backLeftMotor] = bLeft;
	motor[frontRightMotor] = fRight;
	motor[backRightMotor] = bRight;
	wait1Msec(25);
}
void DriveLeftSide( int s ) {
	motor[frontLeftMotor] = s;
	motor[backLeftMotor] = s;
}
void DriveRightSide( int s ) {
	motor[frontRightMotor] = s;
	motor[backRightMotor] = s;
}
void DriveAll(int s){
	Drive(s, s, s, s);
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
/*task autoLeftEncoderGo () {
	DriveLeftSide(100);
 	waitUntil( SensorValue[leftEncoder] <= -900 );
  DriveLeftSide(0);
}
task autoRightEncoderGo () {
	DriveRightSide(100);
  waitUntil( SensorValue[rightEncoder] <= -900 );
  DriveRightSide(0);
}*/
task autonomous()
{
  //flip down the scoop
	DriveAll(100);
  wait1Msec(500);
  DriveAll(-100);
  wait1Msec(300);

  //raise the arm to the level of the stars
  ArmAll(50);
  waitUntil( SensorValue[liftAngle] >= 1325 );
  ArmAll(10);

  //move forward to knock the stars off
  DriveAll(120);
  wait1Msec(2300);
  DriveAll(0);

  //move back to make sure the robot isn't touching a star
  DriveAll(-100);
  wait1Msec(1000);
  DriveAll(0);

  //lower the arm to get ready for driver control
  ArmAll(-50);
  wait1Msec(500);
  ArmAll(0);

  //waitUntil( SensorValue[rightEncoder] <= -900 );
  //waitUntil( SensorValue[leftEncoder] <= -900 );
  //startTask( autoLeftEncoderGo );
  //startTask( autoRightEncoderGo );

  /*
  ArmAll(10);
  DriveAll(50);
 //resetMotorEncoder(rightEncoder);
  waitUntil( SensorValue[rightEncoder] <= -700 );
  DriveAll(0);
  //wait1Msec(1000);
  */
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

	// User control code here, inside the loop
task driveTrain ()
{
	while(true) {
		//drive train
		motor[frontLeftMotor] = vexRT[Ch3];
		motor[backLeftMotor] = vexRT[Ch3];
		motor[frontRightMotor] = vexRT[Ch2];
		motor[backRightMotor] = vexRT[Ch2];
		motor[sideDrive] = vexRT[Ch1];
		wait1Msec(25);
	}
}

task usercontrol ()
{
	startTask (driveTrain);

	enum States {
		Launching,
		Lowering,
		Storing,
		Idle
	};

	enum States RobotState = Idle;
	while( true ) {
		if( vexRT[Btn8D] == 1 ) {
			RobotState = Lowering;
		}
		if( vexRT[Btn8L] == 1 ) { //Store!
			RobotState = Storing;
		}
		if( vexRT[Btn8U] == 1 ) {
			RobotState = Launching;
		}
		if( vexRT[Btn8R] == 1 ) {
			RobotState = Idle;
		}

		switch (RobotState) {
			case Lowering:
				if ( SensorValue[liftAngle] > 80 ) {
					ArmAll( -100 );
				} else {
					ArmAll( 0 );
					RobotState = Idle;
				}
				break;
			case Storing:
				if ( SensorValue[liftAngle] > 500 ) {
					ArmAll( -50 );
				} else if ( SensorValue[liftAngle] < 400 ) {
					ArmAll( 50 );
				} else {
					ArmAll( 0 );
					RobotState = Idle;
				}
				break;
			case Launching:
				if ( SensorValue[liftAngle] < 2000 ) {
					ArmAll(127);
				} else {
					ArmAll(-100);
					wait1Msec(700);
					ArmAll( 0 );
					RobotState = Idle;
				}
				break;
			case Idle:
				if( vexRT[Btn6U] == 1 && SensorValue[liftAngle] < 2300 ) {
					ArmAll( 127 );
				} else if( vexRT[Btn6D] == 1 && SensorValue[liftAngle] > 80 ) {
					ArmAll( -50 );
				} else {
					ArmAll( 13 );
				}
				break;
			default:
				break;
		}

  	wait1Msec( 25 );
  }
}
