#include "program.h"

// downscale factor for the touch coordinates
#define DOWNSCALE_PAD 3
#define DOWNSCALE_TV 2

// number of render buffers
#define BUFFERS 2

#define COLORS_AMOUNT 5

// physical values
#define PAD_HEIGHT 480
#define COLOR_PANEL_WIDTH 50

// get the red value of a color
inline int r(int color) {
	if (color == 0) { // black
		return 0;
	} else if (color == 1) { // grey
		return 128;
	} else if (color == 2) { // red
		return 255;
	} else if (color == 3) { // green
		return 0;
	} else if (color == 4) { // blue
		return 0;
	} else {
		return -1;
	}
}

// get the green value of a color
inline int g(int color) {
	if (color == 0) { // black
		return 0;
	} else if (color == 1) { // grey
		return 128;
	} else if (color == 2) { // red
		return 0;
	} else if (color == 3) { // green
		return 255;
	} else if (color == 4) { // blue
		return 0;
	} else {
		return -1;
	}
}

// get the blue value of a color
inline int b(int color) {
	if (color == 0) { // black
		return 0;
	} else if (color == 1) { // grey
		return 128;
	} else if (color == 2) { // red
		return 0;
	} else if (color == 3) { // green
		return 0;
	} else if (color == 4) { // blue
		return 255;
	} else {
		return -1;
	}
}

void drawColorChooser(int color) {
	int i, button_height = PAD_HEIGHT / COLORS_AMOUNT;

	for(i = 0; i < 6; i++) {
		drawFillRect(SCREEN_PAD, 1, 0, i * button_height, COLOR_PANEL_WIDTH, (i + 1) * button_height, r(i), g(i), b(i), 255);
		if(i == color) {
			// draw cross
			drawLine(SCREEN_PAD, 1, 0, i * button_height, COLOR_PANEL_WIDTH - 1, (i + 1) * button_height, 255, 255, 255, 255);
			drawLine(SCREEN_PAD, 1, 1, i * button_height, COLOR_PANEL_WIDTH, (i + 1) * button_height, 255, 255, 255, 255);

			drawLine(SCREEN_PAD, 1, COLOR_PANEL_WIDTH - 1, i * button_height, 0, (i + 1) * button_height, 255, 255, 255, 255);
			drawLine(SCREEN_PAD, 1, COLOR_PANEL_WIDTH, i * button_height, 1, (i + 1) * button_height, 255, 255, 255, 255);
		}
	}
}

void reset(int color) {
	int i;
	for(i = 0; i < BUFFERS; i++) {
		fillScreen(SCREEN_ALL, 255, 255, 255, 255);
		drawColorChooser(color);
		flipBuffers(SCREEN_ALL);
	}
}

void drawColorChooserAndShow(int color) {
	int i;
	for(i = 0; i < BUFFERS; i++) {
		drawColorChooser(color);
		flipBuffers(SCREEN_ALL);
	}
}

void _entryPoint() {
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
	// the last position are -1 if the screen wasn't touched, if they are set, a line to the previous point is drawn
	int lastxpos = -1, lastypos = -1;
	// the last color is used to see if the color was changed
	int color = 0, last_color = 0;
	int i;
	
	reset(color);

	while (1) {
		VPADRead(0, &vpad_data, 1, &error);
		if (vpad_data.tpdata.touched == 1) {
			if(vpad_data.tpdata.validity == 0) {
				// only process valid touchscreen data
				xpos = vpad_data.tpdata.x * 2 / 3 - 66;
				ypos = 1474 - vpad_data.tpdata.y * 3 / 8;

				if(xpos / DOWNSCALE_PAD <= COLOR_PANEL_WIDTH) {
					// touch at color chooser
					color = ypos * COLORS_AMOUNT / (DOWNSCALE_PAD * PAD_HEIGHT);
				}else{
					// draw line from previous point or dot
					// this uses different scales to make it fill both screens (not only the left upper part of the tv)
					for(i = 0; i < BUFFERS; i++) {
						if(lastxpos == -1) {
							drawPixel(SCREEN_PAD, 1, xpos / DOWNSCALE_PAD, ypos / DOWNSCALE_PAD, r(color), g(color), b(color), 255);
							drawPixel(SCREEN_TV, 1, xpos / DOWNSCALE_TV, ypos / DOWNSCALE_TV, r(color), g(color), b(color), 255);
						}else{
							drawLine(SCREEN_PAD, 1, lastxpos / DOWNSCALE_PAD, lastypos / DOWNSCALE_PAD, xpos / DOWNSCALE_PAD, ypos / DOWNSCALE_PAD, r(color), g(color), b(color), 255);
							drawLine(SCREEN_TV, 1, lastxpos / DOWNSCALE_TV, lastypos / DOWNSCALE_TV, xpos / DOWNSCALE_TV, ypos / DOWNSCALE_TV, r(color), g(color), b(color), 255);
						}
						flipBuffers(SCREEN_ALL);
					}

					lastxpos = xpos;
					lastypos = ypos;
				}
			}
		} else {
			// forget last touch position
			lastxpos = lastypos = -1;

			// process buttons except home only if there is no touch to avoid conflicts with touch color chooser

			if (vpad_data.btn_trigger & BUTTON_UP)
				color--;

			if (vpad_data.btn_trigger & BUTTON_DOWN)
				color++;

			if (vpad_data.btn_hold & BUTTON_PLUS)
				reset(color);
		}
		

		if(color != last_color) {
			// clamp color
			if(color < 0) color = 0;
			if(color > 4) color = 4;
			
			if(color != last_color) {
				last_color = color;
				drawColorChooserAndShow(color);
			}
		}

		if (vpad_data.btn_trigger & BUTTON_HOME) {
			break; //pls exit
		}
	}

	//WARNING: DO NOT CHANGE THIS. YOU MUST CLEAR THE FRAMEBUFFERS AND IMMEDIATELY CALL EXIT FROM THIS FUNCTION. RETURNING TO LOADER CAUSES FREEZE.
	for(i = 0; i < BUFFERS; i++) {
		fillScreen(SCREEN_ALL, 0, 0, 0, 0);
		flipBuffers(SCREEN_ALL);
	}
	_Exit();
}
