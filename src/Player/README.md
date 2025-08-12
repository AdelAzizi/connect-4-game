# مستندات کلاس `Player`

این مستند به بررسی دقیق کلاس `Player`، مسئولیت‌ها، متغیرها و متدهای آن می‌پردازد.

## 📝 نمای کلی

کلاس `Player` یک جزء اساسی از منطق بازی است که برای مدیریت اطلاعات و وضعیت هر بازیکن (اعم از انسان یا کامپیوتر) طراحی شده است. این کلاس تمام داده‌های مربوط به یک بازیکن، مانند نام، امتیاز و رنگ مهره‌ها را در خود نگهداری می‌کند.

## 📁 ساختار فایل

کلاس `Player` در دو فایل سازماندهی شده است:

-   `Player.h`: **فایل هدر (Header File)** که ساختار و اعلان‌های کلاس را در بر می‌گیرد.
-   `Player.cpp`: **فایل پیاده‌سازی (Implementation File)** که منطق و بدنه متدهای اعلان‌شده در فایل هدر را پیاده‌سازی می‌کند.

---

## 📄 تحلیل فایل هدر (`Player.h`)

فایل `Player.h` به عنوان رابط (Interface) کلاس عمل می‌کند و مشخص می‌کند که این کلاس چه داده‌هایی را ذخیره کرده و چه قابلیت‌هایی را ارائه می‌دهد.

```cpp
#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Player {
private:
    std::string name;
    char id;
    int score;
    sf::Color color;
    bool computerFlag;

public:
    // Constructor
    Player(const std::string& name = "", char id = ' ', const sf::Color& color = sf::Color::White, bool isComputer = false);

    // Getters
    std::string getName() const;
    char getId() const;
    int getScore() const;
    sf::Color getColor() const;
    bool isComputer() const;

    // Setters
    void setName(const std::string& newName);
    void setScore(int newScore);
};
```

### بخش‌های مهم کد `Player.h`:

-   **`#pragma once`**: این دستور یک بهینه‌سازی مدرن برای جلوگیری از "double inclusion" است. تضمین می‌کند که محتوای این فایل هدر فقط یک بار در هر واحد کامپایل (translation unit) گنجانده شود، که از خطاهای کامپایل جلوگیری می‌کند.

-   **`#include <string>` و `#include <SFML/Graphics.hpp>`**:
    -   `string`: برای استفاده از کلاس `std::string` جهت ذخیره نام بازیکن.
    -   `SFML/Graphics.hpp`: برای دسترسی به نوع داده `sf::Color` که رنگ مهره‌های بازیکن را مشخص می‌کند.

-   **متغیرهای عضو خصوصی (`private` members)**: این متغیرها وضعیت داخلی یک شیء `Player` را ذخیره می‌کنند و فقط از طریق متدهای عمومی کلاس قابل دسترسی هستند.
    -   `std::string name`: نام بازیکن.
    -   `char id`: شناسه منحصربه‌فرد بازیکن (معمولاً 'X' یا 'O').
    -   `int score`: امتیاز فعلی بازیکن در بازی.
    -   `sf::Color color`: رنگ مهره‌های بازیکن در صفحه بازی.
    -   `bool computerFlag`: یک پرچم که مشخص می‌کند آیا این بازیکن توسط کامپیوتر کنترل می‌شود (`true`) یا یک انسان (`false`).

-   **متدهای عمومی (`public` methods)**: این متدها رابطی برای تعامل با شیء `Player` فراهم می‌کنند.
    -   **`Player(...)` (Constructor)**: سازنده کلاس که برای ایجاد یک نمونه جدید از `Player` استفاده می‌شود. این سازنده دارای مقادیر پیش‌فرض برای تمام پارامترها است، که امکان ایجاد یک بازیکن را به روش‌های مختلف فراهم می‌کند.
    -   **Getters (توابع دریافت‌کننده)**: متدهایی مانند `getName()` و `getScore()` که مقدار متغیرهای عضو خصوصی را بدون تغییر آن‌ها برمی‌گردانند. کلمه کلیدی `const` در انتهای این متدها تضمین می‌کند که آن‌ها وضعیت شیء را تغییر نمی‌دهند.
    -   **Setters (توابع تنظیم‌کننده)**: متدهایی مانند `setName()` و `setScore()` که برای به‌روزرسانی مقادیر متغیرهای عضو خصوصی استفاده می‌شوند.

---

##  cpp. تحلیل فایل پیاده‌سازی (`Player.cpp`)

این فایل، منطق متدهایی را که در `Player.h` اعلان شده‌اند، پیاده‌سازی می‌کند.

```cpp
#include "Player.h"

// Constructor
Player::Player(const std::string& name, char id, const sf::Color& color, bool isComputer)
    : name(name), id(id), score(0), color(color), computerFlag(isComputer) {}

// --- Getters ---
std::string Player::getName() const {
    return name;
}

char Player::getId() const {
    return id;
}

int Player::getScore() const {
    return score;
}

sf::Color Player::getColor() const {
    return color;
}

bool Player::isComputer() const {
    return computerFlag;
}

// --- Setters ---
void Player::setName(const std::string& newName) {
    name = newName;
}

void Player::setScore(int newScore) {
    score = newScore;
}
```

### بخش‌های مهم کد `Player.cpp`:

-   **`#include "Player.h"`**: این خط کد، فایل هدر `Player.h` را شامل می‌شود تا به تعاریف کلاس دسترسی داشته باشد و بتواند متدهای آن را پیاده‌سازی کند.

-   **پیاده‌سازی سازنده (Constructor Implementation)**:
    ```cpp
    Player::Player(...) : name(name), id(id), score(0), color(color), computerFlag(isComputer) {}
    ```
    -   این سازنده از یک **لیست مقداردهی اولیه عضو (member initializer list)** استفاده می‌کند (بخش بعد از دو نقطه `:`). این روش، کارآمدترین و استانداردترین روش برای مقداردهی اولیه متغیرهای عضو در C++ است.
    -   توجه داشته باشید که `score` همیشه با مقدار `0` شروع می‌شود، زیرا هر بازیکن در ابتدای بازی امتیازی ندارد.

-   **پیاده‌سازی Getters و Setters**: این توابع بسیار ساده هستند و به طور مستقیم مقادیر متغیرهای عضو را برمی‌گردانند یا آن‌ها را با مقادیر جدید جایگزین می‌کنند. این جداسازی به حفظ کپسوله‌سازی (encapsulation) کمک می‌کند.
