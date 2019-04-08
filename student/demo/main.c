#include "lib.h"

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	int deltas[] = { -3, -2, -1, 1, 2, 3 };
	int ndeltas = sizeof(deltas) / sizeof(deltas[0]);
	int colors[] = { RED, BLUE, GREEN, WHITE, CYAN, YELLOW, MAGENTA };
	int ncolors = sizeof(colors) / sizeof(colors[0]);
	unsigned short bgcolor = GRAY;
	int score = 0;
	enum gamestate { GS_SPLASH, GS_INIT, GS_BOUNCE, GS_PAUSE, GS_QUIT };
	int buttondown[NBUTTONS] = {0};
	int button[NBUTTONS] = {0};

        struct ball {
                int row;
                int col;
                int rd;
                int cd;
                unsigned short color;
        };

	struct state {
		enum gamestate gamestate;
		int size;
		int nballs;
		int splashnballs;
		struct ball balls[MAXBALLS];
	} cs, ps;  // current and previous states


	// Initialize for the spash screen
	cs.gamestate = GS_SPLASH;
	cs.size = 5;
	cs.nballs = 0;
	cs.splashnballs = 1;
	for (int i = 0; i < MAXBALLS; i++) {
		cs.balls[i].row = 0;
		cs.balls[i].col = 80 + i * 2 * cs.size;
		cs.balls[i].rd = 1
		cs.balls[i].cd = 1
		cs.balls[i].color = colors[i % ncolors];
	}

	// clear the screen to bgcolor
	DMA[3].src = &bgcolor;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;

	while (cs.gamestate != GS_QUIT) { // Game Loop
		// Remember the previous state structure
		ps = cs;	

		// See what's changed and update the current input state
		for (int i = 0; i < NBUTTONS; i++) {
			button[i] = KEY_DOWN_NOW(i) == 0 && buttondown[i];
			buttondown[i] = KEY_DOWN_NOW(i) != 0;
		}

		// Compute the graphics for the current state
		switch (cs.gamestate) {

		// Balls are bouncing — act on buttons and draw new ball locations
		case GS_BOUNCE:
			// See if we need to do something with buttons
			if (button[BUTTON_UP]) {
				cs.size++;
				if(cs.size>150) {
					cs.size = 150;
				}
			}
			if (button[BUTTON_DOWN]) {
				cs.size--;
				if(cs.size<3) {
					cs.size = 3;
				}
			}
			if(button[BUTTON_RIGHT]) {
				if (cs.nballs > 1)
					cs.nballs--;
			}
			if(button[BUTTON_LEFT]) {
				if (cs.nballs < MAXBALLS)
					cs.nballs++;
			}
			// See if we need to bounce off a wall
			for (int i = 0; i < cs.nballs; i++) {
				struct ball *bp = &cs.balls[i];
				bp->row = bp->row + bp->rd;
				bp->col += bp->cd;
				if(bp->row < 0) {
					bp->row = 0;
					bp->rd = -bp->rd;
					score++;
				}
				if(bp->row > 159-cs.size+1) {
					bp->row = 159-cs.size+1;
					bp->rd = -bp->rd;
				}
				if(bp->col < 0) {
					bp->col = 0;
					bp->cd = -bp->cd;
				}
				if(bp->col > 239-cs.size+1) {
					bp->col = 239-cs.size+1;
					bp->cd = -bp->cd;
				}
			}
			break;

		// Animate the splash screen — drop the balls one at a time
		case GS_SPLASH:
			struct ball *bp = &cs.balls[cs.splashnballs - 1];
			// When we've reached the bottom, start the next ball
			if (bp->row >= 159 - cs.size + 1)
				// Start the next ball
				cs.splashnballs++;			else
				// Move the current ball down by 4
				bp->row += 4;	
			// Don't let the ball extend below the bottom
			if (bp->row > 159 - cs.size + 1)
				bp->row = 159 - cs.size + 1;
			// Draw only the balls we've touched up to this point
			if (cs.splashnballs <= MAXBALLS)
				cs.nballs = cs.splashnballs;

		/* debugging code
			char buffer[51];
			drawRect(150, 5, 8, 100, bgcolor);
			sprintf(buffer, "r,c=%d,%d,%d", bp->row, bp->col, cs.nballs);
			drawString(150, 5, buffer, YELLOW);
		*/
			break;

		case GS_INIT:
			// initialize the parameters for bouncing
			cs.nballs = MAXBALLS;
			for (int i = 0; i < cs.nballs; i++) {
				cs.balls[i].row = rand() % 160;
				cs.balls[i].col = rand() % 240;
				cs.balls[i].rd = deltas[rand() % ndeltas];
				cs.balls[i].cd = deltas[rand() % ndeltas];
				cs.balls[i].color = colors[i % ncolors];
			}
			break;

		case GS_PAUSE:
			// We’re paused; don’t do anything until we see an A button
			break;
		}

		// Wait for a Vblank to update the screen
		waitForVblank();

		// Erase the old images
		if (ps.gamestate != GS_PAUSE) {
			for (int i = 0; i < ps.nballs; i++) {
				struct ball *obp = &ps.balls[i];
				drawRect(obp->row, obp->col,
					ps.size, ps.size,
					bgcolor);
			}
			// If we’re bouncing, blank out the score
			if (cs.gamestate == GS_BOUNCE) {
				drawRect(150, 5, 10, 80, bgcolor);
			}
		}

		// Draw the new images
		if (cs.gamestate != GS_PAUSE) {
			for (int i = 0; i < cs.nballs; i++) {
				struct ball *bp = &cs.balls[i];
				drawRect(bp->row, bp->col,
					cs.size, cs.size,
					bp->color);
			}
			// If we’re bouncing, draw the score
			if (cs.gamestate == GS_BOUNCE) {
				char buffer[51];
				sprintf(buffer, "Score: %d", score);
				drawString(150, 5, buffer, YELLOW);
			}
		}
			
		// Choose the next state
		switch (cs.gamestate) {

		// Do the splash screen animation
		case GS_SPLASH: 
			if (cs.splashnballs > MAXBALLS)
				cs.gamestate = GS_INIT;
			break;

		// Initialize the balls for bouncing
		case GS_INIT: 
			cs.gamestate = GS_BOUNCE;
			break;

		// Bounce the balls around
		case GS_BOUNCE:
			if (button[BUTTON_A])
				cs.gamestate = GS_PAUSE;
			if (button[BUTTON_SELECT])
				cs.gamestate = GS_QUIT;
			break;

		// Pause the graphics
		case GS_PAUSE:
			if (button[BUTTON_A])
				cs.gamestate = GS_BOUNCE;
			if (button[BUTTON_SELECT])
				cs.gamestate = GS_QUIT;
			break;
			
		}


	} // Game Loop


}
