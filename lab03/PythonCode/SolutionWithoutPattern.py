import tkinter as tk
from tkinter import messagebox
import copy

class History:
    def __init__(self):
        self._undo_stack = []
        self._redo_stack = []

    def save(self, grid, moves):
        state = (copy.deepcopy(grid), moves)
        self._undo_stack.append(state)
        self._redo_stack.clear()

    def undo(self, current_grid, current_moves):
        if not self._undo_stack:
            return None
        self._redo_stack.append((copy.deepcopy(current_grid), current_moves))
        return self._undo_stack.pop()

    def redo(self, current_grid, current_moves):
        if not self._redo_stack:
            return None  
        self._undo_stack.append((copy.deepcopy(current_grid), current_moves))
        return self._redo_stack.pop()

class NonogramCore:
    def __init__(self):
        self.solution = [
            [1, 1, 0, 0, 1, 0, 0, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 1, 0, 1, 1],
            [1, 0, 0, 0, 0, 0, 1, 0, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
            [1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
            [1, 1, 1, 0, 0, 0, 1, 1, 1, 1],
            [1, 1, 1, 1, 0, 1, 1, 1, 0, 1],
            [1, 0, 1, 1, 1, 1, 1, 0, 1, 0],
            [0, 1, 0, 1, 1, 0, 1, 1, 0, 1],
            [1, 0, 1, 1, 0, 1, 0, 1, 1, 1]
        ]
        self.rows = len(self.solution)
        self.cols = len(self.solution[0])
        self.player_grid = [[0 for _ in range(self.cols)] for _ in range(self.rows)]
        self.moves = 0

    def get_hints(self, line):
        hints = []
        count = 0
        for cell in line:
            if cell == 1:
                count += 1
            elif count > 0:
                hints.append(str(count))
                count = 0
        if count > 0: hints.append(str(count))
        return hints if hints else ["0"]

    def check_win(self):
        current_state = [[1 if val == 1 else 0 for val in row] for row in self.player_grid]
        return current_state == self.solution

class NonogramApp:
    def __init__(self, root):
        self.root = root
        self.core = NonogramCore()
        self.history = History()
        self.cell_size = 35
        self._create_widgets()
        self._create_menu()

    def _create_widgets(self):
        self.move_label = tk.Label(self.root, text=f"Ходы: {self.core.moves}", font=("Arial", 12, "bold"))
        self.move_label.grid(row=0, column=0, padx=5, pady=5)

        for c in range(self.core.cols):
            col_data = [self.core.solution[r][c] for r in range(self.core.rows)]
            hint_text = "\n".join(self.core.get_hints(col_data))
            tk.Label(self.root, text=hint_text, font=("Arial", 8)).grid(row=0, column=c+1, sticky="s")

        for r in range(self.core.rows):
            hint_text = " ".join(self.core.get_hints(self.core.solution[r]))
            tk.Label(self.root, text=hint_text, font=("Arial", 8)).grid(row=r+1, column=0, sticky="e", padx=10)

        self.canvas = tk.Canvas(self.root, 
                                width=self.core.cols * self.cell_size, 
                                height=self.core.rows * self.cell_size, 
                                bg="white")
        self.canvas.grid(row=1, column=1, rowspan=self.core.rows, columnspan=self.core.cols, padx=5, pady=5)
        
        self.draw_grid()
        self.canvas.bind("<Button-1>", self.handle_left_click)
        self.canvas.bind("<Button-3>", self.handle_right_click)

    def _create_menu(self):
        btn_frame = tk.Frame(self.root)
        btn_frame.grid(row=self.core.rows + 2, column=0, columnspan=self.core.cols + 2, pady=10)
        
        tk.Button(btn_frame, text="Undo", command=self.undo).pack(side=tk.LEFT, padx=10)
        tk.Button(btn_frame, text="Redo", command=self.redo).pack(side=tk.LEFT, padx=10)
        
        self.root.bind("<Control-z>", self.undo)
        self.root.bind("<Control-y>", self.redo)

    def draw_grid(self):
        self.canvas.delete("all")
        for r in range(self.core.rows):
            for c in range(self.core.cols):
                x1, y1 = c * self.cell_size, r * self.cell_size
                x2, y2 = x1 + self.cell_size, y1 + self.cell_size
                
                val = self.core.player_grid[r][c]
                fill_color = "black" if val == 1 else "white"
                self.canvas.create_rectangle(x1, y1, x2, y2, outline="gray", fill=fill_color)
                
                if val == 2:
                    p = 8
                    self.canvas.create_line(x1+p, y1+p, x2-p, y2-p, fill="red", width=2)
                    self.canvas.create_line(x1+p, y2-p, x2-p, y1+p, fill="red", width=2)

    def handle_left_click(self, event):
        c, r = event.x // self.cell_size, event.y // self.cell_size
        if 0 <= r < self.core.rows and 0 <= c < self.core.cols:
            self.history.save(self.core.player_grid, self.core.moves)
            self.core.player_grid[r][c] = 1 if self.core.player_grid[r][c] != 1 else 0
            self.core.moves += 1
            self.update_ui()

    def handle_right_click(self, event):
        c, r = event.x // self.cell_size, event.y // self.cell_size
        if 0 <= r < self.core.rows and 0 <= c < self.core.cols:
            self.history.save(self.core.player_grid, self.core.moves)
            self.core.player_grid[r][c] = 2 if self.core.player_grid[r][c] != 2 else 0
            self.core.moves += 1
            self.update_ui()

    def undo(self, event=None):
        state = self.history.undo(self.core.player_grid, self.core.moves)
        if state:
            self.core.player_grid, self.core.moves = state
            self.update_ui(check=False)

    def redo(self, event=None):
        state = self.history.redo(self.core.player_grid, self.core.moves)
        if state:
            self.core.player_grid, self.core.moves = state
            self.update_ui(check=False)

    def update_ui(self, check=True):
        self.draw_grid()
        self.move_label.config(text=f"Ходы: {self.core.moves}")
        if check and self.core.check_win():
            messagebox.showinfo("Победа!", "Картинка собрана!")

if __name__ == "__main__":
    root = tk.Tk()
    app = NonogramApp(root)
    root.mainloop()