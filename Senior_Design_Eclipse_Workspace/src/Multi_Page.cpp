/*
 * Multi_Page.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: smith
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
File        : WIDGET_Multipage.c
Purpose     : Demonstrates the use of the MULTIPAGE widget
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include "main.h"
#include "GUI.h"
#include "MULTIPAGE.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 20)


extern Motor AzimuthalMotor;
extern Motor VerticalMotor;
extern Motor ClawMotor;


extern Encoder encoder;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       Dialog resource
*
*  These tables contain the information required to create the dialogs.
*  It has been created manually, but could also be created by the GUIBuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = {
{ WINDOW_CreateIndirect, "Motor Control", 0,         			0,  0, 480, 272, 0},
  { TEXT_CreateIndirect,     "Azimuthal:" ,  0,          		5,  35,  60,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Vertical:", 0,             		5,  85,  60,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Claw:",  0,                		5,  135, 60,  20, TEXT_CF_LEFT },
  { BUTTON_CreateIndirect,   "Azimuthal",   	GUI_ID_BUTTON0,   	110, 160,  80,  40 },
  { BUTTON_CreateIndirect,   "Vertical",    	GUI_ID_BUTTON1,    	200, 160,  80,  40 },
  { BUTTON_CreateIndirect,   "Claw",     	GUI_ID_BUTTON2,     	290, 160,  80,  40 },
  { BUTTON_CreateIndirect,   "Stop", 		GUI_ID_BUTTON3,  	380, 160,  80,  40 },
  { BUTTON_CreateIndirect,   "Forward",     	GUI_ID_BUTTON4,      	 10,  70,  80,  30 },
  { BUTTON_CreateIndirect,   "Reverse",     	GUI_ID_BUTTON5,      	 10, 110,  80,  30 },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT0,   			290,  80,  30,  20, 0, 3 },

};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
  { WINDOW_CreateIndirect,    "Dialog 2", 0,                   			0,   0, 480, 272, 0 },
  { BUTTON_CreateIndirect,   "ClockWise",   		GUI_ID_BUTTON0,   	100,  50,  80,  40 },
  { BUTTON_CreateIndirect,   "Counter-Clockwise",    	GUI_ID_BUTTON1,    	100, 100,  80,  40 },
  { BUTTON_CreateIndirect,   "Stop",     		GUI_ID_BUTTON2,     	100, 150,  80,  40 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate3[] = {
  { WINDOW_CreateIndirect,    "Dialog 3", 0,                   		0,   0, 480, 272, 0 },
  { BUTTON_CreateIndirect,   "Up",   		GUI_ID_BUTTON0,   	100,  50,  80,  40 },
  { BUTTON_CreateIndirect,   "Down",    	GUI_ID_BUTTON1,    	100, 100,  80,  40 },
  { BUTTON_CreateIndirect,   "Stop",     	GUI_ID_BUTTON2,     	100, 150,  80,  40 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate4[] = {
  { WINDOW_CreateIndirect,   "Dialog 4", 0,                       	0,   0, 480, 272, 0 },
  { BUTTON_CreateIndirect,   "Open",   		GUI_ID_BUTTON0,   	100,  50,  80,  40 },
  { BUTTON_CreateIndirect,   "Close",    	GUI_ID_BUTTON1,    	100, 100,  80,  40 },
  { BUTTON_CreateIndirect,   "Stop",     	GUI_ID_BUTTON2,     	100, 150,  80,  40 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate5[] = {
  { WINDOW_CreateIndirect,    "Dialog 5", 0,                   0,   0, 480, 272, 0 },
  { SLIDER_CreateIndirect,    "",         GUI_ID_SLIDER0,      5,  30, 200,  40  },
  { TEXT_CreateIndirect,      "Dialog 5", 0,                   5,  10,  50,  20, TEXT_CF_LEFT },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate6[] = {
  { WINDOW_CreateIndirect,    "Dialog 6", 0,                   0,   0, 480, 272, 0 },
  { SCROLLBAR_CreateIndirect, "",         GUI_ID_SCROLLBAR0,   5,  30, 200,  40  },
  { TEXT_CreateIndirect,      "Dialog 6", 0,                   5,  10,  50,  20, TEXT_CF_LEFT },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
WM_HWIN hDialog1;

/*********************************************************************
*
*       _SetEditValue
*/
/*static void _SetEditValue(int Id, float Value) {

  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hDialog1, Id);
  EDIT_SetFloatValue(hItem, Value);
}*/
/*********************************************************************
*
*       _cbDialog1
*/
/*static void _cbDialog1(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  WM_HWIN hDlg;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    hItem = WM_GetDialogItem(hDlg, GUI_ID_MULTIEDIT0);
    MULTIEDIT_SetText(hItem, "MULTIEDIT widget");
    MULTIEDIT_SetInsertMode(hItem, 1);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}*/
