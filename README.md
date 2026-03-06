# Prerequisites
- C++17 or higher
- CMake 3.10+
- Python 3.x
- `tkinter` (usually included with standard Python installations)

# Build Instructions (Windows/Linux/Mac)

**Using g++ (Recommended):**
1. Open a terminal in the project root: `d:\...\Experiments\pyGui`
2. Compile the application:
   ```cmd
   g++ -std=c++17 -I./cpp_library cpp_library/PyGui.cpp example/main.cpp -o example_app.exe
   ```
3. Run the application:
   ```cmd
   example_app.exe
   ```

**Using CMake:**
1. Clone this repository.
2. Create a build directory: `mkdir build && cd build`
3. Generate build files: `cmake ..`
4. Compile the project: `cmake --build .`
5. Run the example executable generated in the build directory.

**Note on Windows:** You may need to copy `pygui_backend.py` from the `python_backend` folder to the same directory as your compiled `.exe` before running it, so that the C++ application can start the backend script correctly.

# How it Works
1. When you initialize `PyGui` in C++, it launches `pygui_backend.py` in the background.
2. The C++ application writes commands (like `ADD_BUTTON` or `LOG`) to `gui_commands.txt`.
3. The Python program constantly polls `gui_commands.txt` and updates the window.
4. When a user interacts with the tkinter window, Python writes events to `gui_events.txt`.
5. The C++ application polls `gui_events.txt` and processes UI events in its main loop.

# API Overview (C++)
- `gui.init()`: Starts the background process.
- `gui.createWindow(title, w, h)`
- `gui.addLabel(id, text, x, y)`
- `gui.addButton(id, text, x, y)`
- `gui.setLogArea(x, y, w, h)`: Creates a scrolling text area.
- `gui.log(message)`: Prints to stdout AND the GUI log area.
- `gui.waitForEvent()` or `gui.hasEvent()`: Fetches user interactions.


