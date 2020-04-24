#pragma once

/**
 *  Pin handling made easy
 */

#include <Arduino.h>

namespace Kingswood
{
namespace Pin
{
class Pin
{
public:
    Pin(uint8_t pin) : _pin(pin),
                       _value(0)
    {
    }

    /**
             * Get the pin number
             * @return
             */
    uint8_t pin()
    {
        return _pin;
    }

    /**
             * Get pin value
             * @return
             */
    uint16_t value()
    {
        return _value;
    }

protected:
    uint8_t _pin;
    uint16_t _value;
};
} // namespace Pin
} // namespace Kingswood