#pragma once

#include <raylib.h>

void TextHelpersSetFont(Font font);
void DrawTextCentered(const char* text, Vector2 origin, int fontsize, Color fontColor);

bool DrawTextCenteredWithMouseCheck(const char* text, Vector2 origin, int fontsize, Color fontColor);

#ifdef TEXT_HELPERS_IMPLEMENTATION

Font helper_textFont = { 0 };

void TextHelpersSetFont(Font font)
{
    helper_textFont = font;
}

bool DrawTextCenteredWithMouseCheck(const char* text, Vector2 origin, int fontsize, Color fontColor)
{
    bool result = false;
    Vector2 textSize = MeasureTextEx(helper_textFont, text, fontsize, 2.f);
    float halfWidth = textSize.x / 2.f;
    float halfHeight = textSize.y / 2.f;

    Vector2 textPos = { origin.x - halfWidth, origin.y - halfHeight};
    if (CheckCollisionPointRec(GetMousePosition(), {origin.x - halfWidth, origin.y - halfHeight, textSize.x, textSize.y}))
    {
        result = true;
    }
    DrawTextPro(helper_textFont, text, textPos, { 0.f }, 0.f, fontsize, 2.f, fontColor);

    return result;
}

void DrawTextCentered(const char* text, Vector2 origin, int fontsize, Color fontColor) {
    Vector2 textSize = MeasureTextEx(helper_textFont, text, fontsize, 2.f);
    Vector2 textPos = { origin.x - textSize.x / 2.f, origin.y - textSize.y / 2.f };
    DrawTextPro(helper_textFont, text, textPos, { 0.f }, 0.f, fontsize, 2.f, fontColor);
}

#endif