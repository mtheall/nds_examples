/* Keyboard + consoles demo
 *
 * This example demonstrates using a keyboard on the bottom screen and using a
 * console on both the top and bottom screens.
 */
#include <nds.h>
#include <stdio.h>

/* callback for when a keyboard key is pressed */
void onKeyPress(int key) {
  if(key > 0)
    /* print out the key on the selected console */
    fputc(key, stdout);
}

int main(int argc, char *argv[]) {
  PrintConsole top, bottom;
  Keyboard     kb;
  static char  input[1024];

  /* initialize video */
  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);

  /* initialize VRAM */
  vramSetPrimaryBanks(VRAM_A_MAIN_BG,
                      VRAM_B_MAIN_SPRITE,
                      VRAM_C_SUB_BG,
                      VRAM_D_SUB_SPRITE);

  /* by default the main screen is on top and the sub screen is on bottom */

  /* initialize main screen console
   * use map base 2 and tile base 0
   *   as indicated by:
   *   http://mtheall.com/vram.html#T0=2&NT0=128&MB0=2&TB0=0&S0=0
   */
  consoleInit(&top,    0, BgType_Text4bpp, BgSize_T_256x256,  2, 0, true,  true);

  /* initialize sub screen console and keyboard
   * use map base 14 and tile base 0 for keyboard
   * use map base 18 and tile base 2 for console
   *   as indicated by:
   *   http://mtheall.com/vram.html#T0=2&NT0=864&MB0=14&TB0=0&S0=2&T1=2&NT1=128&MB1=18&TB1=2&S1=0
   */
  consoleInit(&bottom, 1, BgType_Text4bpp, BgSize_T_256x256, 18, 2, false, true);
  keyboardInit(&kb,    0, BgType_Text4bpp, BgSize_T_256x512, 14, 0, false, true);

  /* set callback for when a key is pressed on the keyboard */
  kb.OnKeyPressed = onKeyPress;

  /* select the console we want our callback to print to */
  consoleSelect(&top);
  /* gather input from keyboard */
  scanf("%s", input);

  /* print out a message after we get input */
  iprintf("Top: You input '%s'\n", input);

  /* switch to the other console and print a similar message */
  consoleSelect(&bottom);
  iprintf("Bottom: You input '%s'\n\n", input);

  /* state how to exit */
  iprintf("Press B to exit\n");

  /* wait for exit condition */
  do {
    scanKeys();
  } while(!(keysDown() & KEY_B));

  /* we're done! */
  return 0;
}
