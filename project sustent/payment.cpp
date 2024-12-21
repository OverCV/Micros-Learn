// payment.cpp
#include "payment.h"

extern volatile bool coin100Inserted;
extern volatile bool coin200Inserted;
extern volatile bool coin500Inserted;
extern volatile bool coin1000Inserted;

const Payment::CoinInfo Payment::COIN_CATALOG[] = {
    {1, 100, '1', BTN_COIN_100, LED_COIN_100, coin100Inserted},
    {2, 200, '2', BTN_COIN_200, LED_COIN_200, coin200Inserted},
    {3, 500, '5', BTN_COIN_500, LED_COIN_500, coin500Inserted},
};

const uint8_t Payment::COIN_COUNT = sizeof(COIN_CATALOG) / sizeof(CoinInfo);

Payment::Payment() : _paid(0), _required(0), _lastCoinId(0), _needsReturn(false) {}

void Payment::init() {
    for (uint8_t i = 0; i < COIN_COUNT; i++) {
        pinMode(COIN_CATALOG[i].interruptPin, INPUT_PULLUP);
        pinMode(COIN_CATALOG[i].ledPin, OUTPUT);
        digitalWrite(COIN_CATALOG[i].ledPin, LOW);
    }
}

const Payment::CoinInfo* Payment::findCoinByKey(char key) const {
    for (uint8_t i = 0; i < COIN_COUNT; i++) {
        if (COIN_CATALOG[i].keypadKey == key) {
            return &COIN_CATALOG[i];
        }
    }
    return nullptr;
}

const Payment::CoinInfo* Payment::findCoinById(uint8_t id) const {
    for (uint8_t i = 0; i < COIN_COUNT; i++) {
        if (COIN_CATALOG[i].id == id) {
            return &COIN_CATALOG[i];
        }
    }
    return nullptr;
}

bool Payment::isValidCoin(uint8_t id) const {
    return findCoinById(id) != nullptr;
}

void Payment::checkInterrupts() {
    for (uint8_t i = 0; i < COIN_COUNT; i++) {
        if (COIN_CATALOG[i].flag) {
            insertCoinById(COIN_CATALOG[i].id);
            COIN_CATALOG[i].flag = false;
        }
    }
}

bool Payment::insertCoinByKey(char key) {
    const CoinInfo* coin = findCoinByKey(key);
    if (!coin) return false;
    return insertCoinById(coin->id);
}

bool Payment::insertCoinById(uint8_t id) {
    const CoinInfo* coin = findCoinById(id);
    if (!coin) return false;

    uint16_t newAmount = _paid + coin->value;
    Serial.print(F("Insertando moneda de valor: "));
    Serial.println(coin->value);
    Serial.print(F("Monto total propuesto: "));
    Serial.println(newAmount);

    if (newAmount > _required) {
        Serial.println(F("Error: Monto excede lo requerido"));
        _needsReturn = true;
        blinkCoinLED(coin);
        _needsReturn = false;
        return false;
    }

    _needsReturn = false;
    _paid = newAmount;
    _lastCoinId = coin->id;
    blinkCoinLED(coin);

    Serial.print(F("Monto actualizado: "));
    Serial.println(_paid);

    return true;
}

void Payment::blinkCoinLED(const CoinInfo* coin) {
    if (!coin) return;
    digitalWrite(coin->ledPin, HIGH);
    delay(500);
    digitalWrite(coin->ledPin, LOW);
}

void Payment::setRequired(uint16_t amount) {
    _required = amount;
    reset();
}

void Payment::previewRequired(uint16_t amount) {
    _required = amount;
    Serial.print(F("Vista previa del monto requerido: "));
    Serial.println(_required);
}

void Payment::reset() {
    _paid = 0;
    _needsReturn = false;
    _lastCoinId = 0;

    // Apagar todos los LEDs
    for (uint8_t i = 0; i < COIN_COUNT; i++) {
        digitalWrite(COIN_CATALOG[i].ledPin, LOW);
    }
}