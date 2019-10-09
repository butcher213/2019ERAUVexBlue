#include "Bash.hpp"

#if USE_LV_KB == 0
#error "MUST CHANGE include/display/lv_config.h\n\
       #define USE_LV_KB 0\n\
       #define USE_LV_KB 1"
#endif // USE_LV_KB == 0


pros::Task *Bash::bashTask;

std::map<std::string, std::vector<int>> Bash::motorSets;

std::map<pros::controller_digital_e_t, std::string> Bash::controllerSetsDigital;

std::map<pros::controller_analog_e_t, std::string> Bash::controllerSetsAnalog;

std::map<pros::controller_digital_e_t, std::string> Bash::btnToCode = {
        {pros::E_CONTROLLER_DIGITAL_L1, "L1"},
        {pros::E_CONTROLLER_DIGITAL_L2, "L2"},
        {pros::E_CONTROLLER_DIGITAL_R1, "R1"},
        {pros::E_CONTROLLER_DIGITAL_R2, "R2"},
        {pros::E_CONTROLLER_DIGITAL_UP, "U"},
        {pros::E_CONTROLLER_DIGITAL_DOWN, "D"},
        {pros::E_CONTROLLER_DIGITAL_LEFT, "L"},
        {pros::E_CONTROLLER_DIGITAL_RIGHT, "R"},
        {pros::E_CONTROLLER_DIGITAL_X, "X"},
        {pros::E_CONTROLLER_DIGITAL_B, "B"},
        {pros::E_CONTROLLER_DIGITAL_Y, "Y"},
        {pros::E_CONTROLLER_DIGITAL_A, "A"}
};

std::map<pros::controller_analog_e_t, std::string> Bash::joyToCode = {
        {pros::E_CONTROLLER_ANALOG_LEFT_X, "LX"},
        {pros::E_CONTROLLER_ANALOG_LEFT_Y, "LY"},
        {pros::E_CONTROLLER_ANALOG_RIGHT_X, "RX"},
        {pros::E_CONTROLLER_ANALOG_RIGHT_Y, "RY"}
};


Bash::Bash() {}

Bash::~Bash() {
    delete bashTask;
}


void Bash::bashLoop(void *args) {
    std::cout << "Bash cortex controller is running!" << std::endl;

    while (1) {
        std::string command;
        getline(std::cin, command);

        if (command.size() > 0) {
            std::vector<std::string> tokens = splitCommand(command);

            if (tokens[0].compare("map")) {
                if (tokens[1].compare("motor"))
                    mapMotor(tokens);
                else if (tokens[1].compare("digital"))
                    mapControllerDigital(tokens);
                else if (tokens[1].compare("analog"))
                    mapControllerAnalog(tokens);
                else if (tokens[1].compare("help") || tokens[1].compare("?"))
                    std::cout << "map <motor, digital, analog> <args...>" << std::endl;
                else
                    std::cout << "Error: Token is not recognised: " << tokens[1] << std::endl;
                    std::cout << "    Try typing 'map ?'" << std::endl;
            }
            else if (tokens[0].compare("move")) {
                moveMotor(tokens);
            }
            else if (tokens[0].compare("help") || tokens[0].compare("?")) {
                printHelp();
            }
            else {
                std::cout << "Error: Not a valid command: " << command << std::endl;
                std::cout << "   Try typing '?'" << std::endl;
            }
        }
    }
}

void Bash::guiLoop(void *args) {
    int menuSelection = 0; // switch between windows (like add mapping and main menu)
    std::cout << "Bash GUI cortex controller is running!" << std::endl;

    // render GUI BG
    while (1) {
        int menuSelection = renderMainMenu();

        switch (menuSelection) {
            case 0: // add mapping
                renderAddMapping();
                break;
            case 1: // show current mappings
                renderShowMappings();
                // mappings should be in collapsable "staircase" view with buttons to edit (with keypad) and remove
                break;
            case 2: // move motors
                renderMoveMotors();
                // add buttons for each motor that opens keypad input
                break;
        }
    }
}


