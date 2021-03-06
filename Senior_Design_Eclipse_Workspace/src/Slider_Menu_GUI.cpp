/*
 * Slider_Menu_GUI.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: SMART
 */

/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : DIALOG_SliderColor.c
Purpose     : Example demonstrating the use of a DIALOG widget
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "main.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 15)




extern Motor motor;
extern Encoder encoder;

/*
Encoder AzimuthalEncoder(Azimuthal_Encoder);
Encoder VerticalEncoder(Vertical_Encoder);
Encoder ClawEncoder(Claw_Encoder);
*/


/*********************************************************************
*
*       Static data, SeggerLogo
*
**********************************************************************
*/

/*********************************************************************
*
*       Static data, dialog resource
*
**********************************************************************
*/
/*********************************************************************
*
*       Dialog resource
*
* Function description
*   This table conatins the info required to create the dialog.
*   It has been created manually, but could also be created by a GUI-builder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Motor Control", 0,         		0,  0, 480, 272, 0},
  { TEXT_CreateIndirect,     "Position" ,  0,          			300,  20,  80,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Desired Position", 0,             300,  40,  80,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Duty Cycle",  0,                	300,  60,  80,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Encoder Dir" ,  0,          		300,  80,  80,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Motor Dir", 0,             		300, 100,  80,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Pos Error",  0,                	300, 120,  80,  20, TEXT_CF_LEFT },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT0,            410,  20,  45,  20, 0, 0 },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT1,            410,  40,  45,  20, 0, 3 },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT2,            410,  60,  45,  20, 0, 3 },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT3,            410,  80,  45,  20, 0, 3 },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT4,            410, 100,  45,  20, 0, 3 },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT5,            410, 120,  45,  20, 0, 3 },
  { EDIT_CreateIndirect,	 NULL, 	   GUI_ID_EDIT6,            410, 140,  45,  20, 0, 3 },
  { BUTTON_CreateIndirect,   "Forward",     GUI_ID_BUTTON4,      10, 170,  80,  30 },
  { BUTTON_CreateIndirect,   "Reverse",     GUI_ID_BUTTON5,      10, 210,  80,  30 },
  { BUTTON_CreateIndirect,   "Azimuthal",   GUI_ID_BUTTON0,   	110, 200,  80,  40 },
  { BUTTON_CreateIndirect,   "Vertical",    GUI_ID_BUTTON1,    	200, 200,  80,  40 },
  { BUTTON_CreateIndirect,   "Claw",     	GUI_ID_BUTTON2,     290, 200,  80,  40 },
  { BUTTON_CreateIndirect,   "Stop", 		GUI_ID_BUTTON3,  	380, 200,  80,  40 },
  { BUTTON_CreateIndirect,	"Increment",	GUI_ID_BUTTON6,		 10,  20,  80,  40 },
  { PROGBAR_CreateIndirect,   "",           GUI_ID_PROGBAR0,   	 90, 160, 100,  18},
  { SPINBOX_CreateIndirect,  NULL,          GUI_ID_SPINBOX0, 	130,  15,  80,  40, 0, 0, 0 },

};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

//
// Dialog handles
//
WM_HWIN _hDialogMain;

/*********************************************************************
*
*       _OnValueChanged
*/
/*static void _OnValueChanged(WM_HWIN hDlg, int Id) {
  unsigned Index;
  unsigned v;
  WM_HWIN  hSlider;
  WM_HWIN  hEdit;

  Index = 0;
  v     = 0;
  if ((Id >= GUI_ID_SLIDER0) && (Id <= GUI_ID_SLIDER1)) {
    Index = Id - GUI_ID_SLIDER0;
    //
    // SLIDER-widget has changed, update EDIT-widget
    //
    hSlider = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0 + Index);
    hEdit   = WM_GetDialogItem(hDlg, GUI_ID_EDIT0 + Index);
    v = SLIDER_GetValue(hSlider);
    EDIT_SetValue(hEdit, v);
  } else if ((Id >= GUI_ID_EDIT0) && (Id <= GUI_ID_EDIT1)) {
    Index = Id - GUI_ID_EDIT0;
    //
    // If EDIT-widget has changed, update SLIDER-widget
    //
    hSlider = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0 + Index);
    hEdit   = WM_GetDialogItem(hDlg, GUI_ID_EDIT0 + Index);
    v = EDIT_GetValue(hEdit);
    SLIDER_SetValue(hSlider, v);
  }
  _duty[Index] = v;
  //
  // At last invalidate dialog client window
  //
  WM_InvalidateWindow(WM_GetClientWindow(hDlg));
}*/

/*********************************************************************
*
*       _cbBkWindow
*/
/*static void _cbBkWindow(WM_MESSAGE * pMsg) {



  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);


  default:
    WM_DefaultProc(pMsg);
  }
}*/
/*********************************************************************
*
*       _SetProgbarValue
*/
static void _SetProgbarValue(int Id, I32 Value) {

  WM_HWIN hItem;
  hItem = WM_GetDialogItem(_hDialogMain, Id);
  PROGBAR_SetValue(hItem, Value);

}

