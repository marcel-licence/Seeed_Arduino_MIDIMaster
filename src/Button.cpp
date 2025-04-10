//
// Created by Administrator on 25-4-7.
//

#include "Button.h"

// 消抖时间（毫秒）
const unsigned long debounceDelay = 50;
// 长按时间（毫秒）
const unsigned long longPressTime = 600;

// 事件标志位
bool shortPressFlag_A = false;
bool longPressFlag_A = false;
bool releaseFlag_A = false;
bool shortPressFlag_B = false;
bool longPressFlag_B = false;
bool releaseFlag_B = false;
bool shortPressFlag_C = false;
bool longPressFlag_C = false;
bool releaseFlag_C = false;
bool shortPressFlag_D = false;
bool longPressFlag_D = false;
bool releaseFlag_D = false;

void initButtons(uint8_t pin)
{
    pinMode(pin,INPUT_PULLUP);
}

// 封装按键检测函数
void detectButtonEvents(uint8_t buttonPin, BtnState& button, bool& shortPressFlag, bool& longPressFlag, bool& releaseFlag) {
    // 读取按键状态
    int reading = digitalRead(buttonPin);

    // 检测按键状态是否改变
    if (reading != button.lastButtonState) {
        button.lastDebounceTime = millis();
    }

    // 消抖处理
    if ((millis() - button.lastDebounceTime) > debounceDelay) {
        if (reading != button.buttonState) {
            button.buttonState = reading;

            if (button.buttonState == LOW) {
                // 按键按下
                button.pressStartTime = millis();
                button.longPressTriggered = false;
            } else {
                // 按键释放
                unsigned long pressDuration = millis() - button.pressStartTime;
                if (!button.longPressTriggered && pressDuration < longPressTime) {
                    // 短按事件
                    shortPressFlag = true;
                }
                releaseFlag = true;
            }
        }
    }

    // 检测长按事件
    if (button.buttonState == LOW && (millis() - button.pressStartTime) >= longPressTime) {
        if (!button.longPressTriggered) {
            longPressFlag = true;
            button.longPressTriggered = true;
        }
    }

    // 更新上一次的按键状态
    button.lastButtonState = reading;
}