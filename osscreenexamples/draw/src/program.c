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
	/****************************>             Function            <****************************/
	int error;
	VPADData vpad_data;
	int xpos, ypos;
	int q = 1;
	int r = 128;
	int g = 128;
	int b = 255;
	int color = 2;
	
	/* enum colors {
		color_white;
		color_grey;
		color_lightblue;
		color_lightgreen;
		color_lightred;
	}; all available choices */
	while (1)
	{
		VPADRead(0, &vpad_data, 1, &error);
		if (vpad_data.tpdata.touched == 1)
		{
			xpos = ((vpad_data.tpdata.x / 9) - 11);
			ypos = ((3930 - vpad_data.tpdata.y) / 16);

			drawPixel(xpos, ypos, r, g, b, 255);
			flipBuffers();
			drawPixel(xpos, ypos, r, g, b, 255);
			flipBuffers();
		}
		
		if (vpad_data.btn_trigger & BUTTON_LEFT) //seems to pick random one, no idea why
		{
			if (color == 0) //white
			{
				color = 4; //light red
			} else {
				color -= 1; //go down one
			}
			
			if (color == 0){ //white
				r = 255;
				g = 255;
				b = 255;
			}
			if (color == 1){ //grey
				r = 128;
				g = 128;
				b = 128;
			}
			if (color == 2){ //light blue
				r = 128;
				g = 128;
				b = 255;
			}
			if (color == 3){ //light green
				r = 128;
				g = 255;
				b = 128;
			}
			if (color == 4){ //light red
				r = 255;
				g = 128;
				b = 128;
			}
		}
		
		if (vpad_data.btn_trigger & BUTTON_RIGHT) //seems to pick random one, no idea why
		{
			if (color == 4) //light red
			{
				color = 0;  //white
			} else {
				color += 1; //go up one
			}
			
			if (color == 0){ //white
				r = 255;
				g = 255;
				b = 255;
			}
			if (color == 1){ //grey
				r = 128;
				g = 128;
				b = 128;
			}
			if (color == 2){ //light blue
				r = 128;
				g = 128;
				b = 255;
			}
			if (color == 3){ //light green
				r = 128;
				g = 255;
				b = 128;
			}
			if (color == 4){ //light red
				r = 255;
				g = 128;
				b = 128;
			}
		}
		
		if (vpad_data.btn_hold & BUTTON_PLUS)
		{
			fillScreen(0, 0, 0, 255); // black
			flipBuffers();
			fillScreen(0, 0, 0, 255); // second buffer
			flipBuffers();
		}

		if (vpad_data.btn_trigger & BUTTON_HOME)
		{
		break; //pls exit
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