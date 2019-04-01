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
[✔]- Убедиться, что я могу менять содержимое вершинного буфера
[✔]- FPS counter
[✔]- Вынести FPS limiter в отдельный класс какой-нибудь, чтобы, если что, я мог просто получить доступ к fps_limiter.fps
[✔] Цвет.

Рассказали про PowerVR SDK. Решил установить GeoPOD, ShaderEditor, Shaman, TexTool. По идее, это должно решить все мои проблемы.
https://www.opengl.org/sdk/tools/ShaderDesigner/
4. Коммит


14:20 изменяю VAO, добавляю в него метoд gen()
14:38 ну вот, теперь у меня два VAO: для треугольника и квадрата.
15:09 с помощью чата разобрался, что проблема с VAO/VBO была в моём коде:
```
Не понял. Т.е. это нормально, что glBufferData не даёт никакого эффекта на, уже прибиндинном к VAO VBO?
```


Интересно, не влияет ли то, что мы, для замера FPS, каждый кадр замеряем время? Что лучше: выводить FPS каждый 10000 кадров или каждые 5 секунд?

```python
i_frame = np.array([ 7257.089764, 7294.456749, 7299.757905, 7311.015123, 7347.114725, 7356.196245, 7318.929439, 7322.169452, 7345.169458, 7358.104190, 7312.465005, 7336.359472, 7329.605835, 7318.809467, 7341.746185, 7354.473643, 7326.328595, 7279.033891, 7351.528132, 7351.559289, 7346.012531, 7278.896156, 7339.218926, 7348.697482, 7362.340075, 7318.384154, 7291.326407 ])

i_time = np.array([ 6995.110110, 7299.494899, 7308.149814, 7398.067636, 7381.499999, 7365.127402, 7319.085883, 7394.085145, 7390.828112, 7394.401505, 7394.934880, 7336.806991, 7382.509019, 7346.422101, 7393.727637, 7398.792390, 7392.721745, 7342.236305, 7378.638727, 7400.804726 ])

i_frame.sort()
i_time.sort()

i_frame[3:-3].mean(), i_frame[3:-3].std(), i_time[3:-3].mean(), i_time[3:-3].std()                                                              
Out[9]: (7328.815617809524, 19.13524908895329, 7372.358960785715, 24.842816498519667)
```

Т.е. разницы нет почти.

16:45. Вернулся из магаза. Подумал, что есть смысл замерять не только FPS, но и SPF, т.к. оно меняется пропорционально тормозухам в коде. Типа SPF увеличился на 5 мс, значит, новый участок кода отрабатывает, примерно, 5 мс.

21:14 - 21:35  (0:21) - перевёл письмо-обращение к иностранному коллеги по поводу анализа данных с тонометра.



Я не понимаю, допустим, если я делаю некий MyVBO, который должен цепляться к MyVAO, то как я должен управлять временем жизни всех этих объектов? С одной стороны, хотелось бы, что, если VAO сдох, то он уничтожил и прилинкованные к нему буфера, если они уже не используются. Т.е. надо в неком "VAO" хранить std::vector VBO и, соответственно, когда я передаю VBO в VAO, то это должно происходить с std::move. 

Но есть ещё одна мысль: каждый ресурс сам управляет вренем своей жизни, а всякие VAO хранят список из shared_ptr на VBO.


Произвольное количество VBO в VAO
=================================
---
Короч, тупо сделал хардкод на два вбо
---
Но более правильное решение: сделать inline-функцию с variadic templates. Тогда только эта функция не будет в либе, а оставшиеся - будут! :)


Forward declaration
===================
Почему такая конструкция работает:
```cpp

struct A;
struct B;

struct A {
    B getb();
};

struct B {
    A geta();
};

A B::geta(){
    std::cout << "B::geta()" << std::endl;
    return A();
}

B A::getb(){
    std::cout << "A::getb()" << std::endl;
    return B();
}
```

А такая - нет:
```cpp
struct A;
struct B;

struct A {
    B getb(){
        std::cout << "A::getb()" << endl;
        return B();
    }
};

struct B {
    A geta(){
        std::cout << "B::geta()" << endl;
        return A();
    }
};
```

