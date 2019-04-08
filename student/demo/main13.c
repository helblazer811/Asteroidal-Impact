#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r)) 
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31, 0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0

unsigned short *videoBuffer = (unsigned short *)0x6000000;

// Buttons

#define BUTTON_A		(0)
#define BUTTON_B		(1)
#define BUTTON_SELECT	(2)
#define BUTTON_START	(3)
#define BUTTON_RIGHT	(4)
#define BUTTON_LEFT		(5)
#define BUTTON_UP		(6)
#define BUTTON_DOWN		(7)
#define BUTTON_R		(8)
#define BUTTON_L		(9)
#define NBUTTONS		(10)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & (1<<key))

#define BUTTONS *(volatile unsigned int *)0x4000130

#define SCANLINECOUNTER *(volatile short *)0x4000006




// Prototypes
void setPixel(int , int , unsigned short );
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();
int rand();

#define MAXBALLS 8

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	
	int buttondown[NBUTTONS] = {0};
	int button[NBUTTONS] = {0};

	int deltas[] = { -3, -2, -1, 1, 2, 3 };
        int ndeltas = sizeof(deltas) / sizeof(deltas[0]);
        unsigned short colors[] = {RED, BLUE, GREEN, WHITE, CYAN, YELLOW, MAGENTA};
        int ncolors = sizeof(colors) / sizeof(colors[0]);

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

		// Draw the new objects
		for (int i = 0; i < cs.nball; i++) {
			drawRect(cs.ball[i].row, cs.ball[i].col, 
				cs.size, cs.size, cs.ball[i].color);
		}

	} // Game Loop


}
// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

// drawRect -- draw a rectangle on the screen
void drawRect(int row, int col, int height, int width, unsigned short color) {
        for (int r = 0; r < height; r++) {
                for (int c = 0; c < width; c++) {
                        setPixel(row + r, col + c, color);
                }
        }

}


// Delay for about n tenths of a second
void delay(int n) {
        volatile int x = 0;
        for (int i = 0; i < n * 8000; i++) {
                x++;
        }
}



void waitForVblank() {
        while(SCANLINECOUNTER > 160)
                ;
        while(SCANLINECOUNTER < 160)
                ;
}

