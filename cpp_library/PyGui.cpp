#include "PyGui.h"
#include <iostream>
#include <cstdlib>
#include <chrono>

PyGui::PyGui() : m_running(false) {
    // Clear files on startup
    std::ofstream cmdClear(m_commandsFile, std::ios::trunc);
    cmdClear.close();
    std::ofstream eventClear(m_eventsFile, std::ios::trunc);
    eventClear.close();

    m_cmdStream.open(m_commandsFile, std::ios::app);
}

PyGui::~PyGui() {
    m_running = false;
    if (m_pollThread.joinable()) {
        m_pollThread.join();
    }
    m_cmdStream.close();
    cleanup();
}

bool PyGui::init() {
    m_running = true;
    // Attempt to launch the python script from the python_backend directory
    // This allows it to run correctly if launched from the project root
    std::string command = "start /B python python_backend/pygui_backend.py";
    
    // For Unix-like systems, we fallback to just running it in background
    #ifndef _WIN32
    command = "python3 python_backend/pygui_backend.py &";
    #endif

    int result = std::system(command.c_str());
    m_pollThread = std::thread(&PyGui::pollEvents, this);
    
    return result == 0;
}

void PyGui::sendCommand(const std::string& cmd) {
    if (m_cmdStream.is_open()) {
        m_cmdStream << cmd << std::endl;
        m_cmdStream.flush();
    }
}

void PyGui::createWindow(const std::string& title, int width, int height) {
    sendCommand("WINDOW \"" + title + "\" " + std::to_string(width) + " " + std::to_string(height));
}

void PyGui::addLabel(const std::string& id, const std::string& text, int x, int y) {
    sendCommand("LABEL " + id + " " + std::to_string(x) + " " + std::to_string(y) + " \"" + text + "\"");
}

void PyGui::addButton(const std::string& id, const std::string& text, int x, int y) {
    sendCommand("BUTTON " + id + " " + std::to_string(x) + " " + std::to_string(y) + " \"" + text + "\"");
}

void PyGui::setLogArea(int x, int y, int width, int height) {
    sendCommand("LOGAREA " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(width) + " " + std::to_string(height));
}

void PyGui::log(const std::string& message) {
    // Escape quotes if sending string
    sendCommand("LOG \"" + message + "\"");
    std::cout << message << std::endl; // Also log to standard terminal
}

bool PyGui::hasEvent(std::string& outEvent) {
    std::lock_guard<std::mutex> lock(m_eventMutex);
    if (!m_eventQueue.empty()) {
        outEvent = m_eventQueue.front();
        m_eventQueue.erase(m_eventQueue.begin());
        return true;
    }
    return false;
}

std::string PyGui::waitForEvent() {
    std::string eventStr;
    while (m_running) {
        if (hasEvent(eventStr)) {
            return eventStr;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return "";
}

void PyGui::pollEvents() {
    std::streampos lastPos = 0;
    while (m_running) {
        std::ifstream eventFile(m_eventsFile);
        if (eventFile.is_open()) {
            eventFile.seekg(lastPos);
            std::string line;
            while (std::getline(eventFile, line)) {
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                if (!line.empty()) {
                    std::lock_guard<std::mutex> lock(m_eventMutex);
                    m_eventQueue.push_back(line);
                }
            }
            if (eventFile.eof()) {
                eventFile.clear();
                lastPos = eventFile.tellg();
            }
            eventFile.close();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void PyGui::cleanup() {
    std::remove(m_commandsFile.c_str());
    std::remove(m_eventsFile.c_str());
}
