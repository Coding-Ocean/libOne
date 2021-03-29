#pragma once
enum INPUT_CODE {
    KEY_NONE,
    //ボタン系
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, 
    KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, 
    KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, 
    KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, 
    KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, 
    KEY_Z,
    KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
    KEY_SPACE, KEY_ENTER,
    MOUSE_LBUTTON,
    MOUSE_MBUTTON,
    MOUSE_RBUTTON,
    JOY1_BUTTON1,  JOY1_BUTTON2,  JOY1_BUTTON3,  JOY1_BUTTON4,
    JOY1_BUTTON5,  JOY1_BUTTON6,  JOY1_BUTTON7,  JOY1_BUTTON8,
    JOY1_BUTTON9,  JOY1_BUTTON10, JOY1_BUTTON11, JOY1_BUTTON12,
    JOY1_BUTTON13, JOY1_BUTTON14, JOY1_BUTTON15, JOY1_BUTTON16,
    JOY1_POV_LEFT, JOY1_POV_RIGHT, JOY1_POV_UP, JOY1_POV_DOWN,
    //スティック系
    JOY1_AXIS_A, JOY1_AXIS_B, JOY1_AXIS_C, JOY1_AXIS_D, JOY1_AXIS_E, JOY1_AXIS_F,
    MOUSE_X, MOUSE_Y, MOUSE_WHEEL,
    INPUT_CODE_NUM
};
void initInput();
void getInput();
void getInputState();
void getJoy();
void getJoyXbox();
int inValue( enum INPUT_CODE inCode );
int isPress( INPUT_CODE inCode );
int isTrigger( INPUT_CODE inCode );

extern const char* ButtonName[];
INPUT_CODE getPressedKeyCode();
INPUT_CODE getTiltedJoyStickCode();
INPUT_CODE getPressedButtonCode();
extern float MouseX;
extern float MouseY;
#define mouseX MouseX
#define mouseY MouseY