19/01/2019
[ ] Текстура
[✔] а. Использовать готовый loader из туториала и сосредоточиться на текст. координатах и шейдере.
[ ] b. Забомбить лоадер pvr: http://cdn.imgtec.com/sdk-documentation/PVR+File+Format.Specification.pdf
3. Текстуры: 
1. Loader: https://github.com/paroj/nv_dds/blob/master/nv_dds.cpp
2. png -> dds: http://manpages.ubuntu.com/manpages/bionic/man1/nvcompress.1.html
3. cubemaps: http://manpages.ubuntu.com/manpages/xenial/man1/nvassemble.1.html



https://vk.com/dexbee держит:
A&D UA-911 BT
Medisana 300 Connect
Medisana BU 575 Connect
Koogeek BP1
Koogeek BP2

Готовый драйвер/устройство:https://github.com/mfussi/bpmonitor

Предсказания:
https://github.com/arijit1410/LSTM-TimeSeries

Чел пропатчил свой старый тонометр: https://github.com/edusteinhorst/WiFiBloodPressureMonitor

Чел рассказывает про реверс-инжиниринг девайса: https://github.com/atbrask/BM65DataDownloader

Чел тырит данные с Omron под OS X: https://github.com/seanwhitsell/Pressure

Omron: https://github.com/LazyT/obpm/issues?q=is%3Aopen+is%3Aissue
В принципе, можно брать данные раз в 1-2 недели и всё ок. Хотя, конечно, и не так удобно.

Предикторы
----------
https://github.com/shawnlxh/Blood_Pressure_Prediction



23/01/2019
Билдю проект под Windows
13:00-13:14 - спасибо челу: https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/, благодаря которому я прикрутил SDL2 под Windows.
13:14-13:41 - (прикручиваю GLEW, но нет). Оказалось, что надо было добавить ещё вот эту строчку для SDL2: `set(SDL2_DIR ${SDL2_DIR}  "${CMAKE_SOURCE_DIR}/libs/SDL2/win_x86_64/SDL2-2.0.9")`.

Скачал вот этот файл:
https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindGLEW.cmake

13:41-14:04. Указал пути к GLEW вручную :'( Но, вроде как, оно собирается и генерит солюшн. Интересно, как его теперь запустить из под консоли?

Выкинул glext.h, ибо не использовал, а под Windows его надо ещё найти..

14:33 - под Windows SDL не устраивает модификатор const в main'е: `int main (int argc, const char *argv[])`. Пример правильной сигнатуры: `int main (int argc, char *argv[])`

glew32.dll is missing. Нужно найти

15:18 - ура, я запустил проект. Подтянулись текстуры, мой треугольник запустился. Текущие проблемы:
1. Нужно самому копировать все dll'ки в папку с exe. Видимо, надо сделать build-task, который будет копировать нужные dll'ки.
2. Папка с ресурсами каждый раз разная. Видимо, надо сделать таск, который будет копировать папку с ресурсами в нужные места.

16:43 - так и не скопировал. Пошёл забирать посылки.


add_executable(Arena ${source_files} )
target_link_libraries(Arena ${EXTRA_LIBS})
 
add_custom_command(TARGET Arena PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${Arena_RES_DIR}
    ${PROJECT_BINARY_DIR}
)


