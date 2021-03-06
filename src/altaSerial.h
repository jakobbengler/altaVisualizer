//
//  AltaSerial.h
//  altaVisualizer
//
//  Created by Jakob Bak on 30/03/16.
//
//

#ifndef ALTASERIAL_H
#define ALTASERIAL_H

#include <stdio.h>
#include <ofThread.h>
#include "ofApp.h"
#include "Views.h"

class AltaSerial : public ofThread {
    
public:
    void init();
    void checkSerial();
    void parseSerial(string line);
    int getDebugMessageType(string debugID);
    void addRealtimePlotter(ViewRealtimePlotter2D *plotter) { realtime_plotter = plotter; }
    void addPrinter(printer_t *printer) { alta_printer = printer; } 
    
    // the thread function
    void threadedFunction() {
        
        // start
        
        while(isThreadRunning()) {
            
            checkSerial();
        }
        
        // done
    }
    
protected:
    string linebuffer;
    ViewRealtimePlotter2D * realtime_plotter;
    printer_t *alta_printer;

    
};



#endif /* ALTASERIAL_H */
