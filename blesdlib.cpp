#include "blesdlib.h"
#include "tabla_hid.hpp"

HijelHID_BLEKeyboard keyboard;
File myFile;
boolean first = true;
void Line(String l)
{
  int space_1 = l.indexOf(" ");
  if (space_1 == -1)
  {
    Press(l);
  }
  else if (l.substring(0,space_1) == "STRING")
  {
    keyboard.print(l.substring(space_1 + 1));
  }
  else if (l.substring(0,space_1) == "DELAY")
  {
    int delaytime = l.substring(space_1 + 1).toInt();
    delay(delaytime);
  }
  else if(l.substring(0,space_1) == "REM"){}
  else
  {
 String remain = l;
while(remain.length() > 0)
      {
        int latest_space = remain.indexOf(" ");
        if (latest_space == -1)
        {
          Press(remain);
          remain = "";
        }
        else
        {
          Press(remain.substring(0, latest_space));
          remain = remain.substring(latest_space + 1);
        }
        delay(5);
      }
  }

  keyboard.releaseAll();
}


void Press(String b)
{
  if(b.length() == 1)
  {
    char c = b[0];
    if (c >= 'a' && c <= 'z') keyboard.press(tablaHID[c - 'a']);
  }
  else if (b.equals("ENTER"))
  {
    keyboard.press(KEY_RETURN);
  }
  else if (b.equals("CTRL"))
  {
    keyboard.press(KEY_LCTRL);
  }
    else if (b.equals("SHIFT"))
  {
    keyboard.press(KEY_LSHIFT);
  }
    else if (b.equals("ALT"))
  {
    keyboard.press(KEY_LALT);
  }
    else if (b.equals("GUI"))
  {
    keyboard.press(KEY_LGUI);
  }
    else if (b.equals("UP") || b.equals("UPARROW"))
  {
    keyboard.press(KEY_UP);
  }
    else if (b.equals("DOWN") || b.equals("DOWNARROW"))
  {
    keyboard.press(KEY_DOWN);
  }
    else if (b.equals("LEFT") || b.equals("LEFTARROW"))
  {
    keyboard.press(KEY_LEFT);
  }
    else if (b.equals("RIGHT") || b.equals("RIGHTARROW"))
  {
    keyboard.press(KEY_RIGHT);
  }
    else if (b.equals("DELETE"))
  {
    keyboard.press(KEY_DELETE);
  }
    else if (b.equals("PAGEUP"))
  {
    keyboard.press(KEY_PAGE_UP);
  }
    else if (b.equals("PAGEDOWN"))
  {
    keyboard.press(KEY_PAGE_DOWN);
  }
    else if (b.equals("HOME"))
  {
    keyboard.press(KEY_HOME);
  }
    else if (b.equals("ESC"))
  {
    keyboard.press(KEY_ESCAPE);
  }
    else if (b.equals("INSERT"))
  {
    keyboard.press(KEY_INSERT);
  }
    else if (b.equals("TAB"))
  {
    keyboard.press(KEY_TAB);
  }
    else if (b.equals("END"))
  {
    keyboard.press(KEY_END);
  }
    else if (b.equals("CAPSLOCK"))
  {
    keyboard.press(KEY_CAPS_LOCK);
  }
    else if (b.equals("F1"))
  {
    keyboard.press(KEY_F1);
  }
    else if (b.equals("F2"))
  {
    keyboard.press(KEY_F2);
  }
    else if (b.equals("F3"))
  {
    keyboard.press(KEY_F3);
  }
    else if (b.equals("F4"))
  {
    keyboard.press(KEY_F4);
  }
    else if (b.equals("F5"))
  {
    keyboard.press(KEY_F5);
  }
    else if (b.equals("F6"))
  {
    keyboard.press(KEY_F6);
  }
    else if (b.equals("F7"))
  {
    keyboard.press(KEY_F7);
  }
    else if (b.equals("F8"))
  {
    keyboard.press(KEY_F8);
  }
    else if (b.equals("F9"))
  {
    keyboard.press(KEY_F9);
  }
    else if (b.equals("F10"))
  {
    keyboard.press(KEY_F10);
  }
    else if (b.equals("F11"))
  {
    keyboard.press(KEY_F11);
  }
    else if (b.equals("F12"))
  {
    keyboard.press(KEY_F12);
  }
    else if (b.equals("SPACE"))
  {
    keyboard.press(KEY_SPACE);
  } 
}
void attack() {
if (myFile) {
    keyboard.begin();
String line = "";
    while (myFile.available()) {
      char m = myFile.read();
      if (m == '\n'){
        Line(line);
        line = "";
        }
        else if((int) m != 13)
        {
          line += m;
        }
    }
    Line(line);
    
    myFile.close();
  } else {
  }

  keyboard.end();
}
