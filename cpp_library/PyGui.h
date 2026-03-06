#ifndef PYGUI_H
#define PYGUI_H

#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class PyGui {
public:
    PyGui();
    ~PyGui();

    // Start the Python GUI process
    bool init();

    // Core GUI commands
    void createWindow(const std::string& title, int width, int height);
    void addLabel(const std::string& id, const std::string& text, int x, int y);
    void addButton(const std::string& id, const std::string& text, int x, int y);
    void setLogArea(int x, int y, int width, int height); // Define where logs appear
    
    // Output directly to the GUI log console
    void log(const std::string& message);
    
    // Check if an event (like a button click) has occurred
    bool hasEvent(std::string& outEvent);

    // Wait for a specific event or any event
    std::string waitForEvent();

    // Clean up temporary text files
    void cleanup();

private:
    void sendCommand(const std::string& cmd);
    void pollEvents();

    std::string m_commandsFile = "gui_commands.txt";
    std::string m_eventsFile = "gui_events.txt";

    std::ofstream m_cmdStream;
    
    std::thread m_pollThread;
    std::mutex m_eventMutex;
    std::vector<std::string> m_eventQueue;
    std::atomic<bool> m_running;
    int m_lastEventLine = 0;
};

#endif // PYGUI_H
