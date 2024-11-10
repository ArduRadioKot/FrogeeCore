import customtkinter as ctk
import tkinter as tk
from tkinter import simpledialog, filedialog, messagebox

class textIde(ctk.CTk):
    def __init__(self, master=None):
        super().__init__()

        self.title("Редактор кода")
        self.geometry("800x600")
        self.root = ctk.CTk()
        self.toolbar = ctk.CTkFrame(self.master, width=50, height=768)
        self.toolbar.pack(side="left", fill="y")

        self.label = ctk.CTkLabel(self.toolbar, text="ForgeeCore", font=("Arial", 15))
        self.label.pack(pady=10)

        self.paint_button = ctk.CTkButton(self.toolbar, text="Open", command=self.save)
        self.paint_button.pack(fill="x", pady=10)

        self.ide_button = ctk.CTkButton(self.toolbar, text="Save", command=self.open)
        self.ide_button.pack(fill="x", pady=10)

        self.run_button = ctk.CTkButton(self.toolbar, text="Quit", command=self.quit)
        self.run_button.pack(fill="x", pady=10)



        self.text_area = tk.Text(self, font=("Consolas", 12))
        self.text_area.pack(fill="both", side=ctk.RIGHT, expand=True)
        self.text_area.tag_config('keyword', foreground='#f250e7')  # purple
        self.text_area.tag_config('builtin', foreground='#9950f2')  # blue-violet
        self.text_area.tag_config('string', foreground='#6a8759')  # green
        self.text_area.tag_config('comment', foreground='#546e56')  # dark green
        self.text_area.tag_config('unknown', foreground='#84adf0')  # blue-violet
        self.text_area.tag_config('class', foreground='#48b083')  # green-blue
        self.text_area.tag_config('clas', foreground='#477ad1')  # green-blue
        self.text_area.tag_config('def_func', foreground='#dee86d')  # light green

        self.keywords = ['if', 'else', 'for', 'while', 'import']
        self.builtins = ['self', 'print', 'len', 'range', 'list', 'dict', 'set', 'int', 'float', 'str', 'bool']
        self.strings = ['"', "'", '"""', "'''"]
        self.comments = ['#']
        self.clas = ['class', 'function', 'func']

        # self.text_area.bind("<KeyRelease>", self.auto_brace)
        self.text_area.bind("<KeyRelease>", self.highlight_syntax_realtime)  
        self.line_number_area = tk.Text(self, width=5, height=400, font=("Consolas", 12))
        self.line_number_area.pack(side=ctk.LEFT, fill="y")
        self.update_line_numbers()

    def auto_brace(self, event):
        if event.char in "{}[]()<>":
            self.text_area.insert("insert", event.char)
            self.text_area.insert("insert", self.get_closing_brace(event.char))
            self.text_area.mark_set("insert", "insert -1c")

    def get_closing_brace(self, char):
        braces = {"{": "}", "[": "]", "(": ")", "<":">"}
        return braces[char]

    def update_line_numbers(self):
        self.line_number_area.delete("1.0", "end")
        for i, line in enumerate(self.text_area.get("1.0", "end").split("\n"), start=1):
            self.line_number_area.insert("end", f"{i}\n")
        self.after(100, self.update_line_numbers)

    def highlight_syntax_realtime(self, event):
        self.text_area.tag_remove('keyword', '1.0', 'end')
        self.text_area.tag_remove('builtin', '1.0', 'end')
        self.text_area.tag_remove('string', '1.0', 'end')
        self.text_area.tag_remove('comment', '1.0', 'end')
        self.text_area.tag_remove('unknown', '1.0', 'end')
        self.text_area.tag_remove('class', '1.0', 'end')
        self.text_area.tag_remove('clas', '1.0', 'end')
        self.text_area.tag_remove('def_func', '1.0', 'end')

        text = self.text_area.get('1.0', 'end-1c')
        lines = text.split('\n')

        for i, line in enumerate(lines, 1):
            words = line.split()
            for j, word in enumerate(words):
                if word in self.keywords:
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('keyword', start, end)

                elif word in self.builtins:
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('builtin', start, end)

                elif word == 'class':
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('clas', start, end)

                    if j + 1 < len(words):
                        next_word = words[j + 1]
                        start = f'{i}.{line.index(next_word)}'
                        end = f'{i}.{line.index(next_word) + len(next_word)}'
                        self.text_area.tag_add('class', start, end)

                elif word == 'def':
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('clas', start, end)

                    if j + 1 < len(words):
                        next_word = words[j + 1]
                        start = f'{i}.{line.index(next_word)}'
                        end = f'{i}.{line.index(next_word) + len(next_word)}'
                        self.text_area.tag_add('def_func', start, end)

                elif word.endswith('__') or word.endswith('.__'):  # Check for class names
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('class', start, end)

                elif word not in self.keywords and word not in self.builtins and not word.isdigit():
                    start = f'{i}.{line.index(word)}'
                    end = f'{i}.{line.index(word) + len(word)}'
                    self.text_area.tag_add('unknown', start, end)

            for string in self.strings:
                if string in line:
                    start = f'{i}.{line.index(string)}'
                    end = f'{i}.{line.index(string) + len(string)}'
                    self.text_area.tag_add('string', start, end)

            for comment in self.comments:
                if comment in line:
                    start = f'{i}.{line.index(comment)}'
                    end = f'{i}.end'
                    self.text_area.tag_add('comment', start, end)
    def run(self):
        self.mainloop()

    def open(self):
        self.text_area.delete("1.0", "end")
        file_path = filedialog.asksaveasfilename(defaultextension=".fs", filetypes=[ ('C++ files', '*.h'), ('FrogeScript', '*.fs'), ('Python files')])
        # If a file was selected, save the contents of the text editor to the file
        if file_path:
            with open(file_path, 'w') as file:
                code = self.text_editor.get("1.0", tk.END)
                file.write(code)
    def save(self):
        file_path = filedialog.askopenfilename(filetypes=[('FrogeScript', '*.fs'), ('C++ files', '*.cpp'), ('Python files')])
    # If a file was selected, open it and insert its contents into the text editor
        if file_path:
            with open(file_path, 'r') as file:
                code = file.read()
                self.text_editor.delete('1.0', tk.END)
                self.text_editor.insert(tk.END, code)
    def quit(self):
        self.destroy()
        self.quit()

if __name__ == "__main__":
    app = textIde()
    app.run()