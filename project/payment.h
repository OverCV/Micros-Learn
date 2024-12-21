#ifndef PAYMENT_H
#define PAYMENT_H

#include <Arduino.h>

class Payment {
private:
  static const uint8_t NUM_COINS = 3;
  const uint16_t COINS[NUM_COINS] = { 100, 200, 500 };

  uint16_t _paid;
  uint16_t _required;
  uint8_t _lastCoin;
  bool _needsReturn;

  // Referencias a las flags de monedas
  volatile bool& _coin100;
  volatile bool& _coin200;
  volatile bool& _coin500;

  // Pines de LEDs asociados a las monedas
  const uint8_t _ledCoin100;
  const uint8_t _ledCoin200;
  const uint8_t _ledCoin500;

  bool isValidCoin(uint8_t index) const;

public:
  Payment(volatile bool& coin100, volatile bool& coin200, volatile bool& coin500,
          uint8_t ledCoin100, uint8_t ledCoin200, uint8_t ledCoin500);

  void blinkCoinLED(uint8_t coinIndex);
  void checkCoins();
  bool insertCoin(uint8_t coinIndex);
  bool needsReturn() const {
    return _needsReturn;
  }
  bool isComplete() const {
    return _paid == _required;
  }

  // Getters
  uint16_t getCurrentAmount() const {
    return _paid;
  }
  uint16_t getRequiredAmount() const {
    return _required;
  }
  void previewRequired(uint16_t amount);
  uint16_t getRemaining() const {
    return _required > _paid ? _required - _paid : 0;
  }
  uint16_t getCoinValue(uint8_t index) const {
    return isValidCoin(index) ? COINS[index] : 0;
  }
  uint8_t getLastCoin() const {
    return _lastCoin;
  }

  void setRequired(uint16_t amount);
  void reset();
};

#endif
