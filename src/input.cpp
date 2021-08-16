//#include "common.h"
#include "window.h"
#include "graphic.h"
#include "input.h"
//#include <math.h>

static short InputState[ 2 ][ INPUT_CODE_NUM ];
static int Now = 1;
//ジョイスティック中央付近で無効にする範囲
static unsigned InvalidRangeLow = 32768-10000;
static unsigned InvalidRangeHigh = 32768+10000;

float MouseX=0;
float MouseY=0;

void initInput(){
    for( int i = 0; i < INPUT_CODE_NUM; i++ ){
        InputState[ Now ][ i ] = 0;
        InputState[ !Now ][ i ] = 0;
    }
}

void getInputState(){
    
    Now = !Now;

    //キーボード
    InputState[ Now ][ KEY_A ] = GetAsyncKeyState( 'A' );
    InputState[ Now ][ KEY_B ] = GetAsyncKeyState( 'B' );
    InputState[ Now ][ KEY_C ] = GetAsyncKeyState( 'C' );
    InputState[ Now ][ KEY_D ] = GetAsyncKeyState( 'D' );
    InputState[ Now ][ KEY_E ] = GetAsyncKeyState( 'E' );
    InputState[ Now ][ KEY_F ] = GetAsyncKeyState( 'F' );
    InputState[ Now ][ KEY_G ] = GetAsyncKeyState( 'G' );
    InputState[ Now ][ KEY_H ] = GetAsyncKeyState( 'H' );
    InputState[ Now ][ KEY_I ] = GetAsyncKeyState( 'I' );
    InputState[ Now ][ KEY_J ] = GetAsyncKeyState( 'J' );
    InputState[ Now ][ KEY_K ] = GetAsyncKeyState( 'K' );
    InputState[ Now ][ KEY_L ] = GetAsyncKeyState( 'L' );
    InputState[ Now ][ KEY_M ] = GetAsyncKeyState( 'M' );
    InputState[ Now ][ KEY_N ] = GetAsyncKeyState( 'N' );
    InputState[ Now ][ KEY_O ] = GetAsyncKeyState( 'O' );
    InputState[ Now ][ KEY_P ] = GetAsyncKeyState( 'P' );
    InputState[ Now ][ KEY_Q ] = GetAsyncKeyState( 'Q' );
    InputState[ Now ][ KEY_R ] = GetAsyncKeyState( 'R' );
    InputState[ Now ][ KEY_S ] = GetAsyncKeyState( 'S' );
    InputState[ Now ][ KEY_T ] = GetAsyncKeyState( 'T' );
    InputState[ Now ][ KEY_U ] = GetAsyncKeyState( 'U' );
    InputState[ Now ][ KEY_V ] = GetAsyncKeyState( 'V' );
    InputState[ Now ][ KEY_W ] = GetAsyncKeyState( 'W' );
    InputState[ Now ][ KEY_X ] = GetAsyncKeyState( 'X' );
    InputState[ Now ][ KEY_Y ] = GetAsyncKeyState( 'Y' );
    InputState[ Now ][ KEY_Z ] = GetAsyncKeyState( 'Z' );
    InputState[ Now ][ KEY_LEFT ] = GetAsyncKeyState( VK_LEFT );
    InputState[ Now ][ KEY_RIGHT ] = GetAsyncKeyState( VK_RIGHT );
    InputState[ Now ][ KEY_UP ] = GetAsyncKeyState( VK_UP );
    InputState[ Now ][ KEY_DOWN ] = GetAsyncKeyState( VK_DOWN );
    InputState[ Now ][ KEY_SPACE ] = GetAsyncKeyState( VK_SPACE );
    InputState[ Now ][ KEY_ENTER ] = GetAsyncKeyState(VK_RETURN);
    InputState[ Now ][ KEY_ESCAPE ] = GetAsyncKeyState( VK_ESCAPE );
    InputState[ Now ][ KEY_SHIFT ] = GetAsyncKeyState( VK_SHIFT );

    //マウス
    // ボタン
    InputState[ Now ][ MOUSE_LBUTTON ] = GetAsyncKeyState( VK_LBUTTON );
    InputState[Now][MOUSE_MBUTTON] = GetAsyncKeyState(VK_MBUTTON);//MouseMButton;
    InputState[ Now ][ MOUSE_RBUTTON ] = GetAsyncKeyState( VK_RBUTTON );
    // カーソルの位置
    POINT mouse;
    GetCursorPos( &mouse );
    ScreenToClient( HWnd, &mouse );
    InputState[ Now ][ MOUSE_X ] = (short)mouse.x;
    InputState[ Now ][ MOUSE_Y ] = (short)mouse.y;
    // ウィンドウサイズ変更対応
    float aspect = Width / Height;
    if (ClientWidth == Width && ClientHeight == Height) {
        MouseX = (float)mouse.x;
        MouseY = (float)mouse.y;
    }
    else if ((float)ClientWidth / ClientHeight >= aspect) {
        float width = ClientHeight * aspect;
        float left = (ClientWidth - width) / 2.0f;
        MouseX = (mouse.x - left) / (float)ClientHeight * Height;
        MouseY = mouse.y / (float)ClientHeight * Height;
    }
    else {
        float height = ClientWidth / aspect;
        float top = (ClientHeight - height) / 2.0f;
        MouseX = mouse.x / (float)ClientWidth * Width;
        MouseY = (mouse.y - top) / (float)ClientWidth * Width;
    }

    // ホイール
    //InputState[ Now ][ MOUSE_WHEEL ] = MouseDelta;
    //MouseDelta = 0;

    //コントローラー
    getJoy();
    //getJoyXbox();
}
void getInput(){
    getInputState();
}

