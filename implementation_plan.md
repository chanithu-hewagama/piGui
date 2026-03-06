# Goal Description
Create a lightweight GUI library for C++ that utilizes a Python backend (via `tkinter`) for rendering. The unique requirement is that the two languages must communicate entirely via intermediate `.txt` files. The project will be structured for publishing on GitHub.

## Proposed Architecture
The project will have two main components that run concurrently:
1. **C++ Frontend Library (`PyGui`)**: An easy-to-use C++ class that developers include in their projects. It provides methods like `create_window`, `add_button`, etc.
2. **Python Backend (`pygui_backend.py`)**: A Python script running `tkinter`. It continuously polls for new commands, updates the GUI, and writes user interactions (like button clicks) back to an events file.

### Communication Protocol
We will use two text files to prevent read/write conflicts:
- `gui_commands.txt`: Written to by C++, read by Python.
  - C++ appends commands line by one (e.g., `ADD_BUTTON my_btn 50 50 "Click Me"` or `LOG "This is a console message"`).
  - Python polls this file, keeps track of the last read line, and executes new commands. If it receives a `LOG` command, it appends the text to a scrolling text widget.
- `gui_events.txt`: Written to by Python, read by C++.
  - Python appends events (e.g., `CLICK my_btn`).
  - C++ polls this file to trigger callbacks or return events to the user application.
  
Files are cleared upon initialization to prevent leftover state from previous runs.

## Proposed Changes

### Project Structure
- `cpp_library/PyGui.h`
- `cpp_library/PyGui.cpp`
- `python_backend/pygui_backend.py`
- `example/main.cpp`
- `CMakeLists.txt`
- `README.md`
- `.gitignore`
- `LICENSE`

---

## Verification Plan

### Automated Tests
Currently, this focuses on integration testing.
- Write a `main.cpp` example that creates a window, adds a button, and responds to a click by updating a label.
- Run the example. If text files (`gui_commands.txt` and `gui_events.txt`) are generated and populated correctly, and the Python window appears and reacts to clicks, the core functionality works.

### Manual Verification
1. Open the project in the terminal.
2. Use CMake to configure and build the C++ example.
3. Run the compiled executable.
4. Verify that the Python GUI window opens.
5. Click a button in the GUI and observe if the C++ console registers the click (via reading the txt file).