24/09/2019
10:20 - короч, проект так и не запускается из-за того, что ресы не копируются в нужную папку. Блин, я даже вопрос в группе не задал =( Нет, я задавал, но ответ не получил.


 C:/Program Files/CMake/bin/cmake.exe;-E;copy_if_different;C:/Users/Viacheslav/my/arena_shooter/client/libs/GLEW/win32/glew-2.1.0/bin/Release/Win32/glew32.dll;C:/Users/Viacheslav/my/arena_shooter/client/build/glew32.dll


add_custom_target(
    "copy-dyn-libs" ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${res_file}"
        "$<TARGET_FILE_DIR:Arena>/res/"
        
) 


add_custom_target(
    "copy-dyn-libs" ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        ${SDL2_DYNLIB_FROM}
        ${SDL2_DYNLIB_TO}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        ${GLEW_DYNLIB_FROM}
        ${GLEW_DYNLIB_TO}
)                 # <--this is out-file path


file(GLOB res_files "res/*")
foreach(res_file ${res_files})
    message ("${res_file}")
    add_custom_command(
        TARGET copy-dyn-libs
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  
            "${res_file}"
            "$<TARGET_FILE_DIR:Arena>/res/"
    
    )      
endforeach()

Занимался до, примерно 18-19, потом ушёл играть в MineCraft.

25/01/2019 13:50 - опять тут

26/01/2019 - ехал из города в город. В поезде написал небольшой трансформер из obj в свой формат, пока не дописан.
27/01/2019 11:05 - продолжаю писать кодировщик.


/home/egslava/my/arena-shooter/build-client-Desktop_Qt_5_10_1_GCC_64bit-Debug/res
/home/egslava/my/arena-shooter/build-client-Desktop_Qt_5_10_1_GCC_64bit-Debug/

28/01/2019 - спасибо ребятам из pro.graphon (mrshoor): когда написал свой кодировщик, была проблема  с Depth-буфером (из-за непонимания проекции) + была проблема с Z-up, Y-forward в Blender'е + мне нужно было флипать изображение по вертикали. Порешали. Ура! Теперь домик грузится! :)

Коммит по доке:
```
//1. Задокументировать, как именно экспортировать в obj:
// a. Apply modifiers
// b. Write normals
// c. Include UVs
// d. Triangulate faces
// e. Keep vertex order
// Y forward, Z - up
//2. Задокументировать, что текстура должна быть флипнутой
//3.mesh = bpy.data.meshes[0]; for v in mesh.vertices: v.co
// 4. помог Alexandr Busarov (mrshoor)
```


Коммит по коду:
```
[+] A 3D model loader and converter from .obj to the inner format. A model of a warehouse is not mine (thanks to: Author/Autor: Gunnar Correa - www.gunnarcorrea.com and SatellaSoft: http://www.satellasoft.com )
[x] PVR loader couldn't load just a texture
[x] All the textures are flipped now
x Textures are also in std::vector now, so every mesh is having its own texture
x added a comment for the build-task 'copy-resources', so, now, it's a bit more clear.
```


29/01/2019
----------
Хочу сделать так, чтобы в директории res можно было использовать поддиректории:
/res/hero/model.obj
/res/hero/texture.png

И т.п.

17:00 - сделал.
17:37 - хочу сделать автоматическое преобразование obj-модели в .model через Python'овский скрипт во время билда.
05:26 - модель автоматически преобразуется, причём только в том случае, если .obj новее сгенерированного файла, либо обновился код Python-конвертера.


30/01/2019
----------
Ок, хочу сделать передвижение по миру. Камера будет летать, скорее всего, завтра, а сегодня я хочу просто повертеть модельки, вокруг Y.

19:04 - начал писать класс для работы с матрицами Mat4x4.
21:26 - модельки крутятся :)

06:59 - я могу перемещаться по 3d-пространству: могу крутить предметы мышой и менять своё положение клавиатурой. Я даже могу управлять зумом посредством скролла. Тем не менее, перспективное преобразование не работает..

Сегодня я ещё узнал, что uniform-матрицы нужно передавать транспонированными.


04/02/2019
----------
Доделал перемещение по 3D-миру. До этого сделал так, что перемещаться было можно, но я сделал это случайно и не понимал решение полностью. Сейчас сделал всё осознанно, попутно реализовав классы по работе с матрицами и векторами. Камера тоже имеет внятный интерфейс сейчас.


09/02/2019
----------
Камера умеет перемещаться по миру, разрешая коллизии. Тем не менее, всё равно, на стыке нескольких плоскостей получается убого. Наверное, надо искать расстояние до треугольников и, уже, исходить из него.


10/02/2019
----------
Сделал в Blender'е Full Illumination и загрузил его в игру. Кроме того, сделал простенькую Cube Map (не на Cube Map Sampler'ах, а просто обычная 2d-текстура, нанесённая на грани куба). Качество картинки мне начинает нравится. Но надо научиться импортировать целые сцены.

1. Задокументировать, что PVRTexToolCLI должна быть добавлена в path:
/opt/Imagination/PowerVR_Graphics/PowerVR_Tools/PVRTexTool/CLI/Linux_x86_64

2. Добавить конвертацию текстур в CMake. Ура-ура!
3. В Blender'е сделать автоматический экспорт ВСЕХ моделей + всех текстур в папку. Т.о. должно быть: res/models/hero/models_and_textures