void Bash::init() {
    if (bashTask) {
        bashTask->remove();
    }

    bashTask = new pros::Task(bashLoop, NULL, TASK_PRIORITY_DEFAULT,
                    TASK_STACK_DEPTH_DEFAULT, "BASH");

    std::cout << "Bash cortex controller has been initialized!" << std::endl;
}

void Bash::initGUI() {
    if (pros::lcd::is_initialized()) {
        pros::lcd::shutdown();
        std::cout << "Bash GUI disabled LLEMU." << std::endl;
    }
    if (bashTask) {
        bashTask->remove();
    }

    bashTask = new pros::Task(guiLoop, NULL, TASK_PRIORITY_DEFAULT,
                    TASK_STACK_DEPTH_DEFAULT, "BASH GUI");

    std::cout << "Bash GUI cortex controller has been initialized!" << std::endl;
}


int Bash::renderMainMenu() {
    int newState = -1;
    const char *btnNames[] = {
        "Add Mapping",
        "Show Mappings",
        "Set Motors",
    };
    static const int btnNameCount = sizeof(btnNames) / sizeof(btnNames[0]);

    lv_obj_t *menuList = lv_list_create(lv_scr_act(), NULL);
    lv_obj_align(menuList, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(menuList, LV_HOR_RES / 4, LV_VER_RES / 2);

    lv_obj_t *btns[btnNameCount];
    for (int index = 0; index < btnNameCount; index++)
        btns[index] = lv_list_add(menuList, NULL, btnNames[index], NULL);


    while (newState == -1) {
        for (int index = 0; index < btnNameCount; index++) {
            if (lv_btn_get_state(btns[index]) == LV_BTN_STATE_PR) {
                newState = index;
            }
        }

        Bash::updateMappedMotors();
    }

    for (int index = 0; index < btnNameCount; index++)
        lv_obj_del(btns[index]);
    lv_obj_del(menuList);

    return newState;
}

void Bash::toggleMapMotor(std::vector<int> &ports, int port) {
    std::vector<int>::iterator portItr = std::find(ports.begin(), ports.end(), port);

    if (portItr != ports.end()) {
        ports.push_back(port);
    }
    else {
        ports.erase(portItr);
    }
}
pros::controller_digital_e_t Bash::addBtnPressed() {
    static const pros::controller_digital_e_t btns[] = {
            pros::E_CONTROLLER_DIGITAL_L1,
            pros::E_CONTROLLER_DIGITAL_L2,
            pros::E_CONTROLLER_DIGITAL_R1,
            pros::E_CONTROLLER_DIGITAL_R2,
            pros::E_CONTROLLER_DIGITAL_UP,
            pros::E_CONTROLLER_DIGITAL_DOWN,
            pros::E_CONTROLLER_DIGITAL_LEFT,
            pros::E_CONTROLLER_DIGITAL_RIGHT,
            pros::E_CONTROLLER_DIGITAL_X,
            pros::E_CONTROLLER_DIGITAL_B,
            pros::E_CONTROLLER_DIGITAL_Y,
            pros::E_CONTROLLER_DIGITAL_A
    };
    static pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::controller_digital_e_t selectedBtn;
    bool btnSelected = false;


    lv_obj_t *selectBtnLabel = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(selectBtnLabel, "Press button to map");
    lv_obj_align(selectBtnLabel, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    Button backBtn("BACK", 0, 0, LV_HOR_RES / 10, LV_VER_RES / 10);

    while (!btnSelected && backBtn.getState() != LV_BTN_STATE_PR) {
        for (int btnIndex = 0; btnIndex < sizeof(btns) / sizeof(btns[0]); btnIndex++) {
            if (master.get_digital(btns[btnIndex])) {
                selectedBtn = btns[btnIndex];
                btnSelected = true;
            }
        }
    }

    lv_obj_del(selectBtnLabel);

    return selectedBtn;
}
pros::controller_analog_e_t Bash::addJoyPressed() {
    static const pros::controller_analog_e_t joys[] = {
        pros::E_CONTROLLER_ANALOG_LEFT_X,
        pros::E_CONTROLLER_ANALOG_LEFT_Y,
        pros::E_CONTROLLER_ANALOG_RIGHT_X,
        pros::E_CONTROLLER_ANALOG_RIGHT_Y
    };
    static pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::controller_analog_e_t selectedJoy;
    bool joySelected = false;


    lv_obj_t *selectJoyLabel = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(selectJoyLabel, "Move joystick to map");
    lv_obj_align(selectJoyLabel, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    Button backBtn("BACK", 0, 0, LV_HOR_RES / 10, LV_VER_RES / 10);


    while (!joySelected && backBtn.getState() != LV_BTN_STATE_PR) {
        for (int joyIndex = 0; joyIndex < sizeof(joys) / sizeof(joys[0]); joyIndex++) {
            if (master.get_analog(joys[joyIndex]) > 100) {
                selectedJoy = joys[joyIndex];
                joySelected = true;
            }
        }
    }


    lv_obj_del(selectJoyLabel);


    return selectedJoy;
}
void Bash::renderAddMapping() {
    static const int MAP_TO_NONE = -1,
                     MAP_TO_DGTL = 0,
                     MAP_TO_ANLG = 1;
    static const char *btns[] = {
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                                    "\n",
                "Add Button", "Add Joystick", "Reset Map",
                                    "\n",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        ""
    };
    static uint32_t mapID = 0;
    std::vector<int> ports;
    int mapTo = MAP_TO_NONE;
    pros::controller_analog_e_t selectedJoy;
    pros::controller_digital_e_t selectedBtn;


    lv_obj_t *btnsMap = lv_btnm_create(lv_scr_act(), NULL);
    lv_btnm_set_map(btnsMap, btns);
    lv_obj_align(btnsMap, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    Button backBtn("BACK", 0, 0, LV_HOR_RES / 10, LV_VER_RES / 10);

    while (backBtn.getState() != LV_BTN_STATE_PR) {
        int id = lv_btnm_get_pressed(btnsMap);

        if (id != LV_BTNM_PR_NONE) {
            lv_obj_set_hidden(backBtn.btn, true);
            lv_obj_set_hidden(btnsMap, true);

            if (id < 10) { // ports 1-10
                int port = id;
                toggleMapMotor(ports, port);
            }
            else if (id == 10) { // ADD Button was pushed - add digital
                selectedBtn = addBtnPressed();
                mapTo = MAP_TO_DGTL;
            }
            else if (id == 11) { // ADD Joystick was pressed - add analog
                selectedJoy = addJoyPressed();
                mapTo = MAP_TO_ANLG;
            }
            else if (id == 12) { // Reset Map
                mapTo = MAP_TO_NONE;
            }
            else { // ports 11-20
                int port = id + 12;
                toggleMapMotor(ports, port);
            }

            lv_obj_set_hidden(backBtn.btn, false);
            lv_obj_set_hidden(btnsMap, false);
        }
    }

    lv_obj_t *message = lv_label_create(lv_scr_act(), NULL);
    lv_obj_align(message, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
    if (mapTo == MAP_TO_NONE) {
        lv_label_set_text(message, "MAPPING CANCELED");
    }
    else {
        std::ostringstream msg;
        if (mapTo == MAP_TO_DGTL) {
            msg << "Mapping digital [" << selectedBtn << "] to:" << "\n";
            mapDigitalToMotorSet(++mapID, selectedBtn, ports);
        }
        else if (mapTo ==  MAP_TO_ANLG) {
            msg << "Mapping analog [" << selectedJoy << "] to:" << "\n";
            mapAnalogToMotorSet(++mapID, selectedJoy, ports);
        }

        for (int port: ports)
            msg << "[" << port << "]\n";
        lv_ta_set_text(message, msg.str().c_str());
    }

    pros::delay(3000);
    lv_obj_del(message);

    lv_obj_del(btnsMap);
}

void Bash::fillDigitalControllerInfo(std::vector<btnData_t> &mappingsData, lv_obj_t *mappingsList) {
    for (std::map<pros::controller_digital_e_t, std::string>::iterator dgtlItr = controllerSetsDigital.begin(); dgtlItr != controllerSetsDigital.end(); ++dgtlItr) {
        btnData_t data;
        std::string controllerInfo = btnToCode[dgtlItr->first];

        data.btn = lv_list_add(mappingsList, NULL, ("(dgtl) " + controllerInfo).c_str(), NULL);

        std::ostringstream info;

        /*data.*/info << btnToCode[dgtlItr->first] << std::endl << std::endl;
        if (motorSets.find(dgtlItr->second) != motorSets.end()) {
            for (int port: motorSets[dgtlItr->second]) {
                /*data.*/info << port << std::endl;
            }
        }
        data.info = info.str();

        mappingsData.push_back(data);
    }
}
void Bash::fillAnalogControllerInfo(std::vector<btnData_t> &mappingsData, lv_obj_t *mappingsList) {
    for (std::map<pros::controller_analog_e_t, std::string>::iterator anlgItr = controllerSetsAnalog.begin(); anlgItr != controllerSetsAnalog.end(); ++anlgItr) {
        btnData_t data;
        std::string controllerInfo = joyToCode[anlgItr->first];

        data.btn = lv_list_add(mappingsList, NULL, ("(anlg) " + controllerInfo).c_str(), NULL);

        std::ostringstream info;
        /*data.*/info << controllerInfo << std::endl << std::endl;
        if (motorSets.find(anlgItr->second) != motorSets.end()) {
            for (int port: motorSets[anlgItr->second]) {
                /*data.*/info << port << std::endl;
            }
        }

        mappingsData.push_back(data);
    }
}
void Bash::renderShowMappings() {
    lv_obj_t *mappingsList = lv_list_create(lv_scr_act(), NULL);
    lv_obj_align(mappingsList, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_size(mappingsList, LV_HOR_RES / 2, LV_VER_RES);

    std::vector<btnData_t> mappingsData; // the left buttons
    fillDigitalControllerInfo(mappingsData, mappingsList);
    fillAnalogControllerInfo(mappingsData, mappingsList);

    lv_obj_t *infoText = lv_ta_create(lv_scr_act(), NULL);
    lv_obj_align(infoText, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_ta_set_text(infoText, "[Controller Value]\n[Ports Connected]");

    Button backBtn("BACK", 0, 0, LV_HOR_RES / 10, LV_VER_RES / 10);


    while (backBtn.getState() != LV_BTN_STATE_PR) {
        for (btnData_t btnData_t: mappingsData) {
            if (lv_btn_get_state(btnData_t.btn) == LV_BTN_STATE_PR) {
                lv_label_set_text(infoText, btnData_t.info/*.str()*/.c_str());
            }
        }
    }

    lv_obj_del(mappingsList);
    lv_obj_del(infoText);
}

void Bash::renderMoveMotors() {
    static const int BTN_STATE_STOPPED = 0;
    static const int BTN_STATE_FORWARD = 1;
    static const int BTN_STATE_REVERSE = 2;
    static const char *btns[] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "\n",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", ""
    };
    int btnStates[sizeof(btns) / sizeof(btns[0])];
    for (int i = 0; i < sizeof(btnStates) / sizeof(btnStates[0]); i++)
        btnStates[i] = 0;


    lv_obj_t *motorBtns = lv_btnm_create(lv_scr_act(), NULL);

    Button backBtn("BACK", 0, 0, LV_HOR_RES / 10, LV_VER_RES / 10);


    while (backBtn.getState() != LV_BTN_STATE_PR) {
        int index = lv_btnm_get_pressed(motorBtns);
        int port = index + 1;
        btnStates[index] = (btnStates[index] + 1) % 3;

        pros::Motor mot(port);

        switch (btnStates[index]) {
            case BTN_STATE_STOPPED:
                mot.move(0);
            break;
            case BTN_STATE_FORWARD:
                mot.move(127);
            break;
            case BTN_STATE_REVERSE:
                mot.move(127);
            break;
        }
    }

    for (int i = 0; i < sizeof(btnStates) / sizeof(btnStates[0]); i++) {
        pros::Motor mot(i);
        mot.move(0);
    }


    lv_obj_del(motorBtns);
}


void Bash::printHelp() {
    std::cout << "Commands:" << std::endl;
    std::cout << "> map" << std::endl
              << "> move" << std::endl
              << "> help" << std::endl;
}

void Bash::updateMappedMotors() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    for (std::map<pros::controller_digital_e_t, std::string>::iterator btnMap = controllerSetsDigital.begin(); btnMap != controllerSetsDigital.end(); ++btnMap) {
        if (motorSets.find(btnMap->second) != motorSets.end()) {
            std::vector<int> ports = motorSets[btnMap->second];
            for (int port: ports) {
                pros::Motor mot(port);
                mot.move(127);
            }
        }
    }

    for (std::map<pros::controller_analog_e_t, std::string>::iterator joyMap = controllerSetsAnalog.begin(); joyMap != controllerSetsAnalog.end(); ++joyMap) {
        if (motorSets.find(joyMap->second) != motorSets.end()) {
            std::vector<int> ports = motorSets[joyMap->second];
            for (int port: ports) {
                pros::Motor mot(port);
                mot.move(127);
            }
        }
    }
}

void Bash::mapControllerDigital(std::vector<std::string> &tokens) {
    std::string key = tokens[2];
    std::string button = tokens[3];
    pros::controller_digital_e_t btnType;

    if (key.compare("help") || key.compare("?")) {
        std::cout << "map digital <key> <button code>" << std::endl;
        std::cout << "codes:" << std::endl
                  << "L1 -> L1" << std::endl
                  << "L2 -> L2" << std::endl
                  << "R1 -> R1" << std::endl
                  << "R2 -> R2" << std::endl
                  << "U  -> UP" << std::endl
                  << "D  -> DOWN" << std::endl
                  << "L  -> LEFT" << std::endl
                  << "R  -> RIGHT" << std::endl
                  << "X  -> X" << std::endl
                  << "B  -> B" << std::endl
                  << "Y  -> Y" << std::endl
                  << "A  -> A" << std::endl;
    }
    else if (key.compare("show")) {
        for (std::map<pros::controller_digital_e_t, std::string>::iterator iter = controllerSetsDigital.begin(); iter != controllerSetsDigital.end(); iter++) {
            std::cout << iter->first << " -> " << iter->second << std::endl;
        }
    }
    else {
        int validButton = 1;

        if (button.compare("L1")) {
            btnType = pros::E_CONTROLLER_DIGITAL_L1;
        }
        else if (button.compare("L2")) {
            btnType = pros::E_CONTROLLER_DIGITAL_L2;
        }
        else if (button.compare("R1")) {
            btnType = pros::E_CONTROLLER_DIGITAL_R1;
        }
        else if (button.compare("R2")) {
            btnType = pros::E_CONTROLLER_DIGITAL_R2;
        }
        else if (button.compare("U")) {
            btnType = pros::E_CONTROLLER_DIGITAL_UP;
        }
        else if (button.compare("D")) {
            btnType = pros::E_CONTROLLER_DIGITAL_DOWN;
        }
        else if (button.compare("L")) {
            btnType = pros::E_CONTROLLER_DIGITAL_LEFT;
        }
        else if (button.compare("R")) {
            btnType = pros::E_CONTROLLER_DIGITAL_RIGHT;
        }
        else if (button.compare("X")) {
            btnType = pros::E_CONTROLLER_DIGITAL_X;
        }
        else if (button.compare("B")) {
            btnType = pros::E_CONTROLLER_DIGITAL_B;
        }
        else if (button.compare("Y")) {
            btnType = pros::E_CONTROLLER_DIGITAL_Y;
        }
        else if (button.compare("A")) {
            btnType = pros::E_CONTROLLER_DIGITAL_A;
        }
        else {
            std::cout << "Error: Input not a button: " << button << std::endl;
            validButton = 0;
        }

        if (validButton) {
            controllerSetsDigital.insert(std::pair<pros::controller_digital_e_t, std::string>(btnType, key));
        }
    }
}

void Bash::mapDigitalToMotorSet(uint32_t id, pros::controller_digital_e_t btn, std::vector<int> ports) {
    std::vector<std::string> tokens;

    tokens[2] = std::to_string(id);
    tokens[3] = btnToCode[btn];

    mapControllerDigital(tokens);
}

void Bash::mapControllerAnalog(std::vector<std::string> &tokens) {
    std::string key = tokens[2];
    std::string channel = tokens[3];
    pros::controller_analog_e_t chnlType;

    if (key.compare("help") || key.compare("?")) {
        std::cout << "map analog <key> <channel code>" << std::endl;
        std::cout << "codes:" << std::endl
                  << "LX -> LEFT X" << std::endl
                  << "LY -> LEFT Y" << std::endl
                  << "RX -> RIGHT X" << std::endl
                  << "RY -> RIGHT Y" << std::endl;
    }
    else if (key.compare("show")) {
        for (std::map<pros::controller_analog_e_t, std::string>::iterator iter = controllerSetsAnalog.begin(); iter != controllerSetsAnalog.end(); iter++) {
            std::cout << iter->first << " -> " << iter->second << std::endl;
        }
    }
    else {
        int validChannel = 1;

        if (channel.compare("LX")) {
            chnlType = pros::E_CONTROLLER_ANALOG_LEFT_X;
        }
        else if (channel.compare("LY")) {
            chnlType = pros::E_CONTROLLER_ANALOG_LEFT_Y;
        }
        else if (channel.compare("RX")) {
            chnlType = pros::E_CONTROLLER_ANALOG_RIGHT_X;
        }
        else if (channel.compare("RY")) {
            chnlType = pros::E_CONTROLLER_ANALOG_RIGHT_Y;
        }
        else {
            std::cout << "Error: Input not a channel: " << channel << std::endl;
            validChannel = 0;
        }

        if (validChannel) {
            controllerSetsAnalog.insert(std::pair<pros::controller_analog_e_t, std::string>(chnlType, key));
        }
    }
}

void Bash::mapAnalogToMotorSet(uint32_t id, pros::controller_analog_e_t joy, std::vector<int> ports) {
    std::vector<std::string> tokens;

    tokens[2] = std::to_string(id);
    tokens[3] = joyToCode[joy];

    mapControllerAnalog(tokens);
}

void Bash::mapMotor(std::vector<std::string> &tokens) {
    std::string key = tokens[2];
    std::vector<int> ports;

    if (key.compare("help") || key.compare("?")) {
        std::cout << "map motor <key> <ports...>" << std::endl;
    }
    else if (key.compare("show")) {
        for (std::map<std::string, std::vector<int>>::iterator iter = motorSets.begin(); iter != motorSets.end(); iter++) {
            std::cout << iter->first << std::endl;

            for (int port: iter->second) {
                std::cout << "> " << port << std::endl;
            }
        }
    }
    else {
        int port;
        for (int i = 3; i < tokens.size(); i++) {
            if (isDigits(tokens[i])) {
                port = stoi(tokens[i]);
                ports.push_back(port);
            }
            else {
                std::cout << "Error: Token is not a digit: " << tokens[i] << std::endl;
            }
        }

        motorSets.insert(std::pair<std::string, std::vector<int>>(key, ports));
    }
}

void Bash::moveMotor(std::vector<std::string> &tokens) {
    std::string motorSetToken = tokens[2];
    std::string speedToken = tokens[3];
    if (isDigits(speedToken)) {
        int speed = stoi(speedToken);

        if (isDigits(motorSetToken)) {
            // set port to speed
            int port = stoi(motorSetToken);
            pros::Motor m(port);
            m.move(speed);
        }
        else {
            // set port set to speed if it exists in motorSets
            if (motorSets.find(motorSetToken) != motorSets.end()) {
                for (int port: motorSets.at(motorSetToken)) {
                    pros::Motor m(port);
                    m.move(speed);
                }
            }
            else {
                std::cout << "Error: motor set not defined: " << motorSetToken << std::endl;
            }
        }
    }
    else {
        std::cout << "Error: speed defined is not a digit: " << speedToken << std::endl;
    }
}

std::vector<std::string> Bash::splitCommand(std::string command) {
    std::vector<std::string> tokens;
    int start = 0;
    int end;

    while (start < command.size()) {
        end = command.find_first_of(" ", start);
        tokens.push_back(command.substr(start, end - start));
        start = end;
    }
}

bool Bash::isDigits(std::string str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}
