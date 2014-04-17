/*
 * highLevelWalking.c
 *
 * Created: 4/9/2014 3:38:22 PM
 *  Author: jonha860
 */ 

#define F_CPU 16000000 

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serialServoControl.h"
#include "inverseKinematics.h"
#include "highLevelWalking.h"
#include "usart.h"

/*
#define pi (float) 3.14159265
#define sqrt2 (float) 1.41421356

#define x0_1 (float) (-100/sqrt2-61.85) //standard x pos for leg 1
#define y0_1 (float) (100/sqrt2+120) //standard y pos for leg 1
#define x0_2 (float) (-100-100) //standard x pos for leg 2
#define y0_2 (float) 0 //standard y pos for leg 2
#define x0_3 (float) (-100/sqrt2-61.85) //standard x pos for leg 3
#define y0_3 (float) (-100/sqrt2-120) //standard y pos for leg 3
#define x0_4 (float) (100/sqrt2+61.85) //standard x pos for leg 4
#define y0_4 (float) (-100/sqrt2-120) //standard y pos for leg 4
#define x0_5 (float) (100+100) //standard x pos for leg 5
#define y0_5 (float) 0 //standard y pos for leg 5
#define x0_6 (float) (100/sqrt2+61.85) //standard x pos for leg 6
#define y0_6 (float) (100/sqrt2+120) //standard y pos for leg 6
#define x0 (float) 100
#define y0 (float) 0
#define z0 (float) -120
*/
int speed;

/*
#define side1 (float)1
#define side2 (float)1
#define side3 (float)1
#define side4 (float)(-1)
#define side5 (float)(-1)
#define side6 (float)(-1)
*/

//#define femurAngleAddition (float)0.231 //0.2426
//#define tibiaAngleAddition (float)0.812 //(-3.1415/6)



/*
#define frontLegDistanfce (float)200
#define centerToFrontLegsY (float)120
#define centerToSideLegs (float)100
#define centerToFrontLegs (float)135
#define centerToFrontLegsX (float)61.85
*/

//Jonas function for robot movement

/*



float z0 = -120; //120 fungerar
int speed = 200;

void moveRobot(float direction,float distanfce, float rotation, float z, int servoSpeed, float rotationX, float rotationY)
{
	float sinfrotation = sinf(rotation);
	float cosfrotation = cosf(rotation);
	float sinfdirection = sinf(direction);
	float cosfdirection = cosf(direction);
	float sinfaroundx = 0;//sinf(rotationX);
	float sinfaroundy = 0;//sinf(rotationY);
	
	//First state-------------------
	servoBufferPosition(4, 3.1415/2, servoSpeed); //raise legs
	_delay_ms(5);
	servoBufferPosition(10, 3.1415/2, servoSpeed);
	_delay_ms(5);
	servoBufferPosition(15, -3.1415/2, servoSpeed);*/
	/*
	moveLeg1too(x0_1*cosfrotation-y0_1*sinfrotation-sinfdirection*distanfce, y0_1*cosfrotation-x0_1*sinfrotation+cosfdirection*distanfce, -(z+sinfaroundy*centerToFrontLegsX+sinfaroundx*centerToFrontLegsY-40), servoSpeed);
	moveLeg3too(x0_3*cosfrotation-y0_3*sinfrotation-sinfdirection*distanfce, y0_3*cosfrotation-x0_3*sinfrotation+cosfdirection*distanfce, -(z+sinfaroundy*centerToFrontLegsX-sinfaroundx*centerToFrontLegsY-40), servoSpeed);
	moveLeg5too(x0_5*cosfrotation-y0_5*sinfrotation-sinfdirection*distanfce, y0_5*cosfrotation+x0_5*sinfrotation+cosfdirection*distanfce, -(z-centerToSideLegs*sinfaroundy-40), servoSpeed);
	_delay_ms(200);
	moveLeg1too(x0_1*cosfrotation-y0_1*sinfrotation-sinfdirection*distanfce, y0_1*cosfrotation-x0_1*sinfrotation+cosfdirection*distanfce, -(z+sinfaroundy*centerToFrontLegsX+sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg3too(x0_3*cosfrotation-y0_3*sinfrotation-sinfdirection*distanfce, y0_3*cosfrotation-x0_3*sinfrotation+cosfdirection*distanfce, -(z+sinfaroundy*centerToFrontLegsX-sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg5too(x0_5*cosfrotation-y0_5*sinfrotation-sinfdirection*distanfce, y0_5*cosfrotation+x0_5*sinfrotation+cosfdirection*distanfce, -(z-centerToSideLegs*sinfaroundy), servoSpeed);
	moveLeg6too(x0_6, y0_6, -(z-sinfaroundy*centerToFrontLegsX+sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg4too(x0_4, y0_4, -(z-sinfaroundy*centerToFrontLegsX-sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg2too(x0_2, y0_2, -(z+centerToSideLegs*sinfaroundy), servoSpeed);	
	_delay_ms(300);
		*/
	//Second state-------------------
	/*servoBufferPosition(3, -3.1415/2, servoSpeed); //raise legs
	_delay_ms(5);
	servoBufferPosition(9, -3.1415/2, servoSpeed);
	_delay_ms(5);
	servoBufferPosition(16, 3.1415/2, servoSpeed);*/
	/*
	moveLeg2too(x0_2*cosfrotation-y0_2*sinfrotation-sinfdirection*distanfce, y0_2*cosfrotation+x0_2*sinfrotation+cosfdirection*distanfce, -(z+centerToSideLegs*sinfaroundy-40), servoSpeed);
	moveLeg4too(x0_4*cosfrotation-y0_4*sinfrotation-sinfdirection*distanfce, y0_4*cosfrotation-x0_4*sinfrotation+cosfdirection*distanfce, -(z-sinfaroundy*centerToFrontLegsX-sinfaroundx*centerToFrontLegsY-40), servoSpeed);
	moveLeg6too(x0_6*cosfrotation-y0_6*sinfrotation-sinfdirection*distanfce, y0_6*cosfrotation-x0_6*sinfrotation+cosfdirection*distanfce, -(z-sinfaroundy*centerToFrontLegsX+sinfaroundx*centerToFrontLegsY-40), servoSpeed);
	_delay_ms(200);
	moveLeg2too(x0_2*cosfrotation-y0_2*sinfrotation-sinfdirection*distanfce, y0_2*cosfrotation+x0_2*sinfrotation+cosfdirection*distanfce, -(z+centerToSideLegs*sinfaroundy), servoSpeed);
	moveLeg4too(x0_4*cosfrotation-y0_4*sinfrotation-sinfdirection*distanfce, y0_4*cosfrotation-x0_4*sinfrotation+cosfdirection*distanfce, -(z-sinfaroundy*centerToFrontLegsX-sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg6too(x0_6*cosfrotation-y0_6*sinfrotation-sinfdirection*distanfce, y0_6*cosfrotation-x0_6*sinfrotation+cosfdirection*distanfce, -(z-sinfaroundy*centerToFrontLegsX+sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg1too(x0_1, y0_1, -(z+sinfaroundy*centerToFrontLegsX+sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg3too(x0_3, y0_3, -(z+sinfaroundy*centerToFrontLegsX-sinfaroundx*centerToFrontLegsY), servoSpeed);
	moveLeg5too(x0_5, y0_5, -(z-centerToSideLegs*sinfaroundy), servoSpeed);
	_delay_ms(300);

	//Third state-------------------

	
	
}

*/
//Statusvariabler

