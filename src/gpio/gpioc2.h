/*
 * Copyright 2016 Jeremy Roy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file gpioc2.h
 * @brief Description of GPIOC2 class.
 * @author Jeremy Roy <jeremy.roy@queensu.ca>
 * @copyright (c) Jeremy Roy under the Apache License, version 2.0.
 */

#ifndef GPIOC2_H
#define GPIOC2_H

#include <set>
#include <map>

#define OUTPUT 0
#define INPUT 1

#define LOW 0
#define HIGH 1

enum pin_mode
{
    DIGITAL,
    I2C,
    PWM,
    UART,
    ADC
};

class GPIOC2 {
    public:
        GPIOC2(int pin_number, bool direction);
        ~GPIOC2();
        bool write_value(bool value);
        int read_value();
    private:
        int m_pin_number;
        bool m_pin_direction;
        int m_export_number;

        static const std::set<int> m_voltage_pins;
        static const std::set<int> m_ground_pins;
        static const std::map<int, int> m_digital_pins;

        bool create_file_access();
        bool set_direction();
};

#endif
