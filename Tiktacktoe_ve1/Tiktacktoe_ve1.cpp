#include <raylib.h>

enum state { emp = 0, p1 = 1, p2 = 2 };

class layout {
public:
    layout();
    void Drawbox();
    bool isWon();
    void handleInput();
    state getWinner() const { return winner; }

private:
    static const int row = 3;
    static const int column = 3;
    static const int cellSize = 200;
    state tile[row][column];
    state currentPlayer;
    state winner;
};

layout::layout() {`
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            tile[r][c] = emp;
        }
    }
    currentPlayer = p1;
    winner = emp;
}

void layout::handleInput() {
    if (winner != emp) return; // no input if game already won

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        int c = mousePos.x / cellSize;
        int r = mousePos.y / cellSize;

        if (r >= 0 && r < row && c >= 0 && c < column) {
            if (tile[r][c] == emp) {
                tile[r][c] = currentPlayer;
                if (!isWon()) {  // switch player only if no winner yet
                    currentPlayer = (currentPlayer == p1) ? p2 : p1;
                }
            }
        }
  }

bool layout::isWon() {
    // Check rows
    for (int r = 0; r < row; r++) {
        if (tile[r][0] != emp &&
            tile[r][0] == tile[r][1] &&
            tile[r][1] == tile[r][2]) {
            winner = tile[r][0];
            return true;
        }
    }

    // Check columns
    for (int c = 0; c < column; c++) {
        if (tile[0][c] != emp &&
            tile[0][c] == tile[1][c] &&
            tile[1][c] == tile[2][c]) {
            winner = tile[0][c];
            return true;
        }
    }

    // Check main diagonal
    if (tile[0][0] != emp &&
        tile[0][0] == tile[1][1] &&
        tile[1][1] == tile[2][2]) {
        winner = tile[0][0];
        return true;
    }

    // Check anti-diagonal
    if (tile[0][2] != emp &&
        tile[0][2] == tile[1][1] &&
        tile[1][1] == tile[2][0]) {
        winner = tile[0][2];
        return true;
    }

    return false;
}

void layout::Drawbox() {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            int x = c * cellSize;
            int y = r * cellSize;

            DrawRectangle(x, y, cellSize, cellSize, WHITE);
            DrawRectangleLines(x, y, cellSize, cellSize, BLACK);

            if (tile[r][c] == p1 || tile[r][c] == p2) {
                const char* text = (tile[r][c] == p1) ? "X" : "O";
                int fontSize = 100;
                int textWidth = MeasureText(text, fontSize);
                int xPos = x + (cellSize - textWidth) / 2;
                int yPos = y + (cellSize - fontSize) / 2;
                Color color = (tile[r][c] == p1) ? BLUE : RED;
                DrawText(text, xPos, yPos, fontSize, color);
            }
        }
    }
}

int main() {
    InitWindow(600, 600, "Tic Tac Toe with Winning Logic");
    layout game;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.handleInput();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        game.Drawbox();

        // Display winner message if any
        state w = game.getWinner();
        if (w != emp) {
            const char* winnerText = (w == p1) ? "Blue (X) wins!" : "Red (O) wins!";
            int fontSize = 30;
            int textWidth = MeasureText(winnerText, fontSize);
            DrawText(winnerText, (600 - textWidth) / 2, 610 - 40, fontSize, DARKGRAY);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
