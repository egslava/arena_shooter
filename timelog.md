15/01/2019
12:21-12:50 (0:31) - начинаю учить CMake. CMake всё стёр, прохожу по туториалу ещё раз.
12:50-13:40 (0:50) - дочитал отстойнейший туториал по CMake. Однако, в нём было описано то, как делать тесты: https://www.johnlamp.net/cmake-tutorial-1-getting-started.html
14:53-15:12 (0:19) - продолжаю. Оффтоп на Volatile

15:13-15:38 (0:25) читаю туторы по CMake. Дошёл до step 3. Теперь умею в опциональные фичи (ыыы, круть :) 

17:21-17:36 (0:15) - продолжаю со step3
Кстати, нашёл хорошую доку по OpenGL:
https://learnopengl.com/Getting-started/Hello-Triangle
17:36-18:02 (0:26) - пишу доку по тому, как установить и удалить программу
18:02-19:18 (1:16) - продолжаю туторы

Итого: 3:22

Столкнулся с проблемой:
https://www.gamedev.net/forums/topic/700711-cmake-can-not-find-printf-function/

И героически решил её! :)

16/01/1992
11:22 - 11:40 (0:18). Помимо прочего, удостоверился, что в CMake-либы я могу пихать даже C++-классы. Хотя и без шаблонов =/

11:40-12:45 (1:05) - вчера прошёл 3 шага туториалов по CMake. Продолжаю с 4-го.

Блет, опять, какого-то хера, не находится мой Table.h по туториалу. Хотя я и 
include_directories( ${CMAKE_CURRENT_BINARY_DIR})
Ага. Понял. Просто он находился _два раза_. Сначала в библиотеке .a, а потом в главном файле, который использует эту библиотеку. Из-за этого была коллизия имён.

12:40:13:01 (0:21) - шаг 6. Создание установщика. Done! Получил инсталляторы (правда, без package-manager'ов, для Linux

# 15:39 - опять отвлекли по работе
15:55 - пытаюсь понять, как в cmake сделать более-менее развитую структуру директорий. 

16:11
# <thx to: https://stackoverflow.com/a/51008650/1444191>
file(GLOB source_files
        "*.h"
        "*.cpp"
        "Classes/*/*.cpp"
        "Classes/*/*.h"
)
# </thx to: https://stackoverflow.com/a/51008650/1444191>

23:14-23:30: спасибо этой статье за то, что я прикрутил SDL2 к CMake на Ubuntu. Внезапно, это гораздо проще оказалось, чем я думал и, возможно, проще, чем это будет на Windows.
https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/

00:15-прикручиваю OpenGL 4 по этим туториалам:
http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/

Offtop
```
17/01/2019
1. Выложил тетрадку на гитхаб:
https://github.com/egslava/blood-pressure-insights/

Уведомил челиков, которые занимались схожим:
1. https://github.com/berlogabob/blood_pressure/blob/master/main.ipynb
2. https://github.com/ishanrai05/Blood_Pressure_Analysis/blob/master/Blood_Pressure_Analysis.ipynb
3. https://github.com/mr-ubik/grandpyrents


2. Поднял сервак, который упал из-за обновления Data-центров

3. Скачал базу https://haveibeenpwned.com/Passwords
Скачал скрипт: https://github.com/Landsil/haveibeenpwned-password-check
И узнал, что мои пароли - не поломаны! :)

Расослал ссылку на проект друзьям. Получил фидбек от Серёги Козлова, gr, Кристи, Серёги Сервак, Вовы Гончарова. Поправки от фидбека добавил в issues. Ответил челу, который разрабатывал схожий проект.

Скоро начну заниматься графикой.
```

21:50-22:10 (0:20). Решил закоммитить первые изменения:
[+] Skeletons: spinx docs, CMake (SDL, OpenGL 4.2, Qt, Installer, System Introspection, Configurable files, custom build steps)

22:10 - разрулил сраные конфликты (fatal: refusing to merge unrelated histories) c помощью --allow-unrelated-histories). Начинаю создание VBO и вывода треугольника. Прохожу вот этот тутор:
http://antongerdelan.net/opengl/hellotriangle.html

По-хорошему, код надо бы отрефакторить, т.к. он не безопасен в плане исключений. Но, пока, уверен, что рефактор будет ещё 100 раз, а на утечку ресурсов, в случае полного краша приложения мне пока всё равно.

22:14, ну, блядь, нет функции glGenBuffer и пиздец. И что тут сделаешь? Начинаю опять раскуривать CMake.
22:16, бля, нет, это не ошибка автокомплита, это рили отсутствие ФУНКЦИИ БЛЯДЬ ПОЧЕМУ??? :( Почему glReadBuffer есть, а glGenBuffer, нЕТ БЛЯДЬ НЕТ НЕТ НЕТ НЕТ Нет!!!!!!

Нашёл этот тред. Он длинный и вселяет надежду.
https://www.opengl.org/discussion_boards/showthread.php/172481-glGenBuffer-was-not-declared

Тем не менее, нет. Нужен был GLEW:
1. Установил GLEW: https://www.reddit.com/r/learnprogramming/comments/51u1bg/how_to_install_glew_on_ubuntu/d7ezyxq/
2. Подключил GLEW: https://stackoverflow.com/a/27487567/1444191

Продолжаю инкапсулировать API из тутора в объектики


Допилил треугольник!

18/01/2019
Планы:
[✔]. Коммит
[ ]- Убедиться, что я могу менять содержимое вершинного буфера
2. Цвет.
3. Текстура
4. Коммит


14:20 изменяю VAO, добавляю в него метoд gen()
14:38 ну вот, теперь у меня два VAO: для треугольника и квадрата.
15:09 с помощью чата разобрался, что проблема с VAO/VBO была в моём коде:
```
Не понял. Т.е. это нормально, что glBufferData не даёт никакого эффекта на, уже прибиндинном к VAO VBO?
```