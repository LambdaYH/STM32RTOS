/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter (ST) for emWin V5.32.                      *
*        Compiled Oct  8 2015, 11:58:22                              *
*                                                                    *
*        (c) 1998 - 2015 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: Avatar_Invert                                         *
* Dimensions:  128 * 60                                              *
* NumColors:   2                                                     *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmAvatar_Invert;

#if 0


/*********************************************************************
*
*  This palette is included for reference only
*  As it is saved as device dependent bitmap without color info.
*  Please note that this bitmap requires the physical palette to be
*  identical to the palette of the display.
*  If this does not work out, please convert your bitmap into a DIB 
*/
/*********************************************************************
*
*       Palette
*
*  Description
*    The following are the entries of the palette table.
*    The entries are stored as a 32-bit values of which 24 bits are
*    actually used according to the following bit mask: 0xBBGGRR
*
*    The lower   8 bits represent the Red   component.
*    The middle  8 bits represent the Green component.
*    The highest 8 bits represent the Blue  component.
*/
static GUI_CONST_STORAGE GUI_COLOR _ColorsAvatar_Invert[] = {
  0xFFFFFF, 0x000000
};

#endif

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalAvatar_Invert = {
  2,  // Number of entries
  0,  // No transparency
  NULL
};

static GUI_CONST_STORAGE unsigned char _acAvatar_Invert[] = {
  ________, ________, ________, ________, ________, ________, ________, ______XX, _XXXXX__, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXX____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, XXXXXXXX, XXXXXXXX, XXXXXX__, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _______X, XXXXXXXX, XXXXXXXX, XXXXXXX_, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ______XX, XXXXXXXX, XXXXXXXX, XXXXXXXX, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, X_______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ____XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, X_XXXXXX, XXXX____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, _____XXX, XXXX____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, ______XX, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, ______XX, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, X______X, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XX_____X, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XX______, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXX__, _XX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXX_____, _XX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXX_____, _XX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXX_XXXX, XXXXXX__, _X______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXX_XXXX, XXXXX__X, XX______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, XX______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXX____, X_X_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, X_______, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ____XXXX, XXXXXXXX, XX_XX_XX, XXXXX___, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ____XXXX, XXXXXXXX, XXX_XX_X, XXXX____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ____XXXX, XXXXXXXX, XXX__XXX, XXXX___X, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXX___XX, XXXXXX__, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXX__X_X, XXXX____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXX_XX__, XXX_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXXXXXX_, XXX_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXXXXX__, _XX_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXXXXXX_, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _____XXX, XXXXXXXX, XXXXXXXX, X_______, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ______XX, XXXXXXXX, XXXXXXX_, XX______, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ______XX, XXXXXXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ______XX, XXXXXXXX, XXXXXXXX, XXX_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _______X, XXXXXXXX, XXXXXXX_, _XX_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, _______X, XXXXXXXX, XXXXXX__, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, XXXXXXXX, XXXXXXX_, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, _XXXXXXX, XXXXXXX_, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, _XXXXXXX, XXXXXX__, ________, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXX__, ___X____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ________, _XXXXXXX, XXXXXX__, __X_____, ________, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ______XX, XXXXXXXX, XXXXXXX_, _XX____X, XXX_____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, ____XXXX, XXXXXXXX, XXXXXXXX, XX_____X, XXXX____, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, ________, XXXXXXXX, XXXXXXXX, XXXXXXXX, ________, XXXXXXXX, XXX_____, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, _______X, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX_____X, XXXXXXXX, XXXXXX__, ________, ________, ________, ________,
  ________, ________, ________, ________, ________, _XXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX____X, XXXXXXXX, XXXXXXX_, ________, ________, ________, ________,
  ________, ________, ________, ________, _______X, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX____X, XXXXXXXX, XXXXXXXX, X_______, ________, ________, ________,
  ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_XXXX, XXXXXXXX, XXXXXXXX, XX______, ________, ________, ________,
  ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXX____, ________, ________, ________,
  ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________,
  ________, ________, ________, ________, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX_XXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, ________, ________, ________
};

GUI_CONST_STORAGE GUI_BITMAP bmAvatar_Invert = {
  128, // xSize
  60, // ySize
  16, // BytesPerLine
  1, // BitsPerPixel
  _acAvatar_Invert,  // Pointer to picture data (indices)
  &_PalAvatar_Invert   // Pointer to palette
};

/*************************** End of file ****************************/
