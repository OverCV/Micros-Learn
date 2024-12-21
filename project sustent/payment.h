// payment.h
#ifndef PAYMENT_H
#define PAYMENT_H

#include <Arduino.h>
#include "pins.h"

class Payment {
public:
    struct CoinInfo {
        uint8_t id;
        uint16_t value;
        char keypadKey;
        uint8_t interruptPin;
        uint8_t ledPin;
        volatile bool& flag;
    };

private:
    static const uint8_t MAX_COINS = 10;
    static const CoinInfo COIN_CATALOG[];
    static const uint8_t COIN_COUNT;

    uint16_t _paid;
    uint16_t _required;
    uint8_t _lastCoinId;
    bool _needsReturn;

    // Métodos auxiliares privados
    const CoinInfo* findCoinByKey(char key) const;
    const CoinInfo* findCoinById(uint8_t id) const;
    void blinkCoinLED(const CoinInfo* coin);
    bool isValidCoin(uint8_t id) const;

public:
    Payment();
    
    void init();
    void checkInterrupts();
    bool insertCoinByKey(char key);
    bool insertCoinById(uint8_t id);
    void reset();
    void setRequired(uint16_t amount);
    void previewRequired(uint16_t amount);

    // Getters
    bool needsReturn() const { return _needsReturn; }
    bool isComplete() const { return _paid == _required; }
    uint16_t getCurrentAmount() const { return _paid; }
    uint16_t getRequiredAmount() const { return _required; }
    uint16_t getRemaining() const { return _required > _paid ? _required - _paid : 0; }
    uint8_t getLastCoinId() const { return _lastCoinId; }
};

#endif