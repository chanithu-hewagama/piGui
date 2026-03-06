#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "PyGui.h"

int main() {
    PyGui gui;
    
    std::cout << "Starting PyGui Example..." << std::endl;
    
    if (!gui.init()) {
        std::cerr << "Failed to initialize GUI backend." << std::endl;
        return 1;
    }

    // Give the python backend a moment to start
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    gui.createWindow("PyGui C++ to Python App", 800, 600);
    
    gui.addLabel("lbl_welcome", "Welcome to PyGui!", 350, 50);
    gui.addButton("btn_click", "Click Me!", 350, 100);
    gui.addButton("btn_exit", "Exit Application", 350, 150);
    
    gui.addLabel("lbl_log_title", "Console Application Output:", 50, 280);
    gui.setLogArea(50, 300, 700, 250);
    
    gui.log("PyGui initialized successfully.");
    gui.log("Waiting for user input...");

    bool running = true;
    while(running) {
        std::string event;
        if (gui.hasEvent(event)) {
            gui.log("Received event from GUI: " + event);
            
            if (event == "CLICK btn_click") {
                gui.log("The button was clicked! C++ is processing it...");
            } else if (event == "CLICK btn_exit") {
                gui.log("Exit button clicked. Shutting down.");
                running = false;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    gui.log("Exiting in 2 seconds...");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
