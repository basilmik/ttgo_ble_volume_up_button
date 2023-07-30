#pragma once
#include "string.h"


#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 60

class button {
public:
    int value{ 0 };
    char* txt{ 0 };
    int16_t x, y;

    button(int16_t _x, int16_t _y, int _v, char* _txt = 0)
    {
        set_xy(_x, _y);
        set_value(_v);
        txt = _txt;
    }

    void set_xy(int16_t _x, int16_t _y)
    {
        x = _x;
        y = _y;
    }

    void set_value(int _v)
    {
        value = _v;
    }

    void draw(TFT_eSPI* tft)
    {
        tft->setCursor(x + BUTTON_WIDTH / 2 - 10, y + BUTTON_HEIGHT / 2 - 10);
        tft->setTextColor(TFT_WHITE);
        if (txt != NULL)
        {
            tft->setCursor(x + BUTTON_WIDTH / 2 - 30, y + BUTTON_HEIGHT / 2 - 10);
            tft->print(txt);
        }
        else
            tft->print(value);
    }

    bool does_belong(int _x, int _y)
    {
        return (_x >= x && _x <= x + BUTTON_WIDTH && _y >= y && _y <= y + BUTTON_HEIGHT);
    }

    int tap()
    {
        return value;
    }
};

class pinInput
{
public:

    button* btns[12];
    pinInput()
    {
        btns[1] = new button(BUTTON_WIDTH * 0, BUTTON_HEIGHT * 0, 1);
        btns[2] = new button(BUTTON_WIDTH * 1, BUTTON_HEIGHT * 0, 2);
        btns[3] = new button(BUTTON_WIDTH * 2, BUTTON_HEIGHT * 0, 3);

        btns[4] = new button(BUTTON_WIDTH * 0, BUTTON_HEIGHT * 1, 4);
        btns[5] = new button(BUTTON_WIDTH * 1, BUTTON_HEIGHT * 1, 5);
        btns[6] = new button(BUTTON_WIDTH * 2, BUTTON_HEIGHT * 1, 6);

        btns[7] = new button(BUTTON_WIDTH * 0, BUTTON_HEIGHT * 2, 7);
        btns[8] = new button(BUTTON_WIDTH * 1, BUTTON_HEIGHT * 2, 8);
        btns[9] = new button(BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2, 9);

        btns[10] = new button(BUTTON_WIDTH * 0, BUTTON_HEIGHT * 3, -1, "del");
        btns[0] = new button(BUTTON_WIDTH * 1, BUTTON_HEIGHT * 3, 0);
        btns[11] = new button(BUTTON_WIDTH * 2, BUTTON_HEIGHT * 3, -2, ">>");

    }

    ~pinInput()
    {
        for (int i = 0; i < 12; i++)
            delete btns[i];
    }

    void draw(TFT_eSPI* tft)
    {
        for (int i = 0; i < 12; i++)
            btns[i]->draw(tft);
    }

    int last_input = 0;
    int inputed_number = 0;

    int get_input(TTGOClass* ttgo)
    {
        int16_t x, y;
        Serial.println("input:");
        while (true)
        {
            if (ttgo->getTouch(x, y))
            {
                while (ttgo->getTouch(x, y))
                { // wait for user to release
                }

                for (int i = 0; i < 12; i++)
                {
                    if (btns[i]->does_belong(x, y))
                    {
                        switch (i)
                        {
                        case 10: inputed_number = inputed_number / 10;
                            break;

                        case 11:
                            last_input = inputed_number;
                            //Serial.println(last_input);
                            inputed_number = 0;
                            return last_input;
                            break;

                        default:
                            inputed_number = inputed_number * 10 + btns[i]->value;
                        }
                        break;
                    }
                }
            }
        }
    }

};
