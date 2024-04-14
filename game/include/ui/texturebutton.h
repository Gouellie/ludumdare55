#pragma once

const int BUTTON_STATE_COUNT = 5;

enum class TextureButtonState
{
    STATE_NORMAL    = 0,
    STATE_FOCUSED   = 1,
    STATE_PRESSED   = 2,
    STATE_DISABLED  = 3,
    STATE_SELECTED  = 4,
};

int TextureButton(Rectangle bounds, Texture2D& texture, bool flip, TextureButtonState state = TextureButtonState::STATE_NORMAL);
int TextureButtonWithMouseOver(Rectangle bounds, Texture2D& texture, bool flip, bool* mouseOver, TextureButtonState state = TextureButtonState::STATE_NORMAL);

#ifdef TEXTUREBUTTON_IMPLEMENTATION

int TextureButton(Rectangle bounds, Texture2D& texture, bool flip, TextureButtonState state)
{
    bool mouseOver = false;
    return TextureButtonWithMouseOver(bounds, texture, flip, &mouseOver, state);
}

int TextureButtonWithMouseOver(Rectangle bounds, Texture2D& texture, bool flip, bool* mouseOver, TextureButtonState state)
{
    int result = 0;

    float width = (float)texture.width / BUTTON_STATE_COUNT;

    // Update control
    //--------------------------------------------------------------------

    Vector2 mousePoint = GetMousePosition();

    // Check button state
    if (CheckCollisionPointRec(mousePoint, bounds))
    {
        (*mouseOver) = true;
        if ((state != TextureButtonState::STATE_DISABLED))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                state = TextureButtonState::STATE_PRESSED;
            else
                state = TextureButtonState::STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                result = 1;
        }
    }

    Rectangle source = { (float)state * width, 0.f, width, (float)texture.height };

    if (flip)
    {
        source.width *= -1;
    }

    DrawTexturePro(texture, source, bounds, { 0.f }, 0.f, WHITE);

    return result;
}

#endif