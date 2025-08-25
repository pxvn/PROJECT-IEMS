#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Pins for ILI9341 (SPI wiring in Wokwi)
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// --- Color Theme (converted to 16-bit RGB565 for ILI9341) ---
#define BACKGROUND    0xDED0  
#define TEXT_COLOR    0x0320  
#define BUTTON_COLOR  0x07E0  
#define BUTTON_ACTIVE 0x0480 
#define CARD_COLOR    0xEF7D  
#define STATUS_COLOR  0x25B6  
#define TITLE_BG      0x0400  
#define HIGHLIGHT     0xFFFF  

// Screen dimensions
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240   // ILI9341 is 320x240 (not 320x480)

// Structure for button
struct Button {
    int x, y, w, h;
    const char* label;
    bool active;
};

Button buttons[] = {
    {20, 180, 280, 40, "Auto Mode", false}
};

// Function to draw card
void drawCard(int x, int y, const char* label, const char* value) {
    tft.fillRoundRect(x+2, y+2, 140, 60, 5, 0x0300);  // Shadow
    tft.fillRoundRect(x, y, 140, 60, 5, CARD_COLOR);
    tft.drawRoundRect(x, y, 140, 60, 5, STATUS_COLOR);

    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(1);
    tft.setCursor(x + 10, y + 8);
    tft.print(label);

    tft.setTextSize(2);
    tft.setCursor(x + 10, y + 35);
    tft.print(value);
}

// Function to draw button
void drawButton(Button& btn, bool pressed = false) {
    uint16_t color = pressed ? BUTTON_ACTIVE : BUTTON_COLOR;

    tft.fillRoundRect(btn.x+2, btn.y+2, btn.w, btn.h, 5, 0x0300); // Shadow
    tft.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 5, color);

    if (!pressed) {
        tft.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 5, HIGHLIGHT);
    }

    tft.setTextColor(pressed ? CARD_COLOR : TEXT_COLOR);
    tft.setTextSize(2);

    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(btn.label, 0, 0, &x1, &y1, &w, &h);
    int textX = btn.x + (btn.w - w) / 2;
    int textY = btn.y + (btn.h - h) / 2;

    tft.setCursor(textX, textY);
    tft.print(btn.label);
}

// Function to draw status bar
void drawStatusBar(const char* message) {
    tft.fillRect(0, 220, tft.width(), 20, STATUS_COLOR);
    tft.drawRect(0, 220, tft.width(), 20, BUTTON_ACTIVE);
    tft.setTextColor(CARD_COLOR);
    tft.setTextSize(2);
    tft.setCursor(10, 225);
    tft.print(message);
}

void setup() {
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(3);  // Rotate 180
    tft.fillScreen(BACKGROUND);

    // Title bar
    tft.fillRect(0, 0, SCREEN_WIDTH, 30, TITLE_BG);
    tft.drawFastHLine(0, 30, SCREEN_WIDTH, HIGHLIGHT);
    tft.setTextColor(CARD_COLOR);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("Energy Management System");

    // Info cards
    drawCard(20, 40, "Solar Gen", "0.2 kW");
    drawCard(165, 40, "Battery", "92%");
    drawCard(20, 110, "Usage", "---");
    drawCard(165, 110, "Temp", "---");

    // Status block
    tft.fillRoundRect(20, 150, 280, 25, 5, STATUS_COLOR);
    tft.drawRoundRect(20, 150, 280, 25, 5, BUTTON_ACTIVE);
    tft.setTextColor(CARD_COLOR);
    tft.setTextSize(2);
    tft.setCursor(30, 155);
    tft.print("Status: Normal");

    // Button
    drawButton(buttons[0]);

    // Status bar
    drawStatusBar("Touch to Control");
}

void loop() {
    // Wokwi can't simulate touch, so just blink button
    static unsigned long last = 0;
    if (millis() - last > 2000) {
        last = millis();
        drawButton(buttons[0], true);
        drawStatusBar("Auto Mode Selected");
        delay(500);
        drawButton(buttons[0], false);
        drawStatusBar("Touch to Control");
    }
}