12/02/2019
----------
Ничего не сделал из вышесказанного. По 6 часов рендерил lightmaps + сделал набросок уровня. Планируется иметь телепорт :) Динамическое освещение и частицы. Декали - не нужны (ибо нет пуль). Ура-ура.

Примерный план:
-- Хождение по уровню --
[v] Ускорение
[б.] Торможение.
в. Гравитация (через Physics Pass)
г. Забрание на ступеньки
д. Прыжки
Телепортация через середину (кристалл)
-- Asset Pipeline --
[✓] Автоматическая конвертация текстур в PVR + занести это в доки. 
[✓] Экспорт моделей в obj автоматом.
[ ] Генерация необходимого кода и вывод его через println в консоль Питона.
-- Динамическое освещение --
1. Чтобы получить точки для фрустум куллинга, нужно взять normalized device cube и умножить каждую его точку на inverse(projection_matrix*view_matrix): 
2. В частном случае, когда сфера меньше view frustum'а, сфера находится в фрустуме, если:
а. Её центр находится во фрустуме
б. Точка, лежащая на пересечении луча из центра сферы по направлению к камере, находится во фрустуме.
3. Источники света нужно сортировать по расстоянию до камеры и оставлять 4-е ближайших.
4. Кроме того, мы можем включать/выключать источники света ДЛЯ КАЖДОГО объекта, если расстояние между объектом и источником света меньше заданного. Получается, что у нас будет по 4 источника света на ОБЪЕКТ! Яху! Это выглядит гораздо более многообещающе с мелкими источниками света.
-- Particles --
-- Персонаж --
а. Количество жизней
б. Получение урона (+ откуда именно прилетел?)
б. Смерть (заваливание камеры?)
в. Респаун?
-- Мобы --
-- Логика уровня --
Телепорт
Респавн
Анимации
-- Звук --
Ох, как бы хотелось звуки :-/

2. Прикрутить динамическое освещение.

Профессии появляются и пропадают.


Активные вопросы:
1. Поддерживает ли obj несколько текстурных координат? Цветов, прочего.
- @mrshoor ответил, что нет, не поддерживает.
2. 

14/02/2019
----------
Прикрутил автоматиескую конвертацию текстур в PVR. Было много гемора с CMake: он не поддерживает generator expressions в OUTPUT к add_custom_command. Сделал gist: https://gist.github.com/egslava/7333c2aa1cbda9cb0a965ee41512056d


15/02/2019
----------
Да, чтобы иметь свет, нужно иметь возможность Frustum Culling'а. Но реализовать его проще, сначала, на том, что уже есть - на моделях. Значит, должен ввести всякие классы Model/Light/Node/Scene и прочее и реализовать Frustum Culling. Это - план на сегодня.


16/02/2019
----------
cmake не держит возможность задавать generator expressions в add_custom_command OUTPUT. Поборол это, выложил gist на github'e 


18/02/2019
----------
Разумеется, Frustum Culling'а всё ещё нет :) Пока что непонятки в голове. Сама сцена сделана, я имею доступ ко всем объектам и думаю о способе их организации. Да, кстати, теперь есть некое подобие гравитации, но отстойное, т.к. честного ускорения нет. Несколько Collision Pass'ов - и у меня уже начинаются тормоза. Надо делать BSpheres и Frustum Culling.

[+] Acceleration with shift
[+] Shitty gravity and several collision passes
[+] Backface culling
[+] Material color
[+] Counting FPS without taking into an account double bufferization
[-] cleaned arena.cpp code (comments)


19/02/2019
----------
Я, тормоз, не понимаю даже, как сделать гравитацию с ускорением. Точнее, то, как обработать падение и сбросить скорость персонажа на ноль. Пока что, из возможных вариантов, кажется, что наиболее правильный - искать коллизию и, если угол между нормалью к поверхности и вектором "вниз" меньше заданного, то, считать, что персонаж должен закончить "падение".

Да, практика показала, что на 10 collision pass'ах у меня уже возникают проблемы с производительностью. А персонаж у меня пока только один. Вывод: оптимизации нужны! Срочно за создание BSpheres!


21/02/2019
----------
App level:
1. Added wireframe mode
2. Collisions are nicer: less 'sharp' jumps, though they still are, but I'm satisfied for now.

