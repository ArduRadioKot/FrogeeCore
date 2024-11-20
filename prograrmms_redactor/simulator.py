
import tkinter as tk
import time

class ArduinoSimulator:
    def __init__(self):
        self.pins = {}
        self.variables = {}
        self.constants = {}
        self.output = []
        self.display_output = [[" " for _ in range(128)] for _ in range(8)]  # 8 строк по 128 символов

    def pin_init(self, pin, mode):
        self.pins[pin] = {'mode': mode, 'value': 0}

    def pin_read(self, pin):
        if pin in self.pins and self.pins[pin]['mode'] == 'INPUT':
            return self.pins[pin]['value']
        return None

    def pin_write(self, pin, value):
        if pin in self.pins and self.pins[pin]['mode'] == 'OUTPUT':
            self.pins[pin]['value'] = value

    def delay(self, milliseconds):
        time.sleep(milliseconds / 1000.0)

    def printlnSerial(self, message):
        self.output.append(str(message))
        print(message)

    def printSerial(self, message):
        self.output.append(str(message))

    def displayText(self, text, x, y):
        if 0 <= x <= 127 and 0 <= y <= 63:
            for i, char in enumerate(text):
                if x + i < 128:
                    row = y // 8
                    col = x + i
                    if row < len(self.display_output):
                        self.display_output[row][col] = char

    def displayClear(self):
        self.display_output = [[" " for _ in range(128)] for _ in range(8)]

    def displayRect(self, x, y, width, height):
        for i in range(width):
            for j in range(height):
                if 0 <= x + i < 128 and 0 <= y + j < 64:
                    row = (y + j) // 8
                    col = x + i
                    if row < len(self.display_output):
                        self.display_output[row][col] = '#'

    def displayPixel(self, x, y):
        if 0 <= x < 128 and 0 <= y < 64:
            row = y // 8
            col = x
            if row < len(self.display_output):
                self.display_output[row][col] = '*'

    def run_code(self, code):
        try:
            exec(code, {"print": self, "display": self.displayText, "displayRect": self.displayRect, "displayClear": self.displayClear, "displayPixel": self.displayPixel})
        except Exception as e:
            self.printlnSerial(f"Ошибка: {e}")

class ArduinoApp:
    def __init__(self, root):
        self.simulator = ArduinoSimulator()
        self.root = root
        self.root.title("Arduino Simulator")

        self.create_widgets()

    def create_widgets(self):
        self.code_text = tk.Text(self.root, height=10, width=50)
        self.code_text.pack(pady=10)

        self.run_button = tk.Button(self.root, text="Выполнить код", command=self.run_code)
        self.run_button.pack(pady=10)

        self.clear_button = tk.Button(self.root, text="Очистить дисплей", command=self.clear_display)
        self.clear_button.pack(pady=5)

        self.oled_display = tk.Text(self.root, height=8, width=20, bg="black", fg="white", font=("Courier", 10))
        self.oled_display.pack(pady=10)

        self.terminal_display = tk.Text(self.root, height=10, width=50, bg="black", fg="green", font=("Courier", 10))
        self.terminal_display.pack(pady=10)

    def run_code(self):
        code = self.code_text.get("1.0", tk.END)
        self.simulator.run_code(code)
        self.update_display()

    def clear_display(self):
        self.simulator.displayClear()
        self.update_display()

    def update_display(self):
        self.oled_display.delete(1.0, tk.END)

        for row in self.simulator.display_output:
            line = ''.join(row)
            self.oled_display.insert(tk.END, line + "\n")
        
        self.oled_display.see(tk.END)

        self.terminal_display.delete(1.0, tk.END)
        for line in self.simulator.output:
            self.terminal_display.insert(tk.END, line + "\n")
        self.terminal_display.see(tk.END)

if __name__ == "__main__":
    root = tk.Tk()
    app = ArduinoApp(root)
    root.mainloop()