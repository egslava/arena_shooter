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

21:50. Решил закоммитить первые изменения:
[+] Skeletons: spinx docs, CMake, SDL, OpenGL 4.2