/*********************************************************************
*
*       _cbCallback
*/
static void _cbCallback(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg;
  EDIT_Handle hEdit;
  WM_HWIN hItem;
  int     i;
  int     NCode;
  int     Id;




  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:


      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_SPINBOX0);
      SPINBOX_SetSkin(hItem, SPINBOX_SKIN_FLEX);
      hEdit = SPINBOX_GetEditHandle(hItem);
      EDIT_SetDecMode(hEdit, 16, 1, 100, 0, 0);


      SPINBOX_SetEditMode(hItem, SPINBOX_EM_EDIT);
      SPINBOX_SetRange(hItem, 0, 100);


      //
      // Init progress bars
      //
      hItem = WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);
      WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
      _SetProgbarValue(GUI_ID_PROGBAR0, 0);

      for (i = 3; i < 9; i++)
      {
		//
		// Init edit widgets
		//
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT3);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT4);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT5);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT6);
		EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
      }
      break;
    case WM_KEY:
      switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
        case GUI_KEY_ESCAPE:
          ;
          break;
        case GUI_KEY_ENTER:
          GUI_EndDialog(hDlg, 0);
          break;
      }
      break;
    case WM_NOTIFY_PARENT:

      Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
      NCode = pMsg->Data.v;                 // Notification code

      switch (NCode) {
        case WM_NOTIFICATION_RELEASED:      // React only if released
          if (Id == GUI_ID_OK) {            // OK Button
            GUI_EndDialog(hDlg, 0);
          }
          if (Id == GUI_ID_BUTTON3) {        // Stop Button
        	  motor.setDuty(Azimuthal_Motor, 0);
        	  motor.setDuty(Vertical_Motor, 0);
        	  motor.setDuty(Claw_Motor, 0);

/*        	  for (int8_t i = 0; i < 3; i++)
        	  {
        		  EncoderEnable[i] = false;
        	  }*/
          }
          if (Id == GUI_ID_BUTTON0) {       // Azimuthal Button
        	  for (int8_t i = 0; i < 3; i++)
        	  {
        		  if (i == 0) EncoderEnable[i] = true;

        		  else EncoderEnable[i] = false;
        	  }
        	  encoder.setCount(encoder.getCount());				// Resets the count to 0 everytime
          }
          if (Id == GUI_ID_BUTTON1) {        // Vertical Button
        	  for (int8_t i = 0; i < 3; i++)
        	  {
        		  if (i == 1) EncoderEnable[i] = true;
        		  else EncoderEnable[i] = false;
        	  }
        	  encoder.setCount(VerticalCount);
        	  DeltaVerticalCount = VerticalCount;
          }
          if (Id == GUI_ID_BUTTON2) {        // Claw Button

        	  for (int8_t i = 0; i < 3; i++)
        	  {
        		  if (i == 2) EncoderEnable[i] = true;
        		  else EncoderEnable[i] = false;
        	  }
        	  encoder.setCount(ClawCount);
          }
          if (Id == GUI_ID_BUTTON4) {        // Forward Button
        	  motor.setDirection(false);
          }
          if (Id == GUI_ID_BUTTON5) {        // Reverse Button
        	  motor.setDirection(true);
          }
          break;
        case WM_NOTIFICATION_VALUE_CHANGED: // Value has changed
          if (Id == GUI_ID_SPINBOX0) {
            Divisor = SPINBOX_GetValue(pMsg->hWinSrc);
            hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_SPINBOX0);
          }
          break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  WM_HWIN hDialogMain;
  WM_HWIN hItem;

  WM_SetCreateFlags(WM_CF_MEMDEV);  // Use memory devices on all windows to avoid flicker
  GUI_Init();
  //
  // Check if recommended memory for the sample is available
  //
  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
    GUI_ErrorOut("Not enough memory available.");
    return;
  }
  hDialogMain  = GUI_CreateDialogBox(_aDialogCreate,  GUI_COUNTOF(_aDialogCreate),  _cbCallback,  WM_HBKWIN, 0, 0);

/*
  WM_SetCallback(WM_HBKWIN, _cbBkWindow);

  GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
*/

  while (1) {
    GUI_Delay(10);

    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT0);
    EDIT_SetFloatValue(hItem, encoder.getPosition(Vertical_Encoder));
    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT1);
    EDIT_SetFloatValue(hItem, encoder.getDesiredPosition(Vertical_Encoder));
    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT2);
    EDIT_SetFloatValue(hItem, motor.getDuty(Vertical_Motor));
    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT3);
    EDIT_SetFloatValue(hItem, encoder.getDirection());
    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT4);
    EDIT_SetFloatValue(hItem, motor.getDirection());
    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT5);
    EDIT_SetFloatValue(hItem, encoder.getPosError(Vertical_Encoder));
    hItem = WM_GetDialogItem(hDialogMain, GUI_ID_EDIT6);
    EDIT_SetFloatValue(hItem, (encoder.getPosition(Vertical_Encoder) - encoder.getDesiredPosition(Vertical_Encoder)));


  }
}

/*************************** End of file ****************************/



