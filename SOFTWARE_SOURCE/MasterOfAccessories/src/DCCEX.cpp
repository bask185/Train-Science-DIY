#include "DCCEX.h"
#include <EEPROM.h>

static uint16_t eeAddress=0;
// Constructor initializes the serial communication
DCCEX::DCCEX(Stream& serial) : _serial(serial), power(false) {}

// Begin method for initialization
void DCCEX::begin() {
    // Optional: Initialization logic
    sendStatusResponse() ;
}

void DCCEX::process() {
    static char message[64];
    static uint8_t index = 0;

    // Kijk of er data beschikbaar is
    while (_serial.available()) {
        char c = _serial.read();

        // Start een nieuw bericht bij '<'
        if (c == '<') {
            index = 0;  // Reset de index bij het begin van een bericht
            message[index] = '\0';  // Null-terminate het bericht
        }
        // Eindigt het bericht bij '>'
        else if (c == '>') {
            message[index] = '\0';  // Null-terminate het bericht
            parseMessage(message);  // Verwerk het bericht
        }
        // Voeg tekens toe aan het bericht
        else if (index < sizeof(message) - 1) {
            message[index++] = c;
        }
    }
}

void DCCEX::parseMessage(char* message) {

    EEPROM.write( eeAddress ++, message[0] ) ;

    // Check for each known command by manually checking for specific characters
    if (message[0] == 's' && message[1] == '\0') {
        // Handle status request for DCC++ EX CommandStation
        sendStatusResponse();
    } 
    else if (message[0] == '0' && message[1] == '\0') {
        // Power OFF command
        setPower(false);
        sendPowerResponse(false);
    }
    else if (message[0] == '1' && message[1] == '\0') {
        // Power ON command
        setPower(true);
        sendPowerResponse(true);
    }
    else if (message[0] == 'W' && message[1] == ' ') {
        // Handle turnout (accessory) status change
        int i = 2;
        uint16_t address = 0;
        while (message[i] != ' ' && message[i] != '\0') {
            address = address * 10 + (message[i] - '0');
            i++;
        }
        i++; // Skip space
        bool state = (message[i] == '1');
        
        // Trigger turnout callback if set
        if (NotifyDccExPointSet) {
            NotifyDccExPointSet(address, state);
        }
    }
    else if (message[0] == 'q' && message[1] == '\0') {
        // Handle occupancy sensor status change
        sendOccupancyStatusResponse(0, false);  // Always false for this example
        if (_occupancyCallback) {
            _occupancyCallback(0, false);  // False = not occupied
        }
    } else {
    }
}
void DCCEX::setPower(uint8_t _state) {
    power = _state;
}



uint8_t DCCEX::getPower() {
    return power;
}

// Send response for DCC++ EX status (CommandStation)
void DCCEX::sendStatusResponse() {
    _serial.print("<s p ");
    _serial.print(getPower() ? "1" : "0");
    _serial.println(">");
}

// Send a response indicating power state
void DCCEX::sendPowerResponse(bool powerState) {
    _serial.print("<p ");
    _serial.print(powerState ? "1" : "0");
    _serial.println(">");
}

// Send a response to indicate a turnout state change
void DCCEX::setPoint(uint16_t address, bool state) {
    _serial.print("<W ");
    _serial.print(address);
    _serial.print(" ");
    _serial.print(state ? "1" : "0");
    _serial.println(">");
}

// Send a response for occupancy status change
void DCCEX::sendOccupancyStatusResponse(uint16_t address, bool occupied) {
    _serial.print("<q ");
    _serial.print(address);
    _serial.print(" ");
    _serial.println(occupied ? "1" : "0");
}






// #include "DCCEX.h"

// // Constructor initializes the serial communication
// DCCEX::DCCEX(Stream& serial) : _serial(serial), power(false) {}

// // Begin method for initialization
// void DCCEX::begin() {
//     // Optional: Initialization logic
// }

// // Main process to handle incoming data and DCC-EX commands
// void DCCEX::process() {
//     String message = "";
//     while (_serial.available()) {
//         char c = _serial.read();
//         if (c == '<') {
//             message = ""; // Start a new message
//         } else if (c == '>') {
//             // Message complete, process it
//             parseMessage(message);
//         } else if (message.length() < 64) { // Limit message length
//             message += c;
//         }
//     }
// }

// void DCCEX::setPower(uint8_t _state) 
// {
//     power = _state ;
// }

// void DCCEX::parseMessage(const String& message) {
//     // Trim the message to remove leading and trailing whitespace
//     message.trim();

//     // Check for the type of message based on its format
//     if (message.startsWith("<s>")) {
//         // Handle status request for DCC++ EX CommandStation
//         sendStatusResponse();
//     } 
//     else if (message.startsWith("<0>")) {
//         // Power OFF command
//         setPower(false);
//         sendPowerResponse(false);
//     }
//     else if (message.startsWith("<1>")) {
//         // Power ON command
//         setPower(true);
//         sendPowerResponse(true);
//     }
//     else if (message.startsWith("<W ")) {
//         // Handle turnout (accessory) status change: "<W address state>"
//         int spaceIndex = message.indexOf(' ', 3);  // Find the space after <W
//         if (spaceIndex != -1) {
//             uint16_t address = message.substring(3, spaceIndex).toInt();
//             bool state = message.substring(spaceIndex + 1).toInt();
//             // Trigger the turnout callback with the extracted values
//             if (_turnoutCallback) {
//                 _turnoutCallback(address, state);
//             }
//         }
//     }
//     else if (message.startsWith("<q>")) {
//         // Handle occupancy sensor status change: "<q address>"
//         uint16_t address = message.substring(3).toInt();
//         sendOccupancyStatusResponse(address, false);
//         if (_occupancyCallback) {
//             _occupancyCallback(address, false);  // False = not active
//         }
//     }
// }


// uint8_t DCCEX::getPower()
// {
//     return power ;
// }

// // Send response for DCC++ EX status (CommandStation)
// void DCCEX::sendStatusResponse() {
//     _serial.print("<s ");
//     _serial.print(getPower() ? "1" : "0");
//     _serial.println(">");
// }

// // Send a response indicating power state
// void DCCEX::sendPowerResponse(bool powerState) {
//     _serial.print("<p ");
//     _serial.print(powerState ? "1" : "0");
//     _serial.println(">");
// }

// // Send a response to indicate a turnout state change
// void DCCEX::setPoint(uint16_t address, bool state) {
//     _serial.print("<W ");
//     _serial.print(address);
//     _serial.print(" ");
//     _serial.print(state ? "1" : "0");
//     _serial.println(">");
// }

// // Send a response for occupancy status change
// void DCCEX::sendOccupancyStatusResponse(uint16_t address, bool occupied) {
//     _serial.print("<q ");
//     _serial.print(address);
//     _serial.print(" ");
//     _serial.println(occupied ? "1" : "0");
// }
