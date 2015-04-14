#include "program.h"

void _entryPoint()
{
	/****************************>           Get Handles           <****************************/
	//Get a handle to coreinit.rpl
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	//Get a handle to vpad.rpl */
	unsigned int vpad_handle;
	OSDynLoad_Acquire("vpad.rpl", &vpad_handle);
	/****************************>       External Prototypes       <****************************/
	//VPAD functions
	int(*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);
	//OS functions
	void(*_Exit)();
	/****************************>             Exports             <****************************/
	//VPAD functions
	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);
	//OS functions
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	/****************************>             Globals             <****************************/
	struct pongGlobals myPongGlobals;
	//Flag for restarting the entire game.
	myPongGlobals.restart = 1;
	//scale of game
	myPongGlobals.scale=1;
	//Default locations for paddles and ball location and movement dx/dy
	myPongGlobals.p1X_default=40*myPongGlobals.scale;
	myPongGlobals.p2X_default=340*myPongGlobals.scale;
	myPongGlobals.ballX_default=200*myPongGlobals.scale;
	myPongGlobals.p1Y_default=150*myPongGlobals.scale;
	myPongGlobals.p2Y_default=150*myPongGlobals.scale;
	myPongGlobals.ballY_default=120*myPongGlobals.scale;
	//Sizes of objects
	myPongGlobals.p1X_size=20*myPongGlobals.scale;
	myPongGlobals.p1Y_size=60*myPongGlobals.scale;
	myPongGlobals.ballX_size=8*myPongGlobals.scale;
	myPongGlobals.ballY_size=8*myPongGlobals.scale;
	myPongGlobals.p2X_size=20*myPongGlobals.scale;
	myPongGlobals.p2Y_size=60*myPongGlobals.scale;
	//Boundry of play area (screen)
	myPongGlobals.xMinBoundry=0*myPongGlobals.scale;
	myPongGlobals.xMaxBoundry=400*myPongGlobals.scale;
	myPongGlobals.yMinBoundry=0*myPongGlobals.scale;
	myPongGlobals.yMaxBoundry=240*myPongGlobals.scale;
	
	myPongGlobals.winX=11*2*myPongGlobals.scale;
	myPongGlobals.winY=5*2*myPongGlobals.scale;
	myPongGlobals.score1X=13*2*myPongGlobals.scale;
	myPongGlobals.score2X=15*2*myPongGlobals.scale;
	myPongGlobals.score1Y=0*myPongGlobals.scale;
	myPongGlobals.score2Y=0*myPongGlobals.scale;
	//Game engine globals
	myPongGlobals.direction = 1;
	myPongGlobals.button = 0;
	myPongGlobals.paddleColorR=0xFF;
	myPongGlobals.paddleColorG=0x00;
	myPongGlobals.paddleColorB=0x00;
	myPongGlobals.ballColorR=0x00;
	myPongGlobals.ballColorG=0xFF;
	myPongGlobals.ballColorB=0x00;
	myPongGlobals.ballTrailColorR=0x00;
	myPongGlobals.ballTrailColorG=0x00;
	myPongGlobals.ballTrailColorB=0xFF;
	myPongGlobals.backgroundColorR=0x00;
	myPongGlobals.backgroundColorG=0x00;
	myPongGlobals.backgroundColorB=0x00;
	myPongGlobals.count = 0;
	//Keep track of score
	myPongGlobals.score1 = 0;
	myPongGlobals.score2 = 0;
	myPongGlobals.scoreWin = 9;
	//Game engine globals
	myPongGlobals.direction = 1;
	myPongGlobals.button = 0;
	myPongGlobals.paddleColorR=0xFF;
	myPongGlobals.paddleColorG=0x00;
	myPongGlobals.paddleColorB=0x00;
	myPongGlobals.ballColorR=0x00;
	myPongGlobals.ballColorG=0xFF;
	myPongGlobals.ballColorB=0x00;
	myPongGlobals.ballTrailColorR=0x00;
	myPongGlobals.ballTrailColorG=0x00;
	myPongGlobals.ballTrailColorB=0xFF;
	myPongGlobals.backgroundColorR=0x00;
	myPongGlobals.backgroundColorG=0x00;
	myPongGlobals.backgroundColorB=0x00;
	myPongGlobals.count = 0;
	//Keep track of score
	myPongGlobals.score1 = 0;
	myPongGlobals.scoreWin = 9;
	//Used for collision
	myPongGlobals.flag = 0;

	//Flag to determine if p1 should be rendered along with p1's movement direction
	myPongGlobals.renderP1Flag = 0;
	//Flags for render states
	myPongGlobals.renderResetFlag = 0;
	myPongGlobals.renderBallFlag = 0;
	myPongGlobals.renderWinFlag = 0;
	myPongGlobals.renderScoreFlag = 0;
	/****************************>            VPAD Loop            <****************************/
	int error;
	VPADData vpad_data;
	while (1)
	{
		VPADRead(0, &vpad_data, 1, &error);
		//Get the status of the gamepad
		myPongGlobals.button = vpad_data.btn_hold;
		//If the game has been restarted, reset the game (we do this one time in the beginning to set everything up)
		if (myPongGlobals.restart == 1)
		{
			reset(&myPongGlobals);
			myPongGlobals.restart = 0;
		}		
		//Set old positions.
		updatePosition(&myPongGlobals);

		//Update location of player1 and 2 paddles
		p1Move(&myPongGlobals);
		p2Move(&myPongGlobals);

		//Update location of the ball
		moveBall(&myPongGlobals);
		//Check if their are any collisions between the ball and the paddles.
		checkCollision(&myPongGlobals);
		//Render the scene
		myPongGlobals.renderBallFlag = 1;
		render(&myPongGlobals);

		//Increment the counter (used for physicals calcuations)
		myPongGlobals.count+=1;

		//To exit the game
		if (myPongGlobals.button&BUTTON_HOME)
		{
		break;
		}
	}
	//WARNING: DO NOT CHANGE THIS. YOU MUST CLEAR THE FRAMEBUFFERS AND IMMEDIATELY CALL EXIT FROM THIS FUNCTION. RETURNING TO LOADER CAUSES FREEZE.
	int ii=0;
	for(ii;ii<2;ii++)
	{
		fillScreen(0,0,0,0);
		flipBuffers();
	}
	_Exit();
}