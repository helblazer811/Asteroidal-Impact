#include "lib.h"

#define MAXBALLS 8

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	
	int buttondown[NBUTTONS] = {0};
	int button[NBUTTONS] = {0};

	int deltas[] = { -3, -2, -1, 1, 2, 3 };
        int ndeltas = sizeof(deltas) / sizeof(deltas[0]);
        unsigned short colors[] = {RED, BLUE, GREEN, WHITE, CYAN, YELLOW, MAGENTA};
        int ncolors = sizeof(colors) / sizeof(colors[0]);

	int score = 0;

        struct ball {
                int row;
                int col;
                int rd;
                int cd;
                unsigned short color;
        };

        struct state {
                int size;
		int nball;
                struct ball ball[MAXBALLS];
        } cs, ps; // current state & previous state

	// Initialize the state for the first round
	for (int i = 0; i < MAXBALLS; i++) {
		cs.ball[i].row = rand() % 160;
		cs.ball[i].col = rand() % 240;
		cs.ball[i].rd = deltas[rand() % ndeltas];;
		cs.ball[i].cd = deltas[rand() % ndeltas];;
		cs.ball[i].color = colors[i % ncolors];
	}
	cs.size = 5;
	cs.nball = MAXBALLS;
	
	while(1) { // Game Loop

		// Save current state
		ps = cs;

		// See what's changed and update current input state
                for (int i = 0; i < NBUTTONS; i++) {
			button[i] = KEY_DOWN_NOW(i) == 0 && buttondown[i];
			buttondown[i] = KEY_DOWN_NOW(i) != 0;
		}
		
		// Compute the graphics for the current state
		if(button[BUTTON_UP]) {
			cs.size++;
			if(cs.size>150) {
				cs.size = 150;
			}
		}
		if(button[BUTTON_DOWN]) {
			cs.size--;
			if(cs.size<3) {
				cs.size = 3;
			}
		}
		if (button[BUTTON_LEFT]) {
			if (cs.nball > 1)
				cs.nball--;
		}
		if (button[BUTTON_RIGHT]) {
			if (cs.nball < MAXBALLS)
				cs.nball++;
		}
		for (int i = 0; i < cs.nball; i++) {
			cs.ball[i].row = cs.ball[i].row + cs.ball[i].rd;
			cs.ball[i].col += cs.ball[i].cd;
			if(cs.ball[i].row < 0) {
				cs.ball[i].row = 0;
				cs.ball[i].rd = -cs.ball[i].rd;
			}
			if(cs.ball[i].row > 159-cs.size+1) {
				cs.ball[i].row = 159-cs.size+1;
				cs.ball[i].rd = -cs.ball[i].rd;
			}
			if(cs.ball[i].col < 0) {
				cs.ball[i].col = 0;
				cs.ball[i].cd = -cs.ball[i].cd;
				score++;
			}
			if(cs.ball[i].col > 239-cs.size+1) {
				cs.ball[i].col = 239-cs.size+1;
				cs.ball[i].cd = -cs.ball[i].cd;
			}
		}

		// Wait for Vblank
		waitForVblank();

		// Erase the previous objects
		for (int i = 0; i < ps.nball; i++) {
			drawRect(ps.ball[i].row, ps.ball[i].col, 
				ps.size, ps.size, BLACK);
		}
		drawRect(150, 5, 10, 80, BLACK);  // clear the score area

		// Draw the new objects
		for (int i = 0; i < cs.nball; i++) {
			drawRect(cs.ball[i].row, cs.ball[i].col, 
				cs.size, cs.size, cs.ball[i].color);

		}
		char buffer[51]; // draw the score
		sprintf(buffer, "Score: %d", score);
		drawString(150, 5, buffer, YELLOW);

	} // Game Loop


}
