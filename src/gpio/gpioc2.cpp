#include "gpioc2.h"
#include <fstream>
#include <sstream>

const std::set<int> GPIOC2::m_voltage_pins = {1, 2, 4, 17, 38};
const std::set<int> GPIOC2::m_ground_pins = {6, 9, 14, 20, 25, 30, 34, 39};
const std::map<int, int> GPIOC2::m_digital_pins = {{7, 249},  {11, 247}, {12, 238}, {13, 239}, 
                                                   {15, 237}, {16, 236}, {18, 233}, {19, 235}, 
                                                   {21, 232}, {22, 231}, {23, 230}, {24, 229},
                                                   {26, 225}, {29, 228}, {31, 219}, {32, 224}, 
                                                   {33, 234}, {35, 214}, {36, 218}};

GPIOC2::GPIOC2(int pin_number, bool direction)
: m_pin_number(pin_number),
  m_pin_direction(direction)
{
    /** Check validity of pin */
    if (pin_number < 1 || pin_number > 40) {
        std::stringstream message;
        message << "Invalid pin: " << pin_number << "\tPin of range.";
        printf("GPIO - ERROR message: %s", message.str().c_str());
    }

    /** Check that pin is of mode DIGITAL */
    auto it = m_digital_pins.find(pin_number);
    if (it == m_digital_pins.end()) {
        std::stringstream message;
        message << "Mode DIGITAL not supported for specified pin: "
                << pin_number;
        printf("GPIO - ERROR message: %s", message.str().c_str());
    }
    m_export_number = it->second;
    
    /** Pin and mode valid.  Now we must create file access for the pin and set direction */
    if (create_file_access() == false) {
        std::stringstream message;
        message << "Unable to create GPIO file access for pin " 
                << pin_number;
        printf("GPIO - ERROR message: %s", message.str().c_str());
    }

    if (set_direction() == false) {
        std::stringstream message;
        message << "Unable to set direction of pin " 
                << pin_number;
        printf("GPIO - ERROR message: %s", message.str().c_str());
    }

    std::string s = std::to_string(pin_number);
    std::string d = "";
    if (direction) d = "INPUT"; else d = "OUTPUT";
    printf("GPIO - %s PIN is instantiated as %s", s.c_str(), d.c_str());
}

GPIOC2::~GPIOC2() {}

bool GPIOC2::write_value(bool value)
{
    if (m_pin_direction != OUTPUT)
    {
        std::stringstream message;
        message << "Cannot write to pin " 
                << m_pin_number 
                << ".  Pin direction set to INPUT.";
        printf("GPIO - ERROR message: %s", message.str().c_str());
    }
    std::stringstream value_filename;
    value_filename << "/sys/class/gpio/gpio" 
                   << m_export_number 
                   << "/value";
    std::ofstream value_file(value_filename.str(), std::ios::out & std::ios::trunc);
    if (!value_file.is_open())
    {
        return false;
    }
    value_file << value;
    value_file.close();
    return true;
}

int GPIOC2::read_value()
{
    char value_char;
    std::stringstream value_filename;
    value_filename << "/sys/class/gpio/gpio"
                   << m_export_number
                   << "/value";
    std::ifstream value_file(value_filename.str());
    if (!value_file.is_open())
    {
        return -1;
    }
    value_file.get(value_char);
    return value_char - '0';
}

bool GPIOC2::create_file_access()
{
    std::string export_filename = "/sys/class/gpio/export";
    std::ofstream export_file(export_filename, std::ios::out & std::ios::trunc);
    if (!export_file.is_open())
    {
        return false;
    }
    export_file << m_export_number;
    export_file.close();
    return true;
}

bool GPIOC2::set_direction()
{
    std::stringstream direction_filename;
    direction_filename << "/sys/class/gpio/gpio" 
                       << m_export_number 
                       << "/direction";
    std::ofstream direction_file(direction_filename.str(), std::ios::out & std::ios::trunc);
    if (!direction_file.is_open())
    {
        return false;
    }
    if (m_pin_direction == INPUT)
    {
        direction_file << "in";
    }
    else // (direction == OUTPUT)
    {
        direction_file << "out";
    }
    direction_file.close();
    return true;
}
