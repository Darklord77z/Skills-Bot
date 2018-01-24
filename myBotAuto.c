#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           leftD,         tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port3,           rightD,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port4,           lift,          tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
void initialize(){
	nMotorEncoder[leftD]=0;
	wait1Msec(100);
	nMotorEncoder[rightD]=0;
	wait1Msec(100);
	nMotorEncoder[lift]=0;
	wait1Msec(100);
}
void liftPID(float target){
	float kpLift=0.5;
	float kiLift=0;
	float kdLift=2;

	float errorPLift;
	float errorILift;
	float errorDLift;
	float proportionalLift;
	float integralLift;
	float integralLimitLift=50;
	float integralActiveZoneLift=100;
	float derivativeLift;

	float powerLift;
	clearTimer(T2);
	while(time1(T2)<1000){
		errorPLift=target-nMotorEncoder[lift];
		proportionalLift=errorPLift*kpLift;

		//INTEGRAL
		if(abs(errorPLift)<integralActiveZoneLift){
			errorILift+=errorPLift;
		}
		else{
			errorILift=0;
		}
		integralLift=errorILift*kiLift;
		if(integralLift>integralLimitLift){
			integralLift=integralLimitLift;
		}
		//DERIVATIVE
		derivativeLift=(errorPLift-errorDLift)*kdLift;
		errorDLift=errorPLift;

		powerLift=proportionalLift+integralLift+derivativeLift;

		motor[lift]=-powerLift;
	}
}

void moveStraight(float targetTicks){

	int dummyCounter;
	float proportionalL;
	float proportionalR;
	float integralL;
	float integralR;
	float derivativeL;
	float derivativeR;
	float proportional;
	float integral;
	float derivative;
	float integralActiveZone=100;
	float integralLimit=40;
	float errorPL;
	float errorPR;
	float errorIL;
	float errorIR;
	float errorDL;
	float errorDR;
	float kp=0.15;
	float ki=0;
	float kd=2;
	float power;

	clearTimer(T1);
	initialize();
	//float targetTicks=tilesToTicks(numOfTiles);
	while(time1(T1)<1500&&dummyCounter<60){
		//PROPORTIONAL
		errorPL=targetTicks-nMotorEncoder[leftD];
		errorPR=targetTicks-nMotorEncoder[rightD];
		proportionalL=errorPL*kp;
		proportionalR=errorPR*kp;
		//TIMER
		if(errorPL<40 || errorPR<40){
			dummyCounter++;
		}
		//INTEGRAL
		if(abs(errorPL)<integralActiveZone || abs(errorPR)<integralActiveZone){
			errorIL+=errorPL;
			errorIR+=errorPR;
		}
		else{
			errorIL=0;
			errorIR=0;
		}
		integralL=errorIL*ki;
		integralR=errorIR*ki;
		if(integralL>integralLimit || integralR>integralLimit){
			integralL=integralLimit;
			integralR=integralLimit;
		}
		//DERIVATIVE
		derivativeL=(errorPL-errorDL)*kd;
		derivativeR=(errorPR-errorDR)*kd;
		errorDL=errorPL;
		errorDR=errorPR;
		if(errorPL==0 || errorPR==0){
			derivativeL=0;
			derivativeR=0;
		}
		proportional=(proportionalL+proportionalR)/2;
		integral=(integralL+integralR)/2;
		derivative=(derivativeL+derivativeR)/2;
		power=proportional+integral+derivative;
		motor[leftD]=power;
		motor[rightD]=power;
	}
}

task main()
{
	initialize();
	//wait1Msec(200);
	//liftPID(630);
	//wait1Msec(2000);
	//liftPID(-40);
	while(1){
		moveStraight(700);
		sleep(500);
		moveStraight(-700);
	}
}
