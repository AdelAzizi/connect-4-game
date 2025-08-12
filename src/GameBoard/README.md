# مستندات کلاس `GameBoard`

این مستند به بررسی دقیق کلاس `GameBoard`، که مسئولیت مدیریت کامل صفحه بازی را بر عهده دارد، می‌پردازد.

## 📝 نمای کلی

کلاس `GameBoard` هسته مرکزی وضعیت بازی را تشکیل می‌دهد. این کلاس یک نمایش داخلی از صفحه بازی `6x7` را مدیریت می‌کند و توابعی برای تعامل با این صفحه فراهم می‌کند، مانند قرار دادن یک مهره جدید، بررسی اعتبار یک حرکت، و شمارش تعداد ردیف‌های چهارتایی.

## 📁 ساختار فایل

کلاس `GameBoard` در دو فایل مجزا پیاده‌سازی شده است:

-   `GameBoard.h`: **فایل هدر (Header File)** که تعریف و ساختار کلاس `GameBoard` را شامل می‌شود.
-   `GameBoard.cpp`: **فایل پیاده‌سازی (Implementation File)** که منطق متدهای تعریف‌شده در هدر را پیاده‌سازی می‌کند.

---

## 📄 تحلیل فایل هدر (`GameBoard.h`)

فایل هدر `GameBoard.h` رابط عمومی کلاس را مشخص می‌کند و به سایر بخش‌های برنامه اجازه می‌دهد تا با صفحه بازی تعامل داشته باشند.

```cpp
#pragma once

#include "../Constants.h"

class GameBoard {
public:
    // The game board grid. Kept public to allow direct access from game logic
    // and AI, mirroring the original design.
    char board[ROWS][COLS];

    // Constructor
    GameBoard();

    void reset();
    bool isValidMove(int col) const;
    bool makeMove(int col, char playerId);
    bool isFull() const;
    int countFours(char playerId) const;
    char getCell(int row, int col) const;

private:
    // Helper function to check for four-in-a-row in a specific direction
    bool checkDirection(int row, int col, int rowDir, int colDir, char playerId) const;
};
```

### بخش‌های مهم کد `GameBoard.h`:

-   **`#include "../Constants.h"`**: این فایل شامل ثابت‌های سراسری پروژه مانند `ROWS` و `COLS` است که ابعاد صفحه بازی را تعیین می‌کنند.
-   **`char board[ROWS][COLS]`**: این یک آرایه دو بعدی از کاراکترها است که وضعیت هر خانه از صفحه بازی را ذخیره می‌کند. مقدار `'*'` نشان‌دهنده یک خانه خالی است، در حالی که `'X'` یا `'O'` نشان‌دهنده مهره بازیکنان است. این متغیر به صورت `public` تعریف شده تا منطق هوش مصنوعی بتواند به راحتی به آن دسترسی داشته باشد و کپی‌هایی از آن برای تحلیل حرکات ایجاد کند.
-   **متدهای عمومی (`public` methods)**:
    -   `GameBoard()`: سازنده‌ای که به طور خودکار متد `reset()` را برای آماده‌سازی صفحه بازی فراخوانی می‌کند.
    -   `reset()`: تمام خانه‌های صفحه را به حالت خالی (`'*'`) برمی‌گرداند.
    -   `isValidMove(col)`: بررسی می‌کند که آیا می‌توان در ستون `col` یک مهره جدید قرار داد.
    -   `makeMove(col, playerId)`: مهره بازیکن `playerId` را در پایین‌ترین خانه خالی ستون `col` قرار می‌دهد.
    -   `isFull()`: بررسی می‌کند که آیا صفحه بازی کاملاً پر شده است یا خیر.
    -   `countFours(playerId)`: **مهم‌ترین تابع منطقی بازی.** این تابع تعداد کل ردیف‌های چهارتایی (افقی، عمودی و مورب) را که توسط بازیکن `playerId` ایجاد شده، محاسبه می‌کند.
    -   `getCell(row, col)`: مقدار خانه مشخص‌شده توسط `row` و `col` را برمی‌گرداند.
-   **متد خصوصی (`private` method)**:
    -   `checkDirection(...)`: یک تابع کمکی که توسط `countFours` استفاده می‌شود تا وجود یک ردیف چهارتایی از یک خانه مشخص و در یک جهت مشخص را بررسی کند.

---

##  cpp. تحلیل فایل پیاده‌سازی (`GameBoard.cpp`)

این فایل جزئیات پیاده‌سازی منطق بازی را در بر می‌گیرد.

```cpp
#include "GameBoard.h"

// ... (implementations of constructor, reset, isValidMove, makeMove, isFull) ...

int GameBoard::countFours(char playerId) const {
    int count = 0;
    // Iterate through each cell as a potential start of a four-in-a-row
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            // Check all four directions from each cell
            if (checkDirection(row, col, 0, 1, playerId)) count++;  // Horizontal
            if (checkDirection(row, col, 1, 0, playerId)) count++;  // Vertical
            if (checkDirection(row, col, 1, 1, playerId)) count++;  // Diagonal (down-right)
            if (checkDirection(row, col, 1, -1, playerId)) count++; // Diagonal (down-left)
        }
    }
    return count;
}

// ... (implementation of getCell) ...

bool GameBoard::checkDirection(int row, int col, int rowDir, int colDir, char playerId) const {
    for (int i = 0; i < 4; i++) {
        int r = row + i * rowDir;
        int c = col + i * colDir;
        // Check if the cell is within bounds and belongs to the player
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != playerId) {
            return false;
        }
    }
    return true;
}
```

### بخش‌های مهم کد `GameBoard.cpp`:

-   **`makeMove(col, playerId)`**: این تابع منطق اصلی "افتادن" مهره را پیاده‌سازی می‌کند. با یک حلقه `for` از پایین‌ترین ردیف (`ROWS - 1`) به سمت بالا حرکت می‌کند و اولین خانه خالی (`'*'`) را با شناسه بازیکن پر می‌کند.

-   **`countFours(playerId)`**: این تابع قلب سیستم امتیازدهی بازی است.
    -   این تابع به جای اینکه فقط یک برد را بررسی کند، **تمام** ترکیب‌های چهارتایی ممکن برای یک بازیکن را می‌شمارد.
    -   با دو حلقه `for` تو در تو، از هر خانه (`board[row][col]`) به عنوان نقطه شروع بالقوه برای یک ردیف چهارتایی استفاده می‌کند.
    -   از هر خانه، تابع کمکی `checkDirection` را در چهار جهت اصلی (افقی، عمودی، و دو جهت مورب) فراخوانی می‌کند.
    -   هر بار که `checkDirection` مقدار `true` برگرداند، شمارنده `count` یک واحد افزایش می‌یابد.

-   **`checkDirection(...)`**: این تابع یک منطق ساده اما قدرتمند دارد.
    -   یک حلقه `for` چهار بار تکرار می‌شود تا چهار خانه متوالی را بررسی کند.
    -   در هر تکرار، با استفاده از پارامترهای `rowDir` و `colDir` (که می‌توانند `0`, `1` یا `-1` باشند)، به خانه بعدی در جهت مشخص شده حرکت می‌کند.
    -   اگر در هر مرحله، خانه‌ای خارج از محدوده صفحه باشد یا به بازیکن مورد نظر تعلق نداشته باشد، تابع بلافاصله `false` برمی‌گرداند.
    -   اگر حلقه با موفقیت به پایان برسد، به این معنی است که یک ردیف چهارتایی معتبر پیدا شده و تابع `true` برمی‌گرداند.
