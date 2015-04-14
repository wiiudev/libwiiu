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
	struct renderFlags flags;
	flags.y=0;
	flags.x=0;
	flags.a=0;
	flags.b=0;
	/****************************>            VPAD Loop            <****************************/

	/* Enter the VPAD loop */
	int error;
	VPADData vpad_data;
	//Read initial vpad status
	VPADRead(0, &vpad_data, 1, &error);
	
	while(1)
	{
		VPADRead(0, &vpad_data, 1, &error);

		//button A
		if (vpad_data.btn_hold & BUTTON_A)
		{
			flags.y=0;
			flags.x=0;
			flags.a=1;
			flags.b=0;
			__os_snprintf(flags.output, 1000, "A button pressed");
		}
	
		//button B
		else if (vpad_data.btn_hold & BUTTON_B)
		{
			flags.y=0;
			flags.x=0;
			flags.a=0;
			flags.b=1;
			__os_snprintf(flags.output, 1000, "B button pressed");
		}
	
		//button X
		else if (vpad_data.btn_hold & BUTTON_X)
		{
			flags.y=0;
			flags.x=1;
			flags.a=0;
			flags.b=0;
			__os_snprintf(flags.output, 1000, "X button pressed");
		}
	
		//button Y
		else if (vpad_data.btn_hold & BUTTON_Y)
		{
			flags.y=1;
			flags.x=0;
			flags.a=0;
			flags.b=0;
			__os_snprintf(flags.output, 1000, "Y button pressed");
		}
		
		if(vpad_data.btn_hold & BUTTON_HOME)
		{
		break;
		}
		render(&flags);
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

void render(struct renderFlags *flags)
{
	int i=0;
	for(i;i<2;i++)
	{
		if(flags->y)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		if(flags->x)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		if(flags->a)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		if(flags->b)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		flipBuffers();
	}
	flags->a=0;
	flags->b=0;
	flags->x=0;
	flags->y=0;
}