void getJoy(){
   	JOYINFOEX joy1;
	joy1.dwSize = sizeof( JOYINFOEX );
	joy1.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR;
	if(joyGetPosEx( JOYSTICKID1, &joy1 ) == JOYERR_NOERROR ){
        //ボタン
        for(int i = 0, button = JOY1_BUTTON1; i < 16; i++, button++){
            InputState[ Now ][ button ] = joy1.dwButtons & ( 1 << i );
        }

        //十字キー
        if(      (short)joy1.dwPOV == -1 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV == 0 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 1;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV == 4500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 1;
            InputState[ Now ][ JOY1_POV_UP ] = 1;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV ==  9000 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 1;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV ==  13500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 1;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 1;
        }
        else if( (short)joy1.dwPOV == 18000 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 1;
        }
        else if( (short)joy1.dwPOV == 22500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 1;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 1;
        }
        else if( (short)joy1.dwPOV == 27000 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 1;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV == 31500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 1;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 1;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }

        //スティック系
        DWORD axis[ 6 ] = { 
            joy1.dwXpos,
            joy1.dwYpos,
            joy1.dwZpos,
            joy1.dwRpos,
            joy1.dwUpos,
            joy1.dwVpos,
        };
        for( int i = 0; i < 6; i++ ){
            if( InvalidRangeLow <= axis[ i ] && axis[ i ] <= InvalidRangeHigh ){//未入力とする範囲
                InputState[ Now ][ JOY1_AXIS_A + i ] = 0;
            }
            else{
                InputState[ Now ][ JOY1_AXIS_A + i ] = (int)axis[ i ]*257/65536-128;
            }
        }

/*
        //スティックX
        if( InvalidRangeLow <= joy1.dwXpos && joy1.dwXpos <= InvalidRangeHigh ){//未入力とする範囲
            InputState[ Now ][ JOY1_AXIS_A ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_A ] = (int)joy1.dwXpos*257/65536-128;
        }
        //スティックY
        if( InvalidRangeLow <= joy1.dwYpos && joy1.dwYpos <= InvalidRangeHigh ){//未入力とする範囲
            InputState[ Now ][ JOY1_AXIS_B ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_B ] = -( (int)joy1.dwYpos*257/65536-128 );
        }
        //スティックZ
        if( InvalidRangeLow <= joy1.dwZpos && joy1.dwZpos <= InvalidRangeHigh ){//未入力とする範囲
            InputState[ Now ][ JOY1_AXIS_C ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_C ] = (int)joy1.dwZpos*257/65536-128;
        }
        //スティックR
        if( InvalidRangeLow <= joy1.dwRpos && joy1.dwRpos <= InvalidRangeHigh ){//未入力とする範囲
            InputState[ Now ][ JOY1_AXIS_D ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_D ] = -( (int)joy1.dwRpos*257/65536-128 );
        }
        //スティックU
        if( InvalidRangeLow <= joy1.dwUpos && joy1.dwUpos <= InvalidRangeHigh ){
            InputState[ Now ][ JOY1_AXIS_E ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_E ] = (int)joy1.dwUpos*257/65536-128;
        }
        //スティックV
        if( InvalidRangeLow <= joy1.dwVpos && joy1.dwVpos <= InvalidRangeHigh ){
            InputState[ Now ][ JOY1_AXIS_F ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_F ] = (int)joy1.dwVpos*257/65536-128;
        }
        */
    }
}

/*
void getJoyXbox(){
   	JOYINFOEX joy1;
	joy1.dwSize = sizeof( JOYINFOEX );
	joy1.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR;
	if(joyGetPosEx( JOYSTICKID1, &joy1 ) == JOYERR_NOERROR ){
        //ボタン
        for(int i = 0, button = JOY1_BUTTON1; i < 12; i++, button++){
            if( i <= 9 )
                InputState[ Now ][ button ] = joy1.dwButtons & ( 1 << i );
            else{
                //左右の引金は本来スティックと同じ役割。右(負）に倒すか左(正)に倒すかである
                //左右の引金を同時に引ききった時 v は 0 になる。つまり未入力と同じ。
                int v = (int)joy1.dwZpos*257/65536-128;
                //左引金
                if( i == 10 ){//BUTTON11
                    if( v > 0 )
                        InputState[ Now ][ button ] = 1;
                    else if( v == 0 )
                        InputState[ Now ][ button ] = 0;
                }
                //右引金
                if( i == 11 ){//BUTTON12
                    if( v < 0 )
                        InputState[ Now ][ button ] = 1;
                    else if( v == 0 )
                        InputState[ Now ][ button ] = 0;
                }
            }
        }
        //十字キー
        if( (short)joy1.dwPOV == -1 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV == 0 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 1;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV == 4500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 1;
            InputState[ Now ][ JOY1_POV_UP ] = 1;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV ==  9000 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 1;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV ==  13500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 1;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 1;
        }
        else if( (short)joy1.dwPOV == 18000 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 0;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 1;
        }
        else if( (short)joy1.dwPOV == 22500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 1;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 1;
        }
        else if( (short)joy1.dwPOV == 27000 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 1;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 0;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }
        else if( (short)joy1.dwPOV == 31500 ){
            InputState[ Now ][ JOY1_POV_LEFT ] = 1;
            InputState[ Now ][ JOY1_POV_RIGHT ] = 0;
            InputState[ Now ][ JOY1_POV_UP ] = 1;
            InputState[ Now ][ JOY1_POV_DOWN ] = 0;
        }

        //左スティック横
        if( InvalidRangeLow <= joy1.dwXpos && joy1.dwXpos <= InvalidRangeHigh ){
            InputState[ Now ][ JOY1_AXIS_A ] = 0;
            InputState[ Now ][ JOY1_AXIS_A_BTN2 ] = 0;
            InputState[ Now ][ JOY1_AXIS_A_BTN1 ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_A ] = (int)joy1.dwXpos*257/65536-128;
            if( InputState[ Now ][ JOY1_AXIS_A ] > 0 ){
                InputState[ Now ][ JOY1_AXIS_A_BTN2 ] = 0;
                InputState[ Now ][ JOY1_AXIS_A_BTN1 ] = 1;
            }
            else{
                InputState[ Now ][ JOY1_AXIS_A_BTN2 ] = 1;
                InputState[ Now ][ JOY1_AXIS_A_BTN1 ] = 0;
            }
        }
        //左スティック縦
        if( InvalidRangeLow <= joy1.dwYpos && joy1.dwYpos <= InvalidRangeHigh ){
            InputState[ Now ][ JOY1_AXIS_B ] = 0;
            InputState[ Now ][ JOY1_AXIS_B_BTN1 ] = 0;
            InputState[ Now ][ JOY1_AXIS_B_BTN2 ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_B ] = -( (int)joy1.dwYpos*257/65536-128 );
            if( InputState[ Now ][ JOY1_AXIS_B ] > 0 ){
                InputState[ Now ][ JOY1_AXIS_B_BTN1 ] = 1;
                InputState[ Now ][ JOY1_AXIS_B_BTN2 ] = 0;
            }
            else{
                InputState[ Now ][ JOY1_AXIS_B_BTN1 ] = 0;
                InputState[ Now ][ JOY1_AXIS_B_BTN2 ] = 1;
            }
        }
        //右スティック横
        if( InvalidRangeLow <= joy1.dwUpos && joy1.dwUpos <= InvalidRangeHigh ){
            InputState[ Now ][ JOY1_AXIS_C ] = 0;
            InputState[ Now ][ JOY1_AXIS_C_BTN2 ] = 0;
            InputState[ Now ][ JOY1_AXIS_C_BTN1 ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_C ] = (int)joy1.dwUpos*257/65536-128;
            if( InputState[ Now ][ JOY1_AXIS_C ] > 0 ){
                InputState[ Now ][ JOY1_AXIS_C_BTN2 ] = 0;
                InputState[ Now ][ JOY1_AXIS_C_BTN1 ] = 1;
            }
            else{
                InputState[ Now ][ JOY1_AXIS_C_BTN2 ] = 1;
                InputState[ Now ][ JOY1_AXIS_C_BTN1 ] = 0;
            }
        }
        //右スティック縦
        if( InvalidRangeLow <= joy1.dwRpos && joy1.dwRpos <= InvalidRangeHigh ){
            InputState[ Now ][ JOY1_AXIS_D ] = 0;
            InputState[ Now ][ JOY1_AXIS_D_BTN1 ] = 0;
            InputState[ Now ][ JOY1_AXIS_D_BTN2 ] = 0;
        }
        else{
            InputState[ Now ][ JOY1_AXIS_D ] = -( (int)joy1.dwRpos*257/65536-128 );
            if( InputState[ Now ][ JOY1_AXIS_D ] > 0 ){
                InputState[ Now ][ JOY1_AXIS_D_BTN1 ] = 1;
                InputState[ Now ][ JOY1_AXIS_D_BTN2 ] = 0;
            }
            else{
                InputState[ Now ][ JOY1_AXIS_D_BTN1 ] = 0;
                InputState[ Now ][ JOY1_AXIS_D_BTN2 ] = 1;
            }
        }
    }
}
*/


int inValue( enum INPUT_CODE inCode ){
    return InputState[ Now ][ inCode ];
}

int isPress( INPUT_CODE inCode ){
    return InputState[ Now ][ inCode ];
}

int isTrigger( INPUT_CODE inCode ){
    if( InputState[ !Now ][ inCode ] == 0 && InputState[ Now ][ inCode ] ){
        return 1;
    }
    return 0;
}
int isRelease(INPUT_CODE inCode) {
    if (InputState[!Now][inCode] && InputState[Now][inCode]==0) {
        return 1;
    }
    return 0;
}
extern const char* ButtonName[ MOUSE_X ]={
    "重複(未設定)",
    "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", 
    "Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ",
    "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ",
    "Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ",
    "Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ",
    "Ｚ", 
    "←", "→", "↑", "↓",
    "SPACE","ENTER",
    "MOUSE_L",
    "MOUSE_R",
    "JOY1_BUTTON1",  "JOY1_BUTTON2",   "JOY1_BUTTON3",  "JOY1_BUTTON4", 
    "JOY1_BUTTON5",  "JOY1_BUTTON6",   "JOY1_BUTTON7",  "JOY1_BUTTON8", 
    "JOY1_BUTTON9",  "JOY1_BUTTON10",  "JOY1_BUTTON11", "JOY1_BUTTON12",
    "JOY1_BUTTON13", "JOY1_BUTTON14",  "JOY1_BUTTON15", "JOY1_BUTTON16",
    "JOY1_POV_LEFT", "JOY1_POV_RIGHT", "JOY1_POV_UP", "JOY1_POV_DOWN",
    "JOY1_AXIS_A", "JOY1_AXIS_B", "JOY1_AXIS_C", "JOY1_AXIS_D", "JOY1_AXIS_E", "JOY1_AXIS_F",
};

INPUT_CODE getPressedKeyCode(){
    for( int i = (int)KEY_A; i <= (int)KEY_ENTER; i++ ){
        if( InputState[ Now ][ i ] != 0 ){
            return (INPUT_CODE)i;
        }
    }
    return (INPUT_CODE)0;
}

INPUT_CODE getTiltedJoyStickCode(){
    for( int i = (int)JOY1_AXIS_A; i <= (int)JOY1_AXIS_F; i++ ){
        if( InputState[ !Now ][ i ] != InputState[ Now ][ i ] ){
            return (INPUT_CODE)i;
        }
    }
    return (INPUT_CODE)0;
}

INPUT_CODE getPressedButtonCode(){
    for( int i = (int)JOY1_BUTTON1; i <= (int)JOY1_BUTTON16; i++ ){
        if( InputState[ Now ][ i ] != 0 ){
            return (INPUT_CODE)i;
        }
    }
    return (INPUT_CODE)0;
}
