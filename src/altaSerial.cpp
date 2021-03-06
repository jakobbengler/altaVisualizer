//
//  altaSerial.cpp
//  altaVisualizer
//
//  Created by Jakob Bak on 30/03/16.
//
//

#include "string.h"
#include "ofSerial.h"
#include "AltaSerial.h"
#include "ofApp.h"


ofSerial serial;

void AltaSerial::init() {
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    serial.setup(0, 115200); //open the first device and talk to it at 115200 baud
    linebuffer = "";
}


void AltaSerial::checkSerial() {
    stringstream inbuffer;
    inbuffer << linebuffer;
    while(serial.available()) {
        char in_char;
        in_char = serial.readByte();
        if(in_char == '\r') continue;
        if(in_char == OF_SERIAL_NO_DATA) {
            cout << "no data was read" << endl;
            return;
        } else if(in_char == OF_SERIAL_ERROR) {
            cout << "an error occurred" << endl;
            return;
        } else {
            inbuffer << in_char;
        }
    }
    if(!inbuffer.str().empty()) {
        string newline = inbuffer.str();
        vector<string> newlines = ofSplitString(newline, "\n", false, false);
        int numlines = newlines.size();
        for(int i=0; i < numlines-1; i++) {
            // parse the line here
            parseSerial(newlines[i]);
//            cout << "[" << i << "] " << newlines[i] << endl;
        }
        
        linebuffer = newlines[numlines-1];
//        cout << "nindex is " << nindex << endl;
//        cout << "remaining text is: " << linebuffer << endl;
        inbuffer.str("");
    }
    
}

void AltaSerial::parseSerial(string line) {
    alta_printer->alive = 0.001;
    if(ofIsStringInString(line, "_DEBUG")) {
        vector<string> debug_msg = ofSplitString(line, "@");
        int debugID = getDebugMessageType(debug_msg[0]);
        switch(debugID) {
            case 1:
            {
                vector<string> runtime_value = ofSplitString(debug_msg[1], " ");
//                for(int i=0; i < runtime_value.size(); i++) {
//                    cout << "[" << i << "]" << runtime_value[i] << endl;
//                }
                string label = runtime_value[0];
                float value;
                try {
                    value = stof(runtime_value[1]);
                } catch (invalid_argument ia) {
                    break;
                }
                realtime_plotter->addDataPoint(label, value);
//                cout << label << "=" << defaultfloat << setprecision(5) << value << endl;
            }
                break;
            case 2:
            {
                vector<string> runtime_value = ofSplitString(debug_msg[1], " ");
                //                for(int i=0; i < runtime_value.size(); i++) {
                //                    cout << "[" << i << "]" << runtime_value[i] << endl;
                //                }
                string label = runtime_value[0];
                float value;
                try {
                    value = stof(runtime_value[1]);
                } catch (invalid_argument ia) {
                    break;
                }
//                realtime_plotter->addDataPoint(label, value);
                //                cout << label << "=" << defaultfloat << setprecision(5) << value << endl;
            }
                break;
            default:
                break;
        }
        
    } else {
        if(ofIsStringInString(line, "ok T:")) {
            vector<string> str = ofSplitString(line, ":");
            vector<string> temp = ofSplitString(str[1], "/");
            alta_printer->hotend = atof(temp[0].c_str());
            alta_printer->setpoint = atof(temp[1].c_str());
        } else if(ofIsStringInString(line, "ok bn:")) {
            vector<string> nodes = ofSplitString(line, ":");
            alta_printer->node_buffer = atoi(nodes[1].c_str());
        } else {
//        cout << setprecision(4) << defaultfloat;
//        cout << defaultfloat << setprecision(9);
        cout << line << endl;
//        cout << 0.123456789 << endl;
        }
    }
}

int AltaSerial::getDebugMessageType(string debugID) {
    if(debugID == "_DEBUG_") return 1;
    else if(debugID == "_DEBUG_P") return 2;
    else return 0;
}



