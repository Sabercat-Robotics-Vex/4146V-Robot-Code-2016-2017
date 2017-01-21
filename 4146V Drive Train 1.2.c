#pragma config(Sensor, in1,    liftAngle,      sensorPotentiometer)
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

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


/* Arm at floor level = 55
   Maximum height 2416
   Soft stop at 2300
   Carry = 400
*/

//static float  pid_Kp = 0.5;
//static float  pidLiftRequestedValue;

void Arm(int rArmY, int rArmB, int lArmY, int lArmB)
{
	motor[rightArmY] = rArmY;
	motor[rightArmB] = rArmB;
	motor[leftArmY] = lArmY;
	motor[leftArmB] = lArmB;
	wait1Msec(1000);
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

task autonomous()
{
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................

	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{

while(true)
	{

	//drive train
	motor[frontLeftMotor] = vexRT[Ch3];
	motor[backLeftMotor] = vexRT[Ch3];
	motor[frontRightMotor] = vexRT[Ch2];
	motor[backRightMotor] = vexRT[Ch2];
	motor[sideDrive] = vexRT[Ch1];


	//arm controls
	if((vexRT[Btn6U] == 1)&&(SensorValue[liftAngle] < 2300))
		{
			//Arm(127,127,127,127);
			motor[rightArmY] = 127;
			motor[rightArmB] = 127;
			motor[leftArmY] = 127;
			motor[leftArmB] = 127;
		}
	else if((vexRT[Btn6U] == 1)&&(SensorValue[liftAngle] > 2300))
		{
			Arm(0,0,0,0); //don't go past 2300
		}
	else if((vexRT[Btn6D] == 1)&& (SensorValue[liftAngle] > 50))
		{
			Arm(-100,-100,-100,-100);
		}
	else if((vexRT[Btn6D] == 1)&& (SensorValue[liftAngle] < 50))
		{
			Arm(0,0,0,0);	//don"t go past 50
		}
	else if(vexRT[Btn6U] == 0)
		{
			Arm(5,5,5,5);
		}
	else if(vexRT[Btn6D] == 0)
		{
			Arm(5,5,5,5);
		}

	//Lower arm to starting position
	if((vexRT[Btn8L] == 1) && (SensorValue[liftAngle] > 60))
		{
    	Arm(-100,-100,-100,-100);
			waitUntil(SensorValue[liftAngle] <= 60);
			Arm(0,0,0,0);
		}
	//Move arm to a carrying height
	if((vexRT[Btn8U] == 1) && (SensorValue[liftAngle] > 400))
		{
    	Arm(-50,-50,-50,-50);
			waitUntil(SensorValue[liftAngle] < 400);
			Arm(0,0,0,0);
		}
	else if((vexRT[Btn8U] == 1) && (SensorValue[liftAngle] < 400))
		{
    	Arm(100,100,100,100);
			waitUntil(SensorValue[liftAngle] > 400);
			Arm(0,0,0,0);
		}

	wait1Msec(25);
	}

}

/*task pidLiftControl(){

	float pidSensorCurrentValue;
 	float pidError;
 	float pidDrive;

 	while(true)
 	{
 		pidSensorCurrentValue = SensorValue[liftAngle];

 		pidError = pidLiftRequestedValue - pidSensorCurrentValue;

 		pidDrive = (pidError * pid_Kp);


	}

}*/
