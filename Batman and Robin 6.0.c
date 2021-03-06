
#pragma config(Sensor, in1,    armAngle,       sensorPotentiometer)
#pragma config(Sensor, in2,    batmanAngle,    sensorPotentiometer)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           robinRDrive,   tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           batmanLeftDrive, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           batmanRightDrive, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           batmanLLift,   tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           batmanRLift,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           robinRTopLift, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           robinRBottomLift, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           robinLTopLift, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           robinLBottomLift, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          robinLDrive,   tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// robin low is 4000 // new max low is 2600
//robin hight is 1800 // new max hight is 400
// batman low is 150
//batman high is 2200

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

void batmanDrive(int leftD, int rightD){
motor[batmanLeftDrive] = leftD;
motor[batmanRightDrive] = rightD;
}

void robinDrive(int leftD, int rightD){
motor[robinLDrive] = leftD;
motor[robinRDrive] = rightD;
}

void batmanArm(int lArm, int rArm) {
motor[batmanLLift] = lArm;
motor[batmanRLift] = rArm;
}

void robinArm(int tL, int bL, int tR, int bR) {
motor[robinLTopLift] = tL;
motor[robinLBottomLift] = bL;
motor[robinRTopLift] = tR;
motor[robinRBottomLift] = bR;
}

void robinDriveStraight (int l, int r){
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;

	while(SensorValue[rightEncoder] > r && SensorValue[leftEncoder] < l) {
		if(abs(SensorValue[rightEncoder]) == abs(SensorValue[leftEncoder])) {
			robinDrive(60, 60);
		}
		if(abs(SensorValue[rightEncoder]) > abs(SensorValue[leftEncoder])) {
			motor[robinLDrive] = 60;
			motor[robinRDrive] = 50;
		}
		if(abs(SensorValue[rightEncoder]) < abs(SensorValue[leftEncoder])) {
			motor[robinLDrive] = 50;
			motor[robinRDrive] = 60;
		}
	}
}

//Task for robin in Auto
task RobinAuto ()
{
	//robin backs up from the fence
	robinDrive(-100,-100);
	wait1Msec(1750);
	robinDrive(0,0);

	//robin turns around
	robinDrive(0,75);
	wait1Msec(1750);
	robinDrive(0,0);

	//robin drives forward
	robinDrive(100,100);
	wait1Msec(750);
	robinDrive(0,0);

	//robin turns around
	robinDrive(0,75);
	wait1Msec(1750);
	robinDrive(0,0);


	//robin drives backward
	robinDrive(-100,-100);
	wait1Msec(500);
	robinDrive(0,0);

	//robin arm goes down
	robinArm(-70, -70, -70, -70);
	waitUntil(SensorValue[armAngle] >= 2200);
	robinArm(0,0,0,0);


	//robin drives forward
	robinDrive(100,100);
	wait1Msec(1600);
	robinDrive(0,0);
}

//Task for batman in Auto
task BatmanAuto ()
{
	//batman turns to the direction of the cube
	batmanDrive(35,-35);
	wait1Msec(1100);
	batmanDrive(0,0);

	//batman's arm lowers to ground level
	batmanArm(-20,-20);
	waitUntil(SensorValue[batmanAngle]<= 450);
	batmanArm(0,0);

	batmanDrive(30,30);
	wait1Msec(400);
	batmanDrive(0,0);

	batmanDrive(-30,-30);
	wait1Msec(300);
	batmanDrive(0,0);

	batmanDrive(0,0);
	wait1Msec(300);
	batmanDrive(0,0);

	//batman drives forward to disengage scooper
	batmanDrive(0,0);
	wait1Msec(400);
	batmanDrive(0,0);

	//batman drives forward towards the cube
	batmanDrive(70,70);
	wait1Msec(2000);
	batmanDrive(0,0);

	//batman arm opens out
	batmanArm(70, 70);
	waitUntil(SensorValue[batmanAngle] >= 1000);
	batmanArm(10, 10);


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
	//batman arm opens out
	batmanArm(70, 70);
	waitUntil(SensorValue[batmanAngle] >= 1700);
	batmanArm(10, 10);

	//robin drives forward
	robinDrive(100, 100);
	wait1Msec(800);
	robinDrive(0, 0);

	//robin arm goes up
	robinArm(70, 70, 70, 70);
	waitUntil(SensorValue[armAngle] <= 470);
	robinArm(0,0,0,0);

	//robin drives forward to knock stars on fence
	robinDrive(100, 100);
	wait1Msec(3300);
	robinDrive(0, 0);

	startTask(RobinAuto);
	startTask(BatmanAuto);

//AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
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
// User control code here, inside the loop
stopTask(RobinAuto);
stopTask(BatmanAuto);

	while(true)
	{
		//Assigns robin's drive to main joystick controls.
		motor[robinLDrive] = vexRT[Ch3];
		motor[robinRDrive] = vexRT[Ch2];

		//if potentiometer values is greater than 400, when you press button 6U, robin's arm will go up.
		if (vexRT[Btn6U] == 1 && SensorValue[armAngle] > 400) {
			motor[robinLTopLift] = 100;
			motor[robinLBottomLift] = 100;
			motor[robinRTopLift] = 100;
			motor[robinRBottomLift] = 100;
		}

		//if potentiometer values is less than 2200, when you press button 6D, robin's arm will go down.
		else if (vexRT[Btn6D] == 1 && SensorValue[armAngle] < 2200) {
			motor[robinLTopLift] = -100;
			motor[robinLBottomLift] = -100;
			motor[robinRTopLift] = -100;
			motor[robinRBottomLift] = -100;
		}
  	//robin's arm will continue to apply power when lifting.
 		else {
			motor[robinLTopLift] = 10;
			motor[robinLBottomLift] = 10;
			motor[robinRTopLift] = 10;
			motor[robinRBottomLift] = 10;
		}

		//Assigns batman's drive to partner joystick controls.
		motor[batmanLeftDrive] = vexRT[Ch3Xmtr2];
		motor[batmanRightDrive] = vexRT[Ch2Xmtr2];

		//if potentiometer values is less than 2400, when you press button 6U (partner), batman's arm will go up.
		if (vexRT[Btn6UXmtr2] == 1 && SensorValue[batmanAngle] < 2400) {
		motor[batmanLLift] = 100;
		motor[batmanRLift] = 100;
		}

		//if potentiometer values is greater than 200, when you press button 6D (partner), batman's arm will go down.
		else if (vexRT[Btn6DXmtr2] == 1 && SensorValue[batmanAngle] > 200) {
		motor[batmanLLift] = -100;
		motor[batmanRLift] = -100;
		}

		//batman's arm will continue to apply power when lifting.
		else {
		motor[batmanLLift] = 10;
		motor[batmanRLift] = 10;
		}

		wait1Msec(25);
	}
}
