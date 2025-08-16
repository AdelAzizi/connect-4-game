document.addEventListener('DOMContentLoaded', () => {
    const contentElement = document.getElementById('page-content');
    const prevBtn = document.getElementById('prev-btn');
    const nextBtn = document.getElementById('next-btn');
    const progressIndicator = document.getElementById('progress-indicator');

    // --- Final Content Assembly ---
    const tutorialContent = [
        // Chapter 0: Welcome & Overview
        {
            title: 'فصل ۰: خوش‌آمدگویی و نگاه کلی',
            pages: [
                {
                    title: 'معرفی و نگاه کلی',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">به آموزش تعاملی Connect-4 خوش آمدید!</h2><p class="mb-4">در این آموزش، ما قدم به قدم با هم یک بازی کامل Connect-4 را با استفاده از C++ و کتابخانه گرافیکی SFML خواهیم ساخت.</p><div class="my-4 p-4 bg-gray-700 rounded-lg text-center"><p class="font-bold">[انیمیشن گیم‌پلی نهایی در اینجا نمایش داده می‌شود]</p><p class="text-sm text-gray-400">(یک GIF یا انیمیشن CSS از بازی در حال اجرا)</p></div><p>این آموزش بر اساس یادگیری خرد و بصری طراحی شده است. برای شروع، روی دکمه "بعدی" کلیک کنید.</p>`
                },
                {
                    title: 'معماری پروژه',
                    content: `<h2 class="text-2xl font-bold mb-6 text-yellow-400 text-center">معماری پروژه</h2><p class="text-center mb-8">پروژه ما از سه کلاس اصلی تشکیل شده است. روی هر کلاس هاور کنید تا نقش آن را ببینید.</p><div class="flex flex-col items-center space-y-4" dir="ltr"><div class="tooltip border-2 border-blue-400 bg-gray-800 p-4 rounded-lg shadow-lg w-64 text-center interactive-diagram-node"><span class="font-bold text-lg">ConnectFourGame</span><p class="text-sm text-gray-400">ارکستراتور اصلی</p><span class="tooltiptext">کلاس اصلی که حلقه بازی را مدیریت می‌کند، رویدادها را پردازش کرده و کلاس‌های دیگر را هماهنگ می‌کند.</span></div><div class="text-2xl text-blue-400">↓</div><div class="tooltip border-2 border-green-400 bg-gray-800 p-4 rounded-lg shadow-lg w-64 text-center interactive-diagram-node"><span class="font-bold text-lg">GameBoard</span><p class="text-sm text-gray-400">قلب منطق بازی</p><span class="tooltiptext">منطق صفحه بازی را مدیریت می‌کند: قرار دادن مهره، بررسی برد و وضعیت صفحه.</span></div><div class="text-2xl text-green-400">↓</div><div class="tooltip border-2 border-red-400 bg-gray-800 p-4 rounded-lg shadow-lg w-64 text-center interactive-diagram-node"><span class="font-bold text-lg">Player</span><p class="text-sm text-gray-400">داده‌های بازیکن</p><span class="tooltiptext">اطلاعات یک بازیکن را ذخیره می‌کند، مانند نام، امتیاز و اینکه آیا کامپیوتر است یا خیر.</span></div></div>`
                },
                {
                    title: 'آماده‌سازی محیط',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">آماده‌سازی محیط توسعه</h2><p class="mb-6">برای کامپایل و اجرای این پروژه، به \`g++\`, \`make\` و کتابخانه \`SFML\` نیاز دارید. دستورات نصب را برای سیستم‌عامل خود دنبال کنید.</p><div id="tabs" class="w-full"><div class="flex border-b border-gray-600"><button class="tab-button active" onclick="showTab('ubuntu')">اوبونتو/دبیان</button><button class="tab-button" onclick="showTab('windows')">ویندوز</button><button class="tab-button" onclick="showTab('macos')">macOS</button></div><div id="ubuntu" class="tab-content p-4"><h3 class="font-bold mb-2">نصب روی اوبونتو/دبیان</h3><p>دستورات زیر را در ترمینال خود وارد کنید:</p><pre class="code-block mt-2"><code>sudo apt-get update\nsudo apt-get install -y build-essential libsfml-dev</code></pre></div><div id="windows" class="tab-content p-4" style="display:none;"><h3 class="font-bold mb-2">نصب روی ویندوز</h3><p>۱. کامپایلر MinGW (بخشی از ابزار MSYS2) را نصب کنید.</p><p>۲. کتابخانه SFML را از <a href="https://www.sfml-dev.org/download.php" target="_blank" class="text-blue-400 hover:underline">وب‌سایت رسمی</a> دانلود کرده و در مسیر پروژه قرار دهید.</p><p>۳. متغیرهای محیطی را برای کامپایلر و کتابخانه تنظیم کنید.</p></div><div id="macos" class="tab-content p-4" style="display:none;"><h3 class="font-bold mb-2">نصب روی macOS</h3><p>از Homebrew برای نصب استفاده کنید:</p><pre class="code-block mt-2"><code>brew install sfml</code></pre></div></div><style>.tab-button { padding: 10px 20px; border: none; background-color: transparent; color: white; cursor: pointer; border-bottom: 2px solid transparent; } .tab-button.active { border-bottom: 2px solid #facc15; } .tab-content { animation: fadeIn 0.5s; } @keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }</style><script>function showTab(tabName) { ['ubuntu', 'windows', 'macos'].forEach(id => { if(document.getElementById(id)) { document.getElementById(id).style.display = 'none'; document.querySelector(\`button[onclick="showTab('\${id}')"]\`).classList.remove('active'); } }); document.getElementById(tabName).style.display = 'block'; document.querySelector(\`button[onclick="showTab('\${tabName}')"]\`).classList.add('active'); } document.querySelector('.tab-button').classList.add('active'); document.getElementById('ubuntu').style.display = 'block';</script>`
                },
                {
                    title: 'اولین اجرا',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">کامپایل و اجرای اولیه</h2><p class="mb-4">حالا که محیط آماده است، بیایید پروژه را کامپایل و اجرا کنیم تا نتیجه نهایی را ببینید و انگیزه بگیرید!</p><h3 class="text-xl font-semibold mt-6 mb-2">۱. کامپایل پروژه</h3><p>در پوشه اصلی پروژه، دستور \`make\` را اجرا کنید.</p><pre class="code-block mt-2"><code>make</code></pre><h3 class="text-xl font-semibold mt-6 mb-2">۲. اجرای بازی</h3><p>پس از کامپایل، فایل اجرایی را با دستور زیر اجرا کنید:</p><pre class="code-block mt-2"><code>./connect4</code></pre><p class="mt-4">حالا که می‌دانید چه چیزی خواهیم ساخت، بیایید به سراغ اولین قطعه کد برویم!</p>`
                }
            ]
        },
        // Chapter 1: The Player Class
        {
            title: 'فصل ۱: بلوک سازنده اول: کلاس Player',
            pages: [
                {
                    title: 'چرا به کلاس Player نیاز داریم؟',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">چرا به کلاس Player نیاز داریم؟</h2><p class="mb-4">هر بازی‌ای به بازیکن نیاز دارد. در بازی ما، هر بازیکن اطلاعات مشخصی دارد:</p><ul class="list-disc list-inside mb-4 bg-gray-700 p-4 rounded-lg"><li>یک <strong>نام</strong></li><li>یک <strong>شناسه</strong> منحصر به فرد</li><li>یک <strong>امتیاز</strong></li></ul><p>به جای اینکه این داده‌ها را به صورت متغیرهای جداگانه در برنامه پخش کنیم، آن‌ها را در یک کلاس به نام \`Player\` کپسوله می‌کنیم.</p>`
                },
                {
                    title: 'کالبدشکافی Player.h',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">کالبدشکافی فایل هدر: Player.h</h2><p class="mb-4">فایل هدر مانند شناسنامه یک کلاس است. روی هر خط کد هاور کنید تا توضیح آن را ببینید.</p><pre class="code-block text-lg"><code><span class="tooltip"><span class="text-purple-400">#pragma</span> once<span class="tooltiptext">تضمین می‌کند که این فایل فقط یک بار گنجانده شود.</span></span>\n<span class="tooltip"><span class="text-purple-400">#include</span> &lt;string&gt;<span class="tooltiptext">برای استفاده از std::string.</span></span>\n<span class="tooltip"><span class="text-blue-400">class</span> <span class="text-green-400">Player</span> {</span>\n<span class="tooltip"><span class="text-blue-400">private</span>:<span class="tooltiptext">اعضای این بخش فقط توسط خود کلاس قابل دسترسی هستند.</span></span>\n    <span class="tooltip"><span class="text-cyan-400">std::string</span> name;<span class="tooltiptext">نام بازیکن.</span></span>\n<span class="tooltip"><span class="text-blue-400">public</span>:<span class="tooltiptext">رابط عمومی کلاس.</span></span>\n    <span class="tooltip"><span class="text-yellow-300">Player</span>(...);<span class="tooltiptext">سازنده کلاس.</span></span>\n    <span class="tooltip"><span class="text-cyan-400">std::string</span> <span class="text-yellow-300">getName</span>() <span class="text-blue-400">const</span>;<span class="tooltiptext">تابعی که نام را برمی‌گرداند. const یعنی وضعیت کلاس را تغییر نمی‌دهد. 🔒</span></span>\n};</code></pre>`
                },
                {
                    title: 'کالبدشکافی Player.cpp',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">سازنده و لیست مقداردهی اولیه</h2><p class="mb-4">در فایل \`Player.cpp\`، ما از <strong>لیست مقداردهی اولیه عضو</strong> برای مقداردهی متغیرها استفاده می‌کنیم. این روش بهینه و استاندارد است.</p><pre class="code-block mb-4"><code>Player::Player(...) : name(name), id(id), score(0) {}</code></pre><div class="flex justify-center items-center" dir="ltr"><div class="bg-blue-900 p-4 rounded-lg text-center"><p class="font-bold">پارامتر ورودی</p><div class="mt-2 p-2 bg-gray-800 rounded">'Player 1'</div></div><div class="text-2xl text-yellow-400 mx-4">→</div><div class="bg-green-900 p-4 rounded-lg text-center"><p class="font-bold">متغیر عضو</p><div class="mt-2 p-2 bg-gray-800 rounded">this->name</div></div></div>`
                },
                {
                    title: 'آزمون سریع: Player',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">آزمون سریع!</h2><p class="mb-4">کلمه کلیدی \`const\` در انتهای اعلان یک متد چه معنایی دارد؟</p><div id="quiz-container-1"><div class="space-y-3"><button onclick="handleQuiz(this, true)" class="w-full text-right p-3 bg-gray-700 rounded-lg hover:bg-gray-600">الف) متد هیچ یک از متغیرهای عضو کلاس را تغییر نمی‌دهد.</button><button onclick="handleQuiz(this, false)" class="w-full text-right p-3 bg-gray-700 rounded-lg hover:bg-gray-600">ب) متد فقط مقادیر ثابت را برمی‌گرداند.</button></div><p id="feedback-1" class="mt-4 font-bold"></p></div>`
                }
            ]
        },
        // Chapter 2: GameBoard
        {
            title: 'فصل ۲: قلب بازی: کلاس GameBoard',
            pages: [
                {
                    title: 'نمایش بصری صفحه بازی',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">نمایش بصری صفحه بازی</h2><p class="mb-4">صفحه بازی ما یک آرایه دو بعدی \`char board[6][7]\` است.</p><div class="flex justify-center"><div class="grid grid-cols-7 gap-1 bg-blue-600 p-2 rounded-lg" style="width: 300px;">${Array.from({length: 42}).map((_, i) => `<div class="w-10 h-10 bg-gray-800 rounded-full"></div>`).join('')}</div></div>`
                },
                {
                    title: 'منطق انداختن مهره (makeMove)',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">منطق انداختن مهره (makeMove)</h2><p class="mb-4">ما از پایین به بالا اولین خانه خالی را پیدا می‌کنیم. انیمیشن زیر این فرآیند را نشان می‌دهد:</p><div class="flex justify-center"><div class="grid grid-cols-7 gap-1 bg-blue-600 p-2 rounded-lg"><div class="w-12 h-12 bg-gray-800 rounded-full"></div><div class="w-12 h-12 bg-gray-800 rounded-full"><div id="falling-piece" class="w-full h-full rounded-full bg-red-500"></div></div></div></div><style>#falling-piece { animation: fall 1s ease-in forwards; } @keyframes fall { 0% { transform: translateY(-200%); } 100% { transform: translateY(0); } }</style>`
                },
                {
                    title: 'شبیه‌ساز تشخیص برنده',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">شبیه‌ساز تشخیص برنده</h2><p class="mb-4">روی ستون مناسب کلیک کنید تا برنده شوید و انیمیشن بررسی را ببینید.</p><div id="sim-board" class="grid grid-cols-7 gap-1 bg-blue-600 p-2 rounded-lg cursor-pointer">${Array.from({length:42}).map(()=>`<div class="sim-cell"><div class="piece"></div></div>`).join('')}</div><p id="sim-feedback" class="text-center font-bold mt-4"></p><style>.sim-cell {width:40px;height:40px;background-color:#1f2937;border-radius:50%;}.piece.red{background-color:#ef4444;}.highlight-win{background-color:#16a34a!important;}</style><script>if(document.getElementById('sim-board')){document.getElementById('sim-board').onclick=()=>{document.getElementById('sim-feedback').textContent='عالی! ردیف برنده هایلایت شد.';}}</script>`
                }
            ]
        },
        // Chapter 3: ConnectFourGame
        {
            title: 'فصل ۳: ارکستراتور اصلی: کلاس ConnectFourGame',
            pages: [
                {
                    title: 'حلقه اصلی بازی (Game Loop)',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">حلقه اصلی بازی</h2><p class="mb-6">یک چرخه بی‌پایان از: پردازش رویدادها -> به‌روزرسانی منطق -> رندر کردن.</p><div class="flex justify-center items-center p-4" dir="ltr"><div class="loop-box">Events</div><div class="arrow">→</div><div class="loop-box">Update</div><div class="arrow">→</div><div class="loop-box">Render</div></div><style>.loop-box{padding:20px;background-color:#2563eb;border-radius:8px;margin:0 10px;}.arrow{font-size:2rem;}</style>`
                },
                {
                    title: 'مدیریت وضعیت بازی (GameState)',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">مدیریت وضعیت بازی</h2><p class="mb-6">با یک فلوچارت ساده، جریان بین حالت‌های مختلف بازی را می‌بینیم:</p><div class="flex flex-col items-center space-y-2" dir="ltr"><div class="state-box bg-green-600">MAIN_MENU</div><div class="arrow-down">↓</div><div class="state-box bg-blue-600">PLAYING</div><div class="arrow-down">↓</div><div class="state-box bg-red-600">GAME_OVER</div></div><style>.state-box{padding:15px;border-radius:8px;font-weight:bold;}.arrow-down{font-size:2rem;}</style>`
                },
                {
                    title: 'هوش مصنوعی (Minimax)',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">هوش مصنوعی: Minimax</h2><p>الگوریتم Minimax یک درخت تصمیم می‌سازد تا بهترین حرکت را پیدا کند. کامپیوتر (Max) امتیاز را بیشینه و بازیکن (Min) امتیاز را کمینه می‌کند.</p>`
                }
            ]
        },
        // Chapter 4: Wrap-up
        {
            title: 'فصل ۴: جمع‌بندی و گام‌های بعدی',
            pages: [
                {
                    title: 'مرور دستاوردها',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">تبریک می‌گویم!</h2><p class="mb-6">شما تمام مفاهیم اصلی ساخت یک بازی Connect-4 را یاد گرفتید!</p>`
                },
                {
                    title: 'چالش‌های کدنویسی',
                    content: `<h2 class="text-2xl font-bold mb-4 text-yellow-400">چالش‌های کدنویسی</h2><p class="mb-6">حالا نوبت شماست که کد را دستکاری کنید! فایل \`src/Constants.h\` را باز کرده و مقادیر را تغییر دهید.</p><div class="bg-gray-800 p-4 rounded-lg"><h3 class="font-bold text-lg mb-2">چالش: تغییر رنگ مهره‌ها</h3><label for="color-picker">رنگ بازیکن ۱:</label><input type="color" id="color-picker" value="#dc143c"><div id="color-preview" class="w-10 h-10 rounded-full" style="background-color: #dc143c;"></div></div><script>document.getElementById('color-picker')?.addEventListener('input', e => document.getElementById('color-preview').style.backgroundColor = e.target.value);</script>`
                },
                {
                    title: 'پایان',
                    content: `<h2 class="text-2xl font-bold text-center mt-8">موفق باشید و از کدنویسی لذت ببرید!</h2>`
                }
            ]
        }
    ];

    // --- State & LocalStorage ---
    let savedState = JSON.parse(localStorage.getItem('connect4TutorialProgress'));
    let currentChapterIndex = savedState?.chapter || 0;
    let currentPageIndex = savedState?.page || 0;

    function saveProgress() {
        const progress = { chapter: currentChapterIndex, page: currentPageIndex };
        localStorage.setItem('connect4TutorialProgress', JSON.stringify(progress));
    }

    // --- Rendering & Navigation ---
    function renderPage() {
        const chapter = tutorialContent[currentChapterIndex];
        const page = chapter.pages[currentPageIndex];

        contentElement.innerHTML = page.content;
        progressIndicator.textContent = `فصل ${currentChapterIndex} - بخش ${currentPageIndex + 1}`;

        const scripts = contentElement.getElementsByTagName('script');
        for (let i = 0; i < scripts.length; i++) {
            new Function(scripts[i].innerText)();
        }

        prevBtn.disabled = (currentChapterIndex === 0 && currentPageIndex === 0);
        const isLastPage = currentPageIndex === chapter.pages.length - 1;
        const isLastChapter = currentChapterIndex === tutorialContent.length - 1;
        nextBtn.disabled = isLastPage && isLastChapter;

        saveProgress();
    }

    window.handleQuiz = (button, isCorrect) => {
        const feedbackId = button.parentElement.nextElementSibling.id;
        const feedback = document.getElementById(feedbackId);
        button.parentElement.querySelectorAll('button').forEach(btn => btn.disabled = true);
        if (isCorrect) {
            feedback.textContent = '✔️ آفرین! پاسخ صحیح است.';
            feedback.className = 'mt-4 font-bold text-green-400';
            button.classList.add('bg-green-700');
        } else {
            feedback.textContent = '❌ پاسخ اشتباه است.';
            feedback.className = 'mt-4 font-bold text-red-400';
            button.classList.add('bg-red-700');
        }
    };

    function nextPage() {
        const chapter = tutorialContent[currentChapterIndex];
        if (currentPageIndex < chapter.pages.length - 1) {
            currentPageIndex++;
        } else if (currentChapterIndex < tutorialContent.length - 1) {
            currentChapterIndex++;
            currentPageIndex = 0;
        }
        renderPage();
    }

    function prevPage() {
        if (currentPageIndex > 0) {
            currentPageIndex--;
        } else if (currentChapterIndex > 0) {
            currentChapterIndex--;
            currentPageIndex = tutorialContent[currentChapterIndex].pages.length - 1;
        }
        renderPage();
    }

    nextBtn.addEventListener('click', nextPage);
    prevBtn.addEventListener('click', prevPage);

    renderPage();
});
