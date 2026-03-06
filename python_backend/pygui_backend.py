import tkinter as tk
import time
import os
import shlex
import threading

import os
# Get the absolute path to the project root (where the example runs)
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
COMMANDS_FILE = os.path.join(PROJECT_ROOT, "gui_commands.txt")
EVENTS_FILE = os.path.join(PROJECT_ROOT, "gui_events.txt")

class BackendGUI:
    def __init__(self):
        self.root = None
        self.last_pos = 0
        self.widgets = {}
        self.log_text = None
        self.running = True

    def run(self):
        try:
            self.root = tk.Tk()
        except Exception as e:
            print(f"Error: Could not initialize tkinter (no display?): {e}")
            return
            
        self.root.title("PyGui Initializing...")
        self.root.geometry("400x300")
        
        # Start command polling
        self.root.after(100, self.poll_commands)
        
        # Handle window close strictly
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        
        self.root.mainloop()

    def on_close(self):
        self.running = False
        self.root.destroy()
        
    def write_event(self, event_type, widget_id):
        try:
            with open(EVENTS_FILE, "a") as f:
                f.write(f"{event_type} {widget_id}\n")
        except Exception as e:
            print(f"Error writing event: {e}")

    def on_button_click(self, widget_id):
        self.write_event("CLICK", widget_id)

    def process_command(self, cmd_line):
        parts = shlex.split(cmd_line)
        if not parts:
            return

        action = parts[0]

        try:
            if action == "WINDOW":
                title = parts[1]
                w, h = parts[2], parts[3]
                self.root.title(title)
                self.root.geometry(f"{w}x{h}")
                
            elif action == "LABEL":
                wid = parts[1]
                x, y = int(parts[2]), int(parts[3])
                text = parts[4]
                label = tk.Label(self.root, text=text)
                label.place(x=x, y=y)
                self.widgets[wid] = label
                
            elif action == "BUTTON":
                wid = parts[1]
                x, y = int(parts[2]), int(parts[3])
                text = parts[4]
                btn = tk.Button(self.root, text=text, command=lambda id=wid: self.on_button_click(id))
                btn.place(x=x, y=y)
                self.widgets[wid] = btn
                
            elif action == "LOGAREA":
                x, y = int(parts[1]), int(parts[2])
                w, h = int(parts[3]), int(parts[4])
                
                # Standard Text widget for logging
                self.log_text = tk.Text(self.root, wrap=tk.WORD, state=tk.DISABLED, bg='black', fg='white')
                self.log_text.place(x=x, y=y, width=w, height=h)
                
            elif action == "LOG":
                if self.log_text is not None:
                    msg = parts[1]
                    self.log_text.config(state=tk.NORMAL)
                    self.log_text.insert(tk.END, msg + "\n")
                    self.log_text.see(tk.END)  # Autoscroll
                    self.log_text.config(state=tk.DISABLED)
                else:
                    print(f"LOGAREA not setup: {parts[1]}")
        except Exception as e:
            print(f"Error processing command '{cmd_line}': {e}")

    def poll_commands(self):
        if not self.running:
            return
            
        try:
            if os.path.exists(COMMANDS_FILE):
                with open(COMMANDS_FILE, "r") as f:
                    # Seek to the last read position
                    f.seek(getattr(self, 'last_pos', 0))
                    line = f.readline()
                    while line:
                        cmd = line.strip()
                        if cmd:
                            self.process_command(cmd)
                        self.last_pos = f.tell()
                        line = f.readline()
        except Exception as e:
            # print(f"Error polling: {e}")
            pass # Ignore read errors temporarily
            
        # Schedule next poll
        self.root.after(50, self.poll_commands)

if __name__ == "__main__":
    app = BackendGUI()
    app.run()