Code level:
[+] Frustum Culling: Sphere and Frustum. Intersection detection.
[+] math: Line segment code, closest points to a Line, Segment, Plane and Triangle.
[+] debug: bounding spheres, Wireframe mode is on 'z' key. It turns on wireframe mode and add more ambient light.
[x] RUN_TESTS cmake constant
[x] A bit more bright scene for debugging (flower)
[x] Triangles collapsed into a line are not a problem for collision solver now.


22/02/2019
----------
App:
[+] Non-final (but bug-free) particle system with Billboard computation on GPU. Still doesn't support texture animation, color interpolation and so on. Just a quad.

Code:
[+] Reinhard's tonemapping. Commented it out, since didn't really get the point of it.
[+] Random: max/min, from Ball


24/02/2019
----------
[F] КВАДЫ вместо трианглов - фейл. Больше не поддерживаются.
[v] добавить текстуры

25/02/2019
----------
[v] добавить в сцену несколько спецэффектов.
[v] добавить настраиваемые цвета

26/02/2019
----------
[v] Почему-то частицы начинают своё движение каждый раз заного, когда я открываю аппу, вместо того, чтобы начинать с рандомного состояния.

27/02/2019
----------
[v] Добавить настраиваемое изменение размера
[v] Доделать взрыв: частицы не должны начинать респавниться до тех пор, пока снова не вызовем метод respawn.
[ ] добавить texture frames, чтобы было, хотя бы, партикла 2-4 для огня
[ ] Frustum Culling (Particles -> Triangles -> BBox -> BBall)

28/02/2019
----------
[v] добавить вращение
Commit:
[+] Particle system
[+] Better bakes and better triangulated meshes
[x] Navigation and gravity ARE FINE NOW
[+] The complete level geometry
[ ] fix size
[v] emitter shouuld have acceleratoin in uniform. For explosions we need an acceleration, but for enemies - not.

28/02/2019 - 03/03/2019
-----------------------
[+] Multisampling
[+] Jumps 
[+] Bullets


03/03/2019
----------
Итак, пули у меня уже летали, я отправил видео с выстрелами. Но пока нет мобов. Два-три дня я тупил и пытался думать, как же мне сделать поиск пути по карте (для мобов). Варианты из головы:
1. nav mesh. Но он: плохо документирован (если только речь идёт не про использование его во всяких движках типа Unity/Unreal)
2. way points. Из разговора с Dan'ом, я так понял, что это, типа, когда мы делаем 3d-карту дискретной и ищем путь волновым алгоритмом. Это слишком высокая сложность, в моём случае, т.к. у меня высокие прыжки. Это значит, что, из одной точки мы можем допрыгнуть очень много куда. Но есть места, куда и не можем. Соответственно, алгоритм "переть напрямую" может давать серьёзные затупы, а алгоритмы вида "найти кратчайший путь" имеют слишком большое пространство состояний.

В общем, в итоге, решил создать некоторые "машруты", которые будут односторонними и заранее записанными. Например, как подняться наверх: сначала по лесенке, потом на кристалл, потом прыжок. 

Но надо ещё понять: надо оно или нет. Ведь, вообще говоря, мобу не надо подходить вплотную. Он может спокойно себе стрелять в игрока из любой позиции, а бегать рандомно, как будто он "уклоняется".

Поэтому сегодня-завтра надо постараться сделать настоящие пули.


07/03/2019
----------
1. Пули взрываются о препятствия. Перестаёт лететь огонь. Он тухнет и получается дым.

08/03/2019
----------
Так, всё. Надо делать противников :) Возможно, сначала надо сделать телепорт внутри кристалла.

09/03/2019
----------
[+] Телепорт-кристалл сделан. Работает на меня и на противника.
[+] "Противник" за мной бегает, его можно "убить" - он переродится в случайном месте карты.


Надо:
1. Анимация телепорта
[v] Противник должен смекать, что надо идти к кристаллу. Возможно, если я на платформе или на цветке. Т.е. если высота больше заданной!
Интересно то, как эволюционировались и упрощались идеи о ИИ в игре :) 
1) Сначала я думал о поиске пути, выбирая между навмешем и вей-поинтами. 
2) Потом думал о том, что можно просто "переть напролом", и смотреть: может ли бот дойти до игрока или он встрянет (расстояние между игроком и ботом перестанет "сокращаться"). В таком случае, на каждый кадр пришлось бы делать довольно много интегрирований и, имхо, это лагало бы (попробовал в игре увеличить число интеграций от 1 до 100 и начались тормоза в дебаг-моде).
3) В итоге понял, что бот тупой: не может карабкаться. Т.е. он должен идти к кристаллу, только в том случае, если игрок значительно выше бота. Вот и всё! :) Проверить дельту по z.

