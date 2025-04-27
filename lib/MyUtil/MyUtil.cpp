#include "MyUtil.h"
#include <M5Unified.h>

void MyUtil::print(String text, int textSize)
{
    M5.Display.setTextSize(textSize);
    M5.Display.println(text);
    Serial.println(text);
}