//1 om benet ska lyftas, -1 om det ska vara i marken
float Leg1_lift;
float Leg2_lift;
float Leg3_lift;
float Leg4_lift;
float Leg5_lift;
float Leg6_lift;

/*
#define Gamma0 (float) 0
#define Beta0 (float) 1.875
#define Alpha0 (float) 0.1406
*/

float z = z0;

float direction;
float rotation;
float speedf;

//Position d�r f�reg�ende steg slutade
float Leg1_prev_posx;
float Leg1_prev_posy;

float Leg2_prev_posx;
float Leg2_prev_posy;

float Leg3_prev_posx;
float Leg3_prev_posy;

float Leg4_prev_posx;
float Leg4_prev_posy;

float Leg5_prev_posx;
float Leg5_prev_posy;

float Leg6_prev_posx;
float Leg6_prev_posy;


//Position d�r nuvarande steg ska sluta
float Leg1_new_posx;
float Leg1_new_posy;

float Leg2_new_posx;
float Leg2_new_posy;

float Leg3_new_posx;
float Leg3_new_posy;

float Leg4_new_posx;
float Leg4_new_posy;

float Leg5_new_posx;
float Leg5_new_posy;

float Leg6_new_posx;
float Leg6_new_posy;

float x_direction;
float y_direction;


	
float x_rot1;
float y_rot1;

float x_rot2;
float y_rot2;

float x_rot3;
float y_rot3;

float x_rot4;
float y_rot4;

float x_rot5;
float y_rot5;

float x_rot6;
float y_rot6;

float Step_max;
float Step_test;
float Step_scaling;




float Leg1_new_posx_wb;
float Leg1_new_posy_wb;
float Leg1_new_posz;
float Leg1_new_posx;
float Leg1_new_posy;
float Leg1_prev_posx;
float Leg1_prev_posy;

float Leg2_new_posx_wb;
float Leg2_new_posy_wb;
float Leg2_new_posz;
float Leg2_new_posx;
float Leg2_new_posy;
float Leg2_prev_posx;
float Leg2_prev_posy;

float Leg3_new_posx_wb;
float Leg3_new_posy_wb;
float Leg3_new_posz;
float Leg3_new_posx;
float Leg3_new_posy;
float Leg3_prev_posx;
float Leg3_prev_posy;

float Leg4_new_posx_wb;
float Leg4_new_posy_wb;
float Leg4_new_posz;
float Leg4_new_posx;
float Leg4_new_posy;
float Leg4_prev_posx;
float Leg4_prev_posy;

float Leg5_new_posx_wb;
float Leg5_new_posy_wb;
float Leg5_new_posz;
float Leg5_new_posx;
float Leg5_new_posy;
float Leg5_prev_posx;
float Leg5_prev_posy;

float Leg6_new_posx_wb;
float Leg6_new_posy_wb;
float Leg6_new_posz;
float Leg6_new_posx;
float Leg6_new_posy;
float Leg6_prev_posx;
float Leg6_prev_posy;

	//Vinklar d�r f�reg�ende steg slutade
	float Leg1_prev_angleGamma;
	float Leg1_prev_angleBeta;
	float Leg1_prev_angleAlpha;

	float Leg2_prev_angleGamma;
	float Leg2_prev_angleBeta;
	float Leg2_prev_angleAlpha;
	
	float Leg3_prev_angleGamma;
	float Leg3_prev_angleBeta;
	float Leg3_prev_angleAlpha;
	
	float Leg4_prev_angleGamma;
	float Leg4_prev_angleBeta;
	float Leg4_prev_angleAlpha;
	
	float Leg5_prev_angleGamma;
	float Leg5_prev_angleBeta;
	float Leg5_prev_angleAlpha;
	
	float Leg6_prev_angleGamma;
	float Leg6_prev_angleBeta;
	float Leg6_prev_angleAlpha;
	
	//Benkoordinater som nuvarande steget ska g� till
	float Leg1_new_angleGamma;
	float Leg1_new_angleBeta;
	float Leg1_new_angleAlpha;
	
	float Leg2_new_angleGamma;
	float Leg2_new_angleBeta;
	float Leg2_new_angleAlpha;
	
	float Leg3_new_angleGamma;
	float Leg3_new_angleBeta;
	float Leg3_new_angleAlpha;
	
	float Leg4_new_angleGamma;
	float Leg4_new_angleBeta;
	float Leg4_new_angleAlpha;
	
	float Leg5_new_angleGamma;
	float Leg5_new_angleBeta;
	float Leg5_new_angleAlpha;
	
	float Leg6_new_angleGamma;
	float Leg6_new_angleBeta;
	float Leg6_new_angleAlpha;


//L�ngd fr�n origo till standardposition f�r ben 1,3,4,6
float std_lenght;


//Max m�jliga stegl�ngd fr�n grundpositionen
float max_step_lenght;

//Middle-postioner
float Leg1_new_middle_posx;
float Leg1_new_middle_posy;

float Leg2_new_middle_posx;
float Leg2_new_middle_posy;

float Leg3_new_middle_posx;
float Leg3_new_middle_posy;

float Leg4_new_middle_posx;
float Leg4_new_middle_posy;

float Leg5_new_middle_posx;
float Leg5_new_middle_posy;

float Leg6_new_middle_posx;
float Leg6_new_middle_posy;


//Middle-vinklar
float Leg1_new_middle_angleGamma;
float Leg1_new_middle_angleBeta;
float Leg1_new_middle_angleAlpha;

float Leg2_new_middle_angleGamma;
float Leg2_new_middle_angleBeta;
float Leg2_new_middle_angleAlpha;

float Leg3_new_middle_angleGamma;
float Leg3_new_middle_angleBeta;
float Leg3_new_middle_angleAlpha;

float Leg4_new_middle_angleGamma;
float Leg4_new_middle_angleBeta;
float Leg4_new_middle_angleAlpha;

float Leg5_new_middle_angleGamma;
float Leg5_new_middle_angleBeta;
float Leg5_new_middle_angleAlpha;

float Leg6_new_middle_angleGamma;
float Leg6_new_middle_angleBeta;
float Leg6_new_middle_angleAlpha;

void initvar()
{
	speed = 300;
	max_step_lenght = 100;
	std_lenght = sqrtf(x0_1*x0_1 + y0_1*y0_1);
	
	Leg1_lift = 1;
	Leg2_lift = -1;
	Leg3_lift = 1;
	Leg4_lift = -1;	
	Leg5_lift = 1;
	Leg6_lift = -1;
	
	//Vinklar d�r f�reg�ende steg slutade
	Leg1_prev_angleGamma = Gamma0;
	Leg1_prev_angleBeta = Beta0;
	Leg1_prev_angleAlpha = Alpha0;

	Leg2_prev_angleGamma = Gamma0;
	Leg2_prev_angleBeta = Beta0;
	Leg2_prev_angleAlpha = Alpha0;
	
	Leg3_prev_angleGamma = Gamma0;
	Leg3_prev_angleBeta = Beta0;
	Leg3_prev_angleAlpha = Alpha0;
	
	Leg4_prev_angleGamma = Gamma0;
	Leg4_prev_angleBeta = Beta0;
	Leg4_prev_angleAlpha = Alpha0;
	
	Leg5_prev_angleGamma = Gamma0;
	Leg5_prev_angleBeta = Beta0;
	Leg5_prev_angleAlpha = Alpha0;
	
	Leg6_prev_angleGamma = Gamma0;
	Leg6_prev_angleBeta = Beta0;
	Leg6_prev_angleAlpha = Alpha0;
	
	//Benkoordinater som nuvarande steget ska g� till
	Leg1_new_angleGamma = Gamma0;
	Leg1_new_angleBeta = Beta0;
	Leg1_new_angleAlpha = Alpha0;
	
	Leg2_new_angleGamma = Gamma0;
	Leg2_new_angleBeta = Beta0;
	Leg2_new_angleAlpha = Alpha0;
	
	Leg3_new_angleGamma = Gamma0;
	Leg3_new_angleBeta = Beta0;
	Leg3_new_angleAlpha = Alpha0;
	
	Leg4_new_angleGamma = Gamma0;
	Leg4_new_angleBeta = Beta0;
	Leg4_new_angleAlpha = Alpha0;
	
	Leg5_new_angleGamma = Gamma0;
	Leg5_new_angleBeta = Beta0;
	Leg5_new_angleAlpha = Alpha0;
	
	Leg6_new_angleGamma = Gamma0;
	Leg6_new_angleBeta = Beta0;
	Leg6_new_angleAlpha = Alpha0;
	
	Leg1_new_posx = x0;
	Leg1_new_posy = y0;
	Leg1_new_posz = z0;
	
	Leg2_new_posx = x0;
	Leg2_new_posy = y0;
	Leg2_new_posz = z0;
	
	Leg3_new_posx = x0;
	Leg3_new_posy = y0;
	Leg3_new_posz = z0;
	
	Leg4_new_posx = x0;
	Leg4_new_posy = y0;
	Leg4_new_posz = z0;
	
	Leg5_new_posx = x0;
	Leg5_new_posy = y0;
	Leg5_new_posz = z0;
	
	Leg6_new_posx = x0;
	Leg6_new_posy = y0;
	Leg6_new_posz = z0;
}
//Tar in styrkommandon (format � la Martin) och uppdaterar variabler f�r positionen
//av ben f�r f�rra steget och r�knar ut position f�r n�sta steg.
void moveRobot(int dir, int rot, int spd)
{
	direction = (float)dir*2;
	rotation = (float)rot;
	speedf = (float)spd;
	if (speedf != 0 || rotation != 50)
	{
		

	Leg1_prev_posx = Leg1_new_posx;
	Leg1_prev_posy = Leg1_new_posy;
	
	Leg2_prev_posx = Leg2_new_posx;
	Leg2_prev_posy = Leg2_new_posy;
	
	Leg3_prev_posx = Leg3_new_posx;
	Leg3_prev_posy = Leg3_new_posy;
	
	Leg4_prev_posx = Leg4_new_posx;
	Leg4_prev_posy = Leg4_new_posy;
	
	Leg5_prev_posx = Leg5_new_posx;
	Leg5_prev_posy = Leg5_new_posy;
	
	Leg6_prev_posx = Leg6_new_posx;
	Leg6_prev_posy = Leg6_new_posy;
	
	
	//Nytt steg ska r�knas ut. Nya stegets vinklar s�tts till f�reg�ende stegets vinklar.
	Leg1_prev_angleGamma = Leg1_new_angleGamma;
	Leg1_prev_angleBeta = Leg1_new_angleBeta;
	Leg1_prev_angleAlpha = Leg1_new_angleAlpha;

	Leg2_prev_angleGamma = Leg2_new_angleGamma;
	Leg2_prev_angleBeta = Leg2_new_angleBeta;
	Leg2_prev_angleAlpha = Leg2_new_angleAlpha;

	Leg3_prev_angleGamma = Leg3_new_angleGamma;
	Leg3_prev_angleBeta = Leg3_new_angleBeta;
	Leg3_prev_angleAlpha = Leg3_new_angleAlpha;

	Leg4_prev_angleGamma = Leg4_new_angleGamma;
	Leg4_prev_angleBeta = Leg4_new_angleBeta;
	Leg4_prev_angleAlpha = Leg4_new_angleAlpha;

	Leg5_prev_angleGamma = Leg5_new_angleGamma;
	Leg5_prev_angleBeta = Leg5_new_angleBeta;
	Leg5_prev_angleAlpha = Leg5_new_angleAlpha;

	Leg6_prev_angleGamma = Leg6_new_angleGamma;
	Leg6_prev_angleBeta = Leg6_new_angleBeta;
	Leg6_prev_angleAlpha = Leg6_new_angleAlpha;


	//x och y riktning f�r f�rflyttning, skalad med hastigheten
	x_direction = -sinf(direction * pi / 90) * speedf / 100;
	y_direction = cosf(direction * pi / 90) * speedf / 100;


	//x och y riktning f�r rotation, skalad med rotationshastighet
	x_rot1 = (rotation / 50 -1) * y0_1/ std_lenght;
	y_rot1 = (rotation / 50 -1) * x0_1 / std_lenght;

	x_rot2 = 0;
	y_rot2 = rotation / 50 - 1;

	x_rot3 = (rotation / 50 -1) * (- y0_1 / std_lenght);
	y_rot3 = (rotation / 50 -1) * x0_1 / std_lenght;

	x_rot4 = (rotation / 50 -1) * (- y0_1 / std_lenght);
	y_rot4 = (rotation / 50 -1) * (- x0_1 / std_lenght);

	x_rot5 = 0;
	y_rot5 = -rotation / 50 + 1;

	x_rot6 = (rotation / 50 -1) * y0_1 / std_lenght;
	y_rot6 = (rotation / 50 -1) * (- x0_1 / std_lenght);


	//Addera f�rflyttningarna och s�tter Step_max till l�ngsta stegets l�ngd
	Step_max = powf((x_direction + x_rot1),2) + powf((y_direction + y_rot1),2);
	Step_test = powf((x_direction + x_rot2),2) + powf((y_direction + y_rot2),2);
	Step_max = fmaxf(Step_max, Step_test);

	Step_test = powf((x_direction + x_rot3),2) + powf((y_direction + y_rot3),2);
	Step_max = fmaxf(Step_max, Step_test);

	Step_test = powf((x_direction + x_rot4),2) + powf((y_direction + y_rot4),2);
	Step_max = fmaxf(Step_max, Step_test);

	Step_test = powf((x_direction + x_rot5),2) + powf((y_direction + y_rot5),2);
	Step_max = fmaxf(Step_max, Step_test);

	Step_test = powf((x_direction + x_rot6),2) + powf((y_direction + y_rot6),2);
	Step_max = fmaxf(Step_max, Step_test);
	
	Step_max =sqrtf(Step_max);


	//Stegskalning f�r att inte alltid ta max l�ngd p� steg;
	Step_scaling = fmaxf(speedf/100, fabsf(rotation/50-1));


	//Uppdatera vinklar f�r nya steget


	Leg1_new_posx_wb = Leg1_lift * max_step_lenght / Step_max * (x_direction + x_rot1) * Step_scaling;
	Leg1_new_posy_wb = Leg1_lift * max_step_lenght / Step_max * (y_direction + y_rot1) * Step_scaling;
	Leg1_new_posz = z;
	Leg1_new_posx = basis_change_Leg1x(Leg1_new_posx_wb,Leg1_new_posy_wb);
	Leg1_new_posy = basis_change_Leg1y(Leg1_new_posx_wb,Leg1_new_posy_wb);
	Calc_d(Leg1_new_posx, Leg1_new_posy, Leg1_new_posz);
	Leg1_new_angleGamma = Calc_gamma(Leg1_new_posx, Leg1_new_posy);
	Leg1_new_angleBeta = Calc_Beta();
	Leg1_new_angleAlpha = Calc_Alpha(Leg1_new_posz);


	Leg2_new_posx_wb = Leg2_lift * max_step_lenght / Step_max * (x_direction + x_rot2) * Step_scaling;
	Leg2_new_posy_wb = Leg2_lift * max_step_lenght / Step_max * (y_direction + y_rot2) * Step_scaling;
	Leg2_new_posz = z;
	Leg2_new_posx = basis_change_Leg2x(Leg2_new_posx_wb);
	Leg2_new_posy = basis_change_Leg2y(Leg2_new_posy_wb);
	Calc_d(Leg2_new_posx, Leg2_new_posy, Leg2_new_posz);
	Leg2_new_angleGamma = Calc_gamma(Leg2_new_posx, Leg2_new_posy);
	Leg2_new_angleBeta = Calc_Beta();
	Leg2_new_angleAlpha = Calc_Alpha(Leg2_new_posz);


	Leg3_new_posx_wb = Leg3_lift * max_step_lenght / Step_max * (x_direction + x_rot3) * Step_scaling;
	Leg3_new_posy_wb = Leg3_lift * max_step_lenght / Step_max * (y_direction + y_rot3) * Step_scaling;
	Leg3_new_posz = z;
	Leg3_new_posx = basis_change_Leg3x(Leg3_new_posx_wb,Leg3_new_posy_wb);
	Leg3_new_posy = basis_change_Leg3y(Leg3_new_posx_wb,Leg3_new_posy_wb);
	Calc_d(Leg3_new_posx, Leg3_new_posy, Leg3_new_posz);
	Leg3_new_angleGamma = Calc_gamma(Leg3_new_posx, Leg3_new_posy);
	Leg3_new_angleBeta = Calc_Beta();
	Leg3_new_angleAlpha = Calc_Alpha(Leg3_new_posz);


	Leg4_new_posx_wb = Leg4_lift * max_step_lenght / Step_max * (x_direction + x_rot4) * Step_scaling;
	Leg4_new_posy_wb = Leg4_lift * max_step_lenght / Step_max * (y_direction + y_rot4) * Step_scaling;
	Leg4_new_posz = z;
	Leg4_new_posx = basis_change_Leg4x(Leg4_new_posx_wb,Leg4_new_posy_wb);
	Leg4_new_posy = basis_change_Leg4y(Leg4_new_posx_wb,Leg4_new_posy_wb);
	Calc_d(Leg4_new_posx, Leg4_new_posy, Leg4_new_posz);
	Leg4_new_angleGamma = Calc_gamma(Leg4_new_posx, Leg4_new_posy);
	Leg4_new_angleBeta = Calc_Beta();
	Leg4_new_angleAlpha = Calc_Alpha(Leg4_new_posz);


	Leg5_new_posx_wb = Leg5_lift * max_step_lenght / Step_max * (x_direction + x_rot5) * Step_scaling;
	Leg5_new_posy_wb = Leg5_lift * max_step_lenght / Step_max * (y_direction + y_rot5) * Step_scaling;
	Leg5_new_posz = z;
	Leg5_new_posx = basis_change_Leg5x(Leg5_new_posx_wb);
	Leg5_new_posy = basis_change_Leg5y(Leg5_new_posy_wb);
	Calc_d(Leg5_new_posx, Leg5_new_posy, Leg5_new_posz);
	Leg5_new_angleGamma = Calc_gamma(Leg5_new_posx, Leg5_new_posy);
	Leg5_new_angleBeta = Calc_Beta();
	Leg5_new_angleAlpha = Calc_Alpha(Leg5_new_posz);


	Leg6_new_posx_wb = Leg6_lift * max_step_lenght / Step_max * (x_direction + x_rot6) * Step_scaling;
	Leg6_new_posy_wb = Leg6_lift * max_step_lenght / Step_max * (y_direction + y_rot6) * Step_scaling;
	Leg6_new_posz = z;
	Leg6_new_posx = basis_change_Leg6x(Leg6_new_posx_wb,Leg6_new_posy_wb);
	Leg6_new_posy = basis_change_Leg6y(Leg6_new_posx_wb,Leg6_new_posy_wb);
	Calc_d(Leg6_new_posx, Leg6_new_posy, Leg6_new_posz);
	Leg6_new_angleGamma = Calc_gamma(Leg6_new_posx, Leg6_new_posy);
	Leg6_new_angleBeta = Calc_Beta();
	Leg6_new_angleAlpha = Calc_Alpha(Leg6_new_posz);


	if(Leg1_lift == -1)
	{
		Leg1_new_middle_posx = Leg1_prev_posx + (Leg1_new_posx - Leg1_prev_posx)/2;
		Leg1_new_middle_posy = Leg1_prev_posy + (Leg1_new_posy - Leg1_prev_posy)/2;
		Calc_d(Leg1_new_middle_posx, Leg1_new_middle_posy, z);
		Leg1_new_middle_angleGamma = Calc_gamma(Leg1_new_middle_posx, Leg1_new_middle_posy);
		Leg1_new_middle_angleBeta = Calc_Beta();
		Leg1_new_middle_angleAlpha = Calc_Alpha(Leg1_new_posz);
	}
	
	if(Leg2_lift == -1)
	{
		Leg2_new_middle_posx = Leg2_prev_posx + (Leg2_new_posx - Leg2_prev_posx)/2;
		Leg2_new_middle_posy = Leg2_prev_posy + (Leg2_new_posy - Leg2_prev_posy)/2;
		Calc_d(Leg2_new_middle_posx, Leg2_new_middle_posy, z);
		Leg2_new_middle_angleGamma = Calc_gamma(Leg2_new_middle_posx, Leg2_new_middle_posy);
		Leg2_new_middle_angleBeta = Calc_Beta();
		Leg2_new_middle_angleAlpha = Calc_Alpha(Leg2_new_posz);
	}
	
	if(Leg3_lift == -1)
	{
		Leg3_new_middle_posx = Leg3_prev_posx + (Leg3_new_posx - Leg3_prev_posx)/2;
		Leg3_new_middle_posy = Leg3_prev_posy + (Leg3_new_posy - Leg3_prev_posy)/2;
		Calc_d(Leg3_new_middle_posx, Leg3_new_middle_posy,z);
		Leg3_new_middle_angleGamma = Calc_gamma(Leg3_new_middle_posx, Leg3_new_middle_posy);
		Leg3_new_middle_angleBeta = Calc_Beta();
		Leg3_new_middle_angleAlpha = Calc_Alpha(Leg3_new_posz);
	}
	
	if(Leg4_lift == -1)
	{
		Leg4_new_middle_posx = Leg4_prev_posx + (Leg4_new_posx - Leg4_prev_posx)/2;
		Leg4_new_middle_posy = Leg4_prev_posy + (Leg4_new_posy - Leg4_prev_posy)/2;
		Calc_d(Leg4_new_middle_posx, Leg4_new_middle_posy, z);
		Leg4_new_middle_angleGamma = Calc_gamma(Leg4_new_middle_posx, Leg4_new_middle_posy);
		Leg4_new_middle_angleBeta = Calc_Beta();
		Leg4_new_middle_angleAlpha = Calc_Alpha(Leg4_new_posz);
	}
	
	if(Leg5_lift == -1)
	{
		Leg5_new_middle_posx = Leg5_prev_posx + (Leg5_new_posx - Leg5_prev_posx)/2;
		Leg5_new_middle_posy = Leg5_prev_posy + (Leg5_new_posy - Leg5_prev_posy)/2;
		Calc_d(Leg5_new_middle_posx, Leg5_new_middle_posy,z);
		Leg5_new_middle_angleGamma = Calc_gamma(Leg5_new_middle_posx, Leg5_new_middle_posy);
		Leg5_new_middle_angleBeta = Calc_Beta();
		Leg5_new_middle_angleAlpha = Calc_Alpha(Leg5_new_posz);
	}
	
	if(Leg6_lift == -1)
	{
		Leg6_new_middle_posx = Leg6_prev_posx + (Leg6_new_posx - Leg6_prev_posx)/2;
		Leg6_new_middle_posy = Leg6_prev_posy + (Leg6_new_posy - Leg6_prev_posy)/2;
		Calc_d(Leg6_new_middle_posx, Leg6_new_middle_posy,z);
		Leg6_new_middle_angleGamma = Calc_gamma(Leg6_new_middle_posx, Leg6_new_middle_posy);
		Leg6_new_middle_angleBeta = Calc_Beta();
		Leg6_new_middle_angleAlpha = Calc_Alpha(Leg6_new_posz);
	}
	


	Leg_motion();
	}
}


void Leg_motion()
{
	speed = 150;
	if (Leg1_lift == -1)
	{
		servoBufferPosition(8,Leg1_new_middle_angleGamma,speed);
		_delay_us(100);
		servoBufferPosition(10,side1*(Leg1_new_middle_angleAlpha + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(12,side1*(-Leg1_new_middle_angleBeta + tibiaAngleAddition),speed);
	}
	else
	{
		servoBufferPosition(8,Leg1_prev_angleGamma + (Leg1_new_angleGamma-Leg1_prev_angleGamma)/2 ,speed);
		_delay_us(100);
		servoBufferPosition(10,side1*(Leg1_prev_angleAlpha + (Leg1_new_angleAlpha-Leg1_prev_angleAlpha)/2 + 0.5 + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(12,side1*(-Leg1_prev_angleBeta - (Leg1_new_angleBeta-Leg1_prev_angleBeta)/2 + tibiaAngleAddition),speed);
	}
	_delay_ms(1);

	if (Leg2_lift == -1)
	{
		servoBufferPosition(14,Leg2_new_middle_angleGamma,speed);
		_delay_us(100);
		servoBufferPosition(16,side2*(Leg2_new_middle_angleAlpha + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(18,side2*(-Leg2_new_middle_angleBeta + tibiaAngleAddition),speed);
	}
	else
	{
		servoBufferPosition(14,Leg2_prev_angleGamma + (Leg2_new_angleGamma-Leg2_prev_angleGamma)/2 ,speed);
		_delay_us(100);
		servoBufferPosition(16,side2*(Leg2_prev_angleAlpha + (Leg2_new_angleAlpha-Leg2_prev_angleAlpha)/2 + 0.5 + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(18,side2*(-Leg2_prev_angleBeta - (Leg2_new_angleBeta-Leg2_prev_angleBeta)/2 + tibiaAngleAddition),speed);
	}
	_delay_ms(1);

	if (Leg3_lift == -1)
	{
		servoBufferPosition(2,Leg3_new_middle_angleGamma,speed);
		_delay_us(100);
		servoBufferPosition(4,side3*(Leg3_new_middle_angleAlpha + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(6,side3*(-Leg3_new_middle_angleBeta + tibiaAngleAddition),speed);
	}
	else
	{
		servoBufferPosition(2,Leg3_prev_angleGamma + (Leg3_new_angleGamma-Leg3_prev_angleGamma)/2 ,speed);
		_delay_us(100);
		servoBufferPosition(4,side3*(Leg3_prev_angleAlpha + (Leg3_new_angleAlpha-Leg3_prev_angleAlpha)/2 + 0.5 + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(6,side3*(-Leg3_prev_angleBeta - (Leg3_new_angleBeta-Leg3_prev_angleBeta)/2 + tibiaAngleAddition),speed);
	}
	_delay_ms(1);

	if (Leg4_lift == -1)
	{
		servoBufferPosition(1,Leg4_new_middle_angleGamma,speed);
		_delay_us(100);
		servoBufferPosition(3,side4*(Leg4_new_middle_angleAlpha + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(5,side4*(-Leg4_new_middle_angleBeta + tibiaAngleAddition),speed);
	}
	else
	{
		servoBufferPosition(1,Leg4_prev_angleGamma + (Leg4_new_angleGamma-Leg4_prev_angleGamma)/2 ,speed);
		_delay_us(100);
		servoBufferPosition(3,side4*(Leg4_prev_angleAlpha + (Leg4_new_angleAlpha-Leg4_prev_angleAlpha)/2 + 0.5 + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(5,side4*(-Leg4_prev_angleBeta - (Leg4_new_angleBeta-Leg4_prev_angleBeta)/2 + tibiaAngleAddition),speed);
	}
	_delay_ms(1);

	if (Leg5_lift == -1)
	{
		servoBufferPosition(13,Leg5_new_middle_angleGamma,speed);
		_delay_us(100);
		servoBufferPosition(15,side5*(Leg5_new_middle_angleAlpha + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(17,side5*(-Leg5_new_middle_angleBeta + tibiaAngleAddition),speed);
	}
	else
	{
		servoBufferPosition(13,Leg5_prev_angleGamma + (Leg5_new_angleGamma-Leg5_prev_angleGamma)/2 ,speed);
		_delay_us(100);
		servoBufferPosition(15,side5*(Leg5_prev_angleAlpha + (Leg5_new_angleAlpha-Leg5_prev_angleAlpha)/2 + 0.5 + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(17,side5*(-Leg5_prev_angleBeta - (Leg5_new_angleBeta-Leg5_prev_angleBeta)/2 + tibiaAngleAddition),speed);
	}
_delay_ms(1);

	if (Leg6_lift == -1)
	{
		servoBufferPosition(7,Leg6_new_middle_angleGamma,speed);
		_delay_us(100);
		servoBufferPosition(9,side6*(Leg6_new_middle_angleAlpha + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(11,side6*(-Leg6_new_middle_angleBeta + tibiaAngleAddition),speed);
	}
	else
	{
		servoBufferPosition(7,Leg6_prev_angleGamma + (Leg6_new_angleGamma-Leg6_prev_angleGamma)/2 ,speed);
		_delay_us(100);
		servoBufferPosition(9,side6*(Leg6_prev_angleAlpha + (Leg6_new_angleAlpha-Leg6_prev_angleAlpha)/2 + 0.5 + femurAngleAddition),speed);
		_delay_us(100);
		servoBufferPosition(11,side6*(-Leg6_prev_angleBeta - (Leg6_new_angleBeta-Leg6_prev_angleBeta)/2 + tibiaAngleAddition),speed);
	}
servoAction();
	_delay_ms(200);

	servoBufferPosition(8,Leg1_new_angleGamma,speed);
	_delay_us(100);
	servoBufferPosition(10,side1*(Leg1_new_angleAlpha + femurAngleAddition),speed);
	_delay_us(100);
	servoBufferPosition(12,side1*(-Leg1_new_angleBeta + tibiaAngleAddition),speed);

	_delay_ms(1);

	servoBufferPosition(14,Leg2_new_angleGamma,speed);
	_delay_us(100);
	servoBufferPosition(16,side2*(Leg2_new_angleAlpha + femurAngleAddition),speed);
	_delay_us(100);
	servoBufferPosition(18,side2*(-Leg2_new_angleBeta + tibiaAngleAddition),speed);

	_delay_ms(1);

	servoBufferPosition(2,Leg3_new_angleGamma,speed);
	_delay_us(100);
	servoBufferPosition(4,side3*(Leg3_new_angleAlpha + femurAngleAddition),speed);
	_delay_us(100);
	servoBufferPosition(6,side3*(-Leg3_new_angleBeta + tibiaAngleAddition),speed);

	_delay_ms(1);

	servoBufferPosition(1,Leg4_new_angleGamma,speed);
	_delay_us(100);
	servoBufferPosition(3,side4*(Leg4_new_angleAlpha + femurAngleAddition),speed);
	_delay_us(100);
	servoBufferPosition(5,side4*(-Leg4_new_angleBeta + tibiaAngleAddition),speed);

	_delay_ms(1);

	servoBufferPosition(13,Leg5_new_angleGamma,speed);
	_delay_us(100);
	servoBufferPosition(15,side5*(Leg5_new_angleAlpha + femurAngleAddition),speed);
	_delay_us(100);
	servoBufferPosition(17,side5*(-Leg5_new_angleBeta + tibiaAngleAddition),speed);

	_delay_ms(1);

	servoBufferPosition(7,Leg6_new_angleGamma,speed);
	_delay_us(100);
	servoBufferPosition(9,side6*(Leg6_new_angleAlpha + femurAngleAddition),speed);
	_delay_us(100);
	servoBufferPosition(11,side6*(-Leg6_new_angleBeta + tibiaAngleAddition),speed);
servoAction();
	_delay_ms(200);
}


int main(void)
{
	DDRD |= (1<<PORTD5); //init LED
	//servoTx;
	
	servoRetrunLevel(BROADCASTING_ID, 1); //turns off return packets
	
	sei();
	initServoSerial(); //Init servos
	USART_init();
	initvar();
	
	/*
	float alpha = 3.1415/4;
	float beta = 3.1415/2.2;
	int speed = 180;
	
	//dummy code that puts robot in stanfdard position
	
	servoBufferPosition(1, 0, speed);
	servoBufferPosition(2, 0, speed);
	servoBufferPosition(7, 0, speed);
	servoBufferPosition(8, 0, speed);
	servoBufferPosition(13, 0, speed);
	servoBufferPosition(14, 0, speed);
	
	_delay_ms(100);
	
	servoBufferPosition(15, -alpha, speed);
	servoBufferPosition(3, -alpha, speed);
	servoBufferPosition(9, -alpha, speed);
	servoBufferPosition(16, alpha, speed);
	servoBufferPosition(4, alpha, speed);
	servoBufferPosition(10, alpha, speed);
	
	_delay_ms(100);
	
	servoBufferPosition(12, -beta, speed);
	servoBufferPosition(18, -beta, speed);
	servoBufferPosition(6, -beta, speed);
	servoBufferPosition(5, beta, speed);
	servoBufferPosition(11, beta, speed);
	servoBufferPosition(17, beta, speed);
	
	_delay_ms(5000);
	*/
	moveLeg1too(x0_1, y0_1, z0, speed);
	moveLeg2too(x0_2, y0_2, z0, speed);
	moveLeg3too(x0_3, y0_3, z0, speed);
	moveLeg4too(x0_4, y0_4, z0, speed);
	moveLeg5too(x0_5, y0_5, z0, speed);
	moveLeg6too(x0_6, y0_6, z0, speed);
	
	
	_delay_ms(5000);
	
	//moveRobotTob(0,50,100);
    while(1)
    {
		//moveRobot(USART_getDirection(),USART_getRotation(),USART_getSpeed());
		moveRobot(0,50,100);
		Leg1_lift = -Leg1_lift;
		Leg2_lift = -Leg2_lift;
		Leg3_lift = -Leg3_lift;
		Leg4_lift = -Leg4_lift;
		Leg5_lift = -Leg5_lift;
		Leg6_lift = -Leg6_lift;
		
		
		//USART_DecodeRxFIFO();

		//moveRobot((float)0,(float)40,(float)0,(float)120,(int)100,(float)0,(float)0);


        //TODO:: Please write your application code 
    }
}