3. Противник должен уметь ползать "по кругу" и не падать с него. Делаю. Аналогично, он должен не падать с "розочки". Решил сделать условно включаемыми (по высоте) "отталкивателями".
4. Противник должен уметь "запрыгивать"
4. Прицел
5. Много противников. Должны нарастать "волнами"

11/03/2019
----------
[+] Enemy is running
[+] Enemy can be killed
[+] Enemy can walk around imaginary spheres to avoid falling from the flower
[+] enemy jumps if it has stucked
[+] Enemy can fire! :)
[+] Nebula is rotating!
[x] Changed teleportation: now it jumps straigh on the flower, not higher

Code:
[-] Avoid clearing color buffer for now - it's not needed
[x] Mat4x4::set_rot_z is fixed now
[+] Camera::look_at
[+] set_model_mat / set_camera_mat
[+] Node::Flags::APPLY_TRANSFORMS / Node::Flags::SCREEN_COORDS
[+] added a world-axes in debug mode and local-axes in debug mode as well

Наконец-то работает метод look_at. Было очень проблеммно его сделать. Простой перевод из декартовых координат в сферические не работает, нужен ещё костыль.
Вот просто пример перевода координат:
rotX = acos(y)
rotY = atan2(x,z)

А вот реально работающий код:
rot_X = -(acos(y) - M_PI/2);
rot_Y = atan2(x,z) - M_PI;


13/03/2019
----------
[+] Bugfix when everything goes underground because of the slow initialization
[+] Gameover. Because of the falling down or due to a shot
[+] Bullets explode when bump into each other
[+] Enemies respawn on some distance on player

14/03/2019
----------
[+] Enemies' bullets do not kill enemies
[+] Enemies grow more gracefully (50% chance after killing each enemy)
[+] Bullets appear closer to a shooter
[+] Exploded bullets (smoke) do not kill anyone
[+] Enemies are more 'spread' of each other. Thus, they don't form a line / point when they stalk the player.
[+] One more time: Bugfix when everything goes underground because of the slow initialization

- Надо сделать, чтобы враги отталкивались друг от друга

const float _angles[6*2] = float[6*2](
    45, 135, -135, 
    -135, -45, 45,
    135, 45, -45, 
    -45, -135, 135 );



12/03/2019
----------
[x] Больше нельзя оттолкнуться от потолка
[+] Прицел
Небула должна рисоваться в последнюю очередь
Частицы можно дискардить с помощью бекфейс-куллинга

Ща буду делать респавн перса.

04/01/2019
----------

[v] Better compilation speed (moved screen_width & screen_height outside of a header file)
1. enemy bug A: new enemies do not appear
1. enemy bug B: enemies do not have a model
2. press any key




Credits:
Графика:
# @mrshoor - Alexander Busarov - подсказал мне, почему у меня была перевёрнутая модель в Blender'е, а так же рассказал, что в вершинном шейдере выходная xyzw, z записывает в z-buffer, поэтому у меня z-buffer работал, но отрисовывалась модель неправильно (нужна матрица проекции).

# @alprog - Александр Тужик. Объяснил про то, что текстуру надо загружать перевёрнутой в OpenGL. А вместе с @mrshoor они доспорились до того, что я понял, что в текстурных координатах 0,0 всегда будет лежать то, что лежало в самом начале и в D3D и в OpenGL, несмотря на то, что в этих GAPI разные текстурные координаты и разные требования по порядку байт при загрузке текстур.

CMake:
# @Artalus (Igor Ivanov) и @egorpugin (Egor Pugin). Помогли портировать на Windows, объяснили про Generator Expressions. Сказали, что создать цель: copy-res, которая бы не устаревала постоянно, невозможно.
@megaxela - Alex Ushanov. Подсказал, как добавить поддиректории в res.
@DenisKormalev объяснил, что target_sources - это не только .cpp-файлы, а, вообще все зависимости проекта. А давать ли их на вход компилятору или нет - это уже решает CMake, в засимости от используемого языка программирования (должно настраиваться).

