#pragma config(Sensor, in1,    armAngle,       sensorPotentiometer)
#pragma config(Sensor, in2,    batmanAngle,    sensorPotentiometer)
#pragma config(Sensor, in3,    autoSwitch,     sensorPotentiometer)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           robinRightDrive, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           batmanLeftDrive, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           batmanRightDrive, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           batmanLLift,   tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           batmanRLift,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           robinRTopLift, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           robinRBottomLift, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           robinLTopLift, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           robinLBottomLift, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          robinLeftDrive, tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Robin Arm max low is 4000 // new max low is 2600 // new low 3200
// Robin Arm max hight is 1800 // new max hight is 400 // new high 400?
// Batman Arm max low is 150
// Batman Arm max high is 2200

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/* Use Void Functions instead of setting each individual motor */
void batmanDrive( int leftD, int rightD ){
motor[batmanLeftDrive] = leftD;
motor[batmanRightDrive] = rightD;
}

void robinDrive( int leftD, int rightD ){
motor[robinLeftDrive] = leftD;
motor[robinRightDrive] = rightD;
}

void batmanArmSeparate( int lArm, int rArm ) {
motor[batmanLLift] = lArm;
motor[batmanRLift] = rArm;
}

// Allows you to only input one value instead of 2 of the same
void batmanArm( int s ) {
	batmanArmSeparate( s, s );
}

void robinArmSeparate( int tL, int bL, int tR, int bR ) {
motor[robinLTopLift] = tL;
motor[robinLBottomLift] = bL;
motor[robinRTopLift] = tR;
motor[robinRBottomLift] = bR;
}

// Allows you to only input one value instead of 4 of the same
void robinArm( int s ) {
	robinArmSeparate( s, s, s, s );
}

/*
*	Uses Quadrature encoders to check if both sides of the drive train
*	are lined up with each other and compensates by makeing one side
*	slower if they are not lined up
*/
void robinDriveStraight ( int l, int r ){
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;

	while(SensorValue[rightEncoder] < r && SensorValue[leftEncoder] > l) {
		if(abs(SensorValue[rightEncoder]) == abs(SensorValue[leftEncoder])) {
			robinDrive(110, 110);
		}
		if(abs(SensorValue[rightEncoder]) > abs(SensorValue[leftEncoder])) {
			motor[robinLeftDrive] = 110;
			motor[robinRightDrive] = 100;
		}
		if(abs(SensorValue[rightEncoder]) < abs(SensorValue[leftEncoder])) {
			motor[robinLeftDrive] = 100;
			motor[robinRightDrive] = 110;
		}
	}
}

// Task for Robin in Auto
task RobinAuto ()
{
	// robin backs up from the fence
	robinDrive(-100,-100);
	wait1Msec(1000);
	robinDrive(0,0);

	// robin turns
	robinDrive(-100, 100);
	wait1Msec(650);
	robinDrive(0, 0);

	// robin hits the wall
	robinDrive(100, 100);
	wait1Msec(1750);
	robinDrive (0, 0);

	// robin turns to face away from the fence
	SensorValue[leftEncoder] = 0;
	while(SensorValue[leftEncoder] < 800) {
		robinDrive(-100, 0);
	}
	robinDrive(0,0);

	// robin lowers arm
	robinArm(-50);
	waitUntil(SensorValue[armAngle] >= 3100);
	robinArm(0);

	// robin drives forward to pick up stars
	robinDriveStraight(-1050, 1050);
	robinDrive(0,0);

	// robin picks up stars while also moving backwards
	robinArm(50);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	robinDrive(-120, -120);
	waitUntil(SensorValue[armAngle] <= 2100);
	robinArm(10);
	waitUntil(SensorValue[leftEncoder] >= 2000 && SensorValue[rightEncoder] <= -2000);
	robinDrive(0,0);

	// robin dumps stars over the fence
	robinArm(80);
	waitUntil(SensorValue[armAngle] <= 550);
	robinArm(-50);
	waitUntil(SensorValue[armAngle] >= 3100);
	robinArm(0);

}

// Task for Batman in Auto
task BatmanAuto ()
{
	// batman turns to the direction of the cube
	batmanDrive(35,-35);
	wait1Msec(1100);
	batmanDrive(0,0);

	// batman's arm lowers to ground level
	batmanArm(-20);
	waitUntil(SensorValue[batmanAngle]<= 1000);
	batmanArm(0);

	// batman moves forward and back to flip the scooper down
	batmanDrive(100,100);
	wait1Msec(1700);
	batmanDrive(0,0);

	batmanDrive(-100,-100);
	wait1Msec(500);
	batmanDrive(0,0);

/* batman picks up cube
	//batman drives forward
	batmanDrive(100,100);
	wait1Msec(2000);
	batmanDrive(0,0);

	//batman lifts scooper
	batmanArm(60);
	waitUntil(SensorValue[batmanAngle]>= 1600);
	batmanArm(10);
*/

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
	//////////////////////////Robin and Batman//////////////////////////////////////
	if(SensorValue[autoSwitch] > 2000 && SensorValue[autoSwitch] < 3200) {
		// batman arm opens out
		batmanArm(100);
		waitUntil(SensorValue[batmanAngle] >= 1700);
		batmanArm(10);

		// robin drives forward
		robinDrive(100, 100);
		wait1Msec(800);
		robinDrive(0, 0);

		// robin arm goes up and drives forward
		robinArm(40);
		SensorValue[leftEncoder] = 0;
		SensorValue[rightEncoder] = 0;
		robinDrive(100, 100);
		waitUntil(SensorValue[armAngle] <= 1200);
		robinArm(0);
		waitUntil(SensorValue[leftEncoder] <= -2400 && SensorValue[rightEncoder] >= 2400);
		robinDrive(0,0);

		startTask( RobinAuto );
		startTask( BatmanAuto );
	}

	//////////////////////////Robin NO Batman///////////////////////////////
	else if(SensorValue[autoSwitch] > 1000 && SensorValue[autoSwitch] < 2000) {
		// batman arm opens out
		batmanArm(100);
		waitUntil(SensorValue[batmanAngle] >= 1700);
		batmanArm(10);

		// robin drives forward
		robinDrive(100, 100);
		wait1Msec(800);
		robinDrive(0, 0);

		// robin arm goes up and drives forward
		robinArm(40);
		SensorValue[leftEncoder] = 0;
		SensorValue[rightEncoder] = 0;
		robinDrive(100, 100);
		waitUntil(SensorValue[armAngle] <= 1200);
		robinArm(0);
		waitUntil(SensorValue[leftEncoder] <= -2400 && SensorValue[rightEncoder] >= 2400);
		robinDrive(0,0);

		startTask( RobinAuto );
	}

	////////////////////////////////No Auto///////////////////////////////////////
	if(SensorValue[autoSwitch] > 3600) {

	}

	/////////////////////////////Robin just forward///////////////////////////
	if(SensorValue[autoSwitch] < 1000 && SensorValue[autoSwitch] >= 0) {
		batmanArm(100);
		waitUntil(SensorValue[batmanAngle] >= 1700);
		batmanArm(10);

		// robin drives forward
		robinDrive(100, 100);
		wait1Msec(800);
		robinDrive(0, 0);

		// robin arm goes up and drives forward
		robinArm(40);
		SensorValue[leftEncoder] = 0;
		SensorValue[rightEncoder] = 0;
		robinDrive(100, 100);
		waitUntil(SensorValue[armAngle] <= 1200);
		robinArm(0);
		waitUntil(SensorValue[leftEncoder] <= -2400 && SensorValue[rightEncoder] >= 2400);
		robinDrive(0,0);

		// robin drives back away from the fence
		robinDrive(-100,-100);
		wait1Msec(1000);
		robinDrive(0,0);
	}

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
	stopTask( RobinAuto );
	stopTask( BatmanAuto );

	while(true)
	{
		// Assigns robin's drive to main joystick controls.
		motor[robinLeftDrive] = vexRT[Ch3];
		motor[robinRightDrive] = vexRT[Ch2];

		// If potentiometer values is greater than 400, when you press button 6U, robin's arm will go up.
		if (vexRT[Btn6U] == 1 && SensorValue[armAngle] > 400) {
			robinArm(100);
		}

		// If potentiometer values is less than 3200, when you press button 6D, robin's arm will go down.
		else if (vexRT[Btn6D] == 1 && SensorValue[armAngle] < 3200) {
			robinArm(-100);
		}
  	// Robin's arm will continue to apply power when not moving.
 		else {
			robinArm(10);
		}

		// Assigns batman's drive to partner joystick controls.
		motor[batmanLeftDrive] = vexRT[Ch3Xmtr2];
		motor[batmanRightDrive] = vexRT[Ch2Xmtr2];

		// If potentiometer values is less than 2400, when you press button 6U (partner), batman's arm will go up.
		if (vexRT[Btn6UXmtr2] == 1 && SensorValue[batmanAngle] < 2400) {
			batmanArm(100);
		}

		// If potentiometer values is greater than 200, when you press button 6D (partner), batman's arm will go down.
		else if (vexRT[Btn6DXmtr2] == 1 && SensorValue[batmanAngle] > 200) {
			batmanArm(-100);
		}

		// Batman's arm will continue to apply power when not moving.
		else {
			batmanArm(10);
		}

		// Don't hog CPU
		wait1Msec(25);
	}
}