/*********************************************************************
*
*       _cbDialog1
*/
static void _cbDialog1(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  WM_HWIN hDlg;

  int Id, NCode;
  hDlg = pMsg->hWin;

  switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
      hItem = WM_GetDialogItem (hDlg, GUI_ID_EDIT0);
      EDIT_SetFloatMode(hItem, 0.0, -999.0, 999.0, 2, 0);
      break;
    case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);                    /* Id of widget */
    NCode = pMsg->Data.v;                         /* Notification code */
    switch (NCode){
    case WM_NOTIFICATION_RELEASED:
        if (Id == GUI_ID_BUTTON0) {       // Azimuthal Button
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 0) EncoderEnable[i] = true;

      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(0);				// Resets the count to 0 everytime
      	  VerticalMotor.dutyCycle(0);	// Ensures the other motors are off
      	  ClawMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON1) {        // Vertical Button
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 1) EncoderEnable[i] = true;
      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(VerticalCount);
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(100);
        }
        if (Id == GUI_ID_BUTTON2) {        // Claw Button
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 2) EncoderEnable[i] = true;
      		  else EncoderEnable[i] = false;
      	  }
//      	  encoder.set(ClawCount);
      	  encoder.set(0);
      	  AzimuthalMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON3) {        // Stop Button
      	  AzimuthalMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON4) {        // Forward Button
      	  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
        }
        if (Id == GUI_ID_BUTTON5) {        // Reverse Button
      	  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
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
*       _cbDialog2
*/
static void _cbDialog2(WM_MESSAGE * pMsg) {
  WM_HWIN hWinSrc;
  int Id, NCode;
  hWinSrc = pMsg->hWinSrc;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(hWinSrc);                    /* Id of widget */
    NCode = pMsg->Data.v;                         /* Notification code */
    switch (NCode){
    case WM_NOTIFICATION_RELEASED:
        if (Id == GUI_ID_BUTTON0) {       		// Up Button
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 0) EncoderEnable[i] = true;

      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(0);
      	  VerticalMotor.dutyCycle(0);	// Ensures the other motors are off
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON1) {        		// Down Button
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 0) EncoderEnable[i] = true;
      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(0);
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON2) {        		// Stop Button
      	  AzimuthalMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
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
*       _cbDialog3
*/
static void _cbDialog3(WM_MESSAGE * pMsg) {
  WM_HWIN hWinSrc;
  int Id, NCode;
  hWinSrc = pMsg->hWinSrc;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(hWinSrc);                    /* Id of widget */
    NCode = pMsg->Data.v;                         /* Notification code */
    switch (NCode){
    case WM_NOTIFICATION_RELEASED:
        if (Id == GUI_ID_BUTTON0) {       		// Open Button
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 1) EncoderEnable[i] = true;

      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(VerticalCount);
      	  VerticalMotor.dutyCycle(100);	// Ensures the other motors are off
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON1) {        		// Close Button
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 1) EncoderEnable[i] = true;
      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(VerticalCount);
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(100);
        }
        if (Id == GUI_ID_BUTTON2) {        		// Stop Button
      	  AzimuthalMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
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
*       _cbDialog4
*/
static void _cbDialog4(WM_MESSAGE * pMsg) {
  WM_HWIN hWinSrc;
  int Id, NCode;
  hWinSrc = pMsg->hWinSrc;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(hWinSrc);                    /* Id of widget */
    NCode = pMsg->Data.v;                         /* Notification code */
    switch (NCode){
    case WM_NOTIFICATION_RELEASED:
        if (Id == GUI_ID_BUTTON0) {       		// Open Button
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 2) EncoderEnable[i] = true;
      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(ClawCount);
      	  VerticalMotor.dutyCycle(0);	// Ensures the other motors are off
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(100);
        }
        if (Id == GUI_ID_BUTTON1) {        		// Close Button
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
      	  for (int8_t i = 0; i < 3; i++)
      	  {
      		  if (i == 2) EncoderEnable[i] = true;
      		  else EncoderEnable[i] = false;
      	  }
      	  encoder.set(ClawCount);
      	  AzimuthalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(100);
      	  VerticalMotor.dutyCycle(0);
        }
        if (Id == GUI_ID_BUTTON2) {        		// Stop Button
      	  AzimuthalMotor.dutyCycle(0);
      	  VerticalMotor.dutyCycle(0);
      	  ClawMotor.dutyCycle(0);
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
*       _cbBkWindow
*
*  Function description
*    Callback routine of the background window.
*    It shows the sample title and draws the background.
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
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
  WM_HWIN hMultiPage;
  WM_HWIN hFrameWin;
  WM_HWIN hDialog, hDialog1;

  //
  // Enable use of memory devices
  //
  WM_SetCreateFlags(WM_CF_MEMDEV);
  GUI_Init();
  //
  // Check if recommended memory for the sample is available
  //
  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
    GUI_ErrorOut("Not enough memory available.");
    return;
  }
  WM_SetCallback(WM_HBKWIN, _cbBkWindow);
  //
  // Create the frame window
  //
  hFrameWin = FRAMEWIN_Create("FrameWindow", NULL, WM_CF_SHOW, 0, 0, 480, 272);
  FRAMEWIN_SetClientColor(hFrameWin, GUI_GREEN);
  FRAMEWIN_SetActive(hFrameWin, 1);
  FRAMEWIN_SetMoveable(hFrameWin, 1);
  //
  // Create the MULTIPAGE widget
  //
  hMultiPage = MULTIPAGE_CreateEx(0, 0, 480, 252, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, 0);
  GUI_Delay(50);
  //
  // Create and attach the MULTIPAGE dialog windows
  //
  MULTIPAGE_SetTabHeight(hMultiPage, 40);

  hDialog1 = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), _cbDialog1, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog1, "	Debug	");
  GUI_Delay(50);
  hDialog = GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), _cbDialog2, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "	Azimuthal	");
  GUI_Delay(50);
  hDialog = GUI_CreateDialogBox(_aDialogCreate3, GUI_COUNTOF(_aDialogCreate3), _cbDialog3, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "	Vertical	");
  GUI_Delay(50);
  hDialog = GUI_CreateDialogBox(_aDialogCreate4, GUI_COUNTOF(_aDialogCreate4), _cbDialog4, WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "	Claw	");
  GUI_Delay(50);
  hDialog = GUI_CreateDialogBox(_aDialogCreate5, GUI_COUNTOF(_aDialogCreate5), NULL,       WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "	Page 5	");
  GUI_Delay(50);
  hDialog = GUI_CreateDialogBox(_aDialogCreate6, GUI_COUNTOF(_aDialogCreate6), NULL,       WM_UNATTACHED, 0, 0);
  MULTIPAGE_AddPage(hMultiPage, hDialog, "	About	");
  GUI_Delay(50);
  //
  // Demonstrate the use of MULTIPAGE_SetAlign
  //

  MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_LEFT | MULTIPAGE_ALIGN_BOTTOM);
  while (1) {
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/


