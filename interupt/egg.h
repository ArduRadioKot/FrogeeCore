#ifndef EGG_H
#define EGG_H

#include <GyverOLED.h>

class DinoGame {
public:
    DinoGame(GyverOLED<SSD1306_128x64, OLED_BUFFER>& display, int jumpButtonPin)
        : oled(display), buttonPin(jumpButtonPin) {}

    void setup() {
        pinMode(buttonPin, INPUT_PULLUP);
        resetGame();
    }

    void loop() {
        unsigned long frameStart = millis();
        
        updateGame();
        
        if (checkCollision()) {
            gameOver();
            return;
        }

        drawGame();
        
        int frameTime = millis() - frameStart;
        if (frameTime < 60) {
            delay(60 - frameTime);
        }
    }

private:
    GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled;
    int buttonPin;

    // Спрайты 16x16
    const uint8_t dino[16][2] = {
        {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
        {0x03, 0xC0}, {0x07, 0xE0}, {0x07, 0xE0}, {0x07, 0xE0},
        {0x07, 0xC0}, {0x06, 0x00}, {0x0E, 0x00}, {0x1E, 0x00},
        {0x3E, 0x00}, {0x3E, 0x00}, {0x3C, 0x00}, {0x18, 0x00}
    };

    const uint8_t cactus[16][2] = {
        {0x00, 0x00}, {0x00, 0x00}, {0x00, 0xC0}, {0x00, 0xC0},
        {0x01, 0xE0}, {0x01, 0xE0}, {0x01, 0xE0}, {0x01, 0xE0},
        {0x01, 0xE0}, {0x01, 0xE0}, {0x01, 0xE0}, {0x01, 0xE0},
        {0x01, 0xE0}, {0x01, 0xE0}, {0x00, 0x00}, {0x00, 0x00}
    };

    // Параметры игры
    const int dinoX = 10;
    const int groundY = 48;
    const int jumpHeight = 48;  
    const int jumpDuration = 1000;  
    const int cactusSpeed = 5;  
    int cactusX = 128;
    bool isJumping = false;
    unsigned long jumpStartTime = 0;
    int score = 0;

    void updateGame() {
        cactusX -= cactusSpeed;
        if (cactusX < -16) {
            cactusX = 128 + random(20, 60);
            score++;
        }

        if (digitalRead(buttonPin) == LOW && !isJumping) {
            isJumping = true;
            jumpStartTime = millis();
        }

        if (isJumping && millis() - jumpStartTime > jumpDuration) {
            isJumping = false;
        }
    }

    void drawGame() {
        oled.clear();

        int dinoY = getDinoY();
        drawSprite(dinoX, dinoY, dino);
        drawSprite(cactusX, groundY, cactus);

        oled.setCursorXY(100, 0);
        oled.print(score);

        oled.update();
    }

    int getDinoY() {
        if (!isJumping) return groundY;

        float progress = (millis() - jumpStartTime) / (float)jumpDuration;
        return groundY - jumpHeight * sin(progress * PI);
    }

    bool checkCollision() {
        if (cactusX > dinoX + 12 || cactusX + 12 < dinoX) {
            return false;
        }

        int dinoY = getDinoY();
        int dinoBottom = dinoY + 16;
        int cactusTop = groundY;

        return (dinoBottom >= cactusTop);
    }

    void gameOver() {
        for (int i = 0; i < 3; i++) {
            oled.invertDisplay(true);
            delay(200);
            oled.invertDisplay(false);
            delay(200);
        }

        oled.clear();
        oled.setScale(2);
        oled.setCursorXY(20, 20);
        oled.print("SCORE: ");
        oled.print(score);
        oled.setCursorXY(20, 40);
        oled.print("GAME OVER");
        oled.update();

        while (digitalRead(buttonPin) == HIGH) delay(50);

        resetGame();
    }

    void resetGame() {
        cactusX = 128;
        isJumping = false;
        score = 0;
        oled.clear();
        drawGame();
        delay(500);
    }

    void drawSprite(int x, int y, const uint8_t sprite[16][2]) {
        for (int row = 0; row < 16; row++) {
            for (int col = 0; col < 16; col++) {
                int bytePos = col / 8;
                int bitPos = 7 - (col % 8);
                if (sprite[row][bytePos] & (1 << bitPos)) {
                    oled.dot(x + col, y + row);
                }
            }
        }
    }
};

#endif