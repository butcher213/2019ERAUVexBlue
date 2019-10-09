#ifndef _BASH_HPP_
#define _BASH_HPP_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
#include "main.h"
#include "Button.hpp"


class Bash {
private:
    typedef struct btnData {
        // std::ostringstream info;
        std::string info;
        lv_obj_t *btn;
    } btnData_t;

    // The currently running task. Only Bash or Bash GUI can run at any time
    static pros::Task *bashTask;

    // Named sets of motors
    static std::map<std::string, std::vector<int>> motorSets;

    // Digital inputs from controller are mapped to a named set of motors
    static std::map<pros::controller_digital_e_t, std::string> controllerSetsDigital;

    // Analog inputs from controller are mapped to a named set of motors
    static std::map<pros::controller_analog_e_t, std::string> controllerSetsAnalog;

    static std::map<pros::controller_digital_e_t, std::string> btnToCode;
    static std::map<pros::controller_analog_e_t, std::string> joyToCode;


    // Bash objects shuld not be created. All operations are static
    Bash();
    ~Bash();

    // Take a string command and splits it with delimeter ' ' (space)
    static std::vector<std::string> splitCommand(std::string command);

    // Main logic loop for Bash that runs in a seperate task
    static void bashLoop(void *args);

    // Main logic loop got Bash GUI that runs in a seperate task
    static void guiLoop(void *args);

    // Bash GUI render loop for main menu
    static int renderMainMenu();

    // Bash GUI handler for when a motor button is pressed when mapping inputs
    //  to motors
    static void toggleMapMotor(std::vector<int> &ports, int port);
    // Bash GUI interface for setting digital inputs`when mapping inputs to
    //  motors
    static pros::controller_digital_e_t addBtnPressed();
    // Bash GUI interface for setting analog inputs`when mapping inputs to
    //  motors
    static pros::controller_analog_e_t addJoyPressed();
    // Bash GUI interface that draws buttons for mapping inputs and toggling
    //  motors in the motor set
    static void renderAddMapping();

    static void fillDigitalControllerInfo(std::vector<btnData_t> &mappingsData, lv_obj_t *mappingsList);
    static void fillAnalogControllerInfo(std::vector<btnData_t> &mappingsData, lv_obj_t *mappingsList);
    // Bash GUI interface that displays the current mappings
    static void renderShowMappings();

    // Bash GUI interface that temporarilly applies power to motor ports
    static void renderMoveMotors();

public:
    // Starts the Bash interface task
    static void init();

    // Starts the Bash GUI interface task
    static void initGUI();

    // Prints a list of Bash commands to std out
    static void printHelp();

    // updates motor sets' speeds if an input is recieved. This code must run
    //  for the inputs to manipulate motors
    static void updateMappedMotors();

    // maps the specified digital input to a name of a motor set
    static void mapControllerDigital(std::vector<std::string> &tokens);

    // maps the digital input to the motor set via a name consisting of the
    //  uint32 'id'
    static void mapDigitalToMotorSet(uint32_t id, pros::controller_digital_e_t btn, std::vector<int> ports);

    // maps the specified analog input to a name of a motor set
    static void mapControllerAnalog(std::vector<std::string> &tokens);

    // maps the analog input to the motor set via a name consisting of the
    //  uint32 'id'
    static void mapAnalogToMotorSet(uint32_t id, pros::controller_analog_e_t joy, std::vector<int> ports);

    // creates a motor set out of the designated ports and names the set;
    static void mapMotor(std::vector<std::string> &tokens);

    // move the motor at the specified port the specified speed
    static void moveMotor(std::vector<std::string> &tokens);

    // detects whether the string contains only digits
    static bool isDigits(std::string str);
};

#endif // _BASH_HPP_
