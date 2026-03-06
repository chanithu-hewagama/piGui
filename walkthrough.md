# C++ / Python GUI Library Implementation Walkthrough

The project has been successfully implemented at `d:\my_stuff\RESTRICTED\NOPE\GO_AWAY\LIKE_LITALRALY\WHATEVER\01110101001100001010110111101111\Experiments\pyGui`. Due to a system environment configuration issue (Powershell not found in PATH), automated compilation could not be completed, so manual verification is required.

## Project Structure
The repository is set up with a clean structure suitable for GitHub:
- [PyGui.h](file:///d:/my_stuff/RESTRICTED/NOPE/GO_AWAY/LIKE_LITALRALY/WHATEVER/01110101001100001010110111101111/Experiments/pyGui/cpp_library/PyGui.h) & [PyGui.cpp](file:///d:/my_stuff/RESTRICTED/NOPE/GO_AWAY/LIKE_LITALRALY/WHATEVER/01110101001100001010110111101111/Experiments/pyGui/cpp_library/PyGui.cpp): C++ frontend library.
- [pygui_backend.py](file:///d:/my_stuff/RESTRICTED/NOPE/GO_AWAY/LIKE_LITALRALY/WHATEVER/01110101001100001010110111101111/Experiments/pyGui/python_backend/pygui_backend.py): The `tkinter` backend script.
- [main.cpp](file:///d:/my_stuff/RESTRICTED/NOPE/GO_AWAY/LIKE_LITALRALY/WHATEVER/01110101001100001010110111101111/Experiments/pyGui/example/main.cpp): An example C++ application using the library.
- `CMakeLists.txt`: Configured to build the `main.cpp` example and link the library.
- `README.md`, `.gitignore`, `LICENSE`: Standard files for open-source repositories.

## Key Features Implemented

### 1. Inter-Process Communication Details
The C++ process and the Python backend run simultaneously and interact asynchronously without a direct memory bridge or sockets:
* **`gui_commands.txt`**: C++ opens this file in *append* mode. Commands like `WINDOW "Title"`, `BUTTON id x y`, and `LOG "message"` are written line-by-line. Python reads this file periodically, tracking the last read line, and uses the `tkinter` `after` function to periodically execute these UI updates on its main thread.
* **`gui_events.txt`**: When a widget is interacted with in the Python UI, an event is appended here (e.g. `CLICK btn_id`). The C++ application spawns a background thread that constantly reads this file. When read, it locks a mutex, pushes the event string to an internal queue, and makes it accessible to the main thread via the `hasEvent` or `waitForEvent` methods.

### 2. Console Logging Feature
Per your request, a logging feature was built in. 
* C++ uses `gui.setLogArea(...)` to tell Python to initialize a scrolling Text widget.
* C++ can then use `gui.log("Some message")`. This command prints normally to `stdout` in the C++ terminal, but *also* writes `LOG "Some message"` to the text file. The Python script picks this up and appends it to the Text widget in real-time.

### 3. Example Use Case
The `main.cpp` demonstrates initializing the backend, creating a UI, setting up the log widget, sending initial log text, and spinning in a while loop reacting to button clicks (logging a response when `btn_click` is clicked, and exiting when `btn_exit` is clicked).

## Manual Verification Steps

To test this project on your machine, please use your standard compiler or CMake environment:

1. Open a terminal (like Command Prompt, Git Bash, or developer command prompt) in the project root: `d:\...\Experiments\pyGui`.
2. Generate build files:
   ```cmd
   cmake -B build -S .
   ```
3. Compile the executable:
   ```cmd
   cmake --build build
   ```
4. Copy the python backend next to the executable (if your build path changes directories, or just change directories to `python_backend` temporarily).
5. Run the generated `example_app.exe`.

You should see a window open, buttons appear, and logging functional within the UI!
