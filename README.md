# Space Invaders
##### Opis: Projekt na algorytmikę i struktury danych.
- [ ] Czy Osoba Prowadząca Zajęcia pozwoliła użyć kodu da zaliczeń innych przedmiotów

> [!WARNING]
> Nie odpowiadam za bule głowy spowodowane nazewnictwem zmiennych, ortografią oraz interpunkcją.

Jaki kod jest każdy widzi. Więc opisze ciekawe mechanizmy, jakie użyłem w kodzie.<br/>

> [!IMPORTANT]
> Pierwsze co trzeba wyjaśnić to, czemu wszystkie pliki są `.h`. Jest tak dlatego,
> że w większość plików używam `template`[^1] a funkcje szablonowe muszą być w plikach nagłówkowych,
> ponieważ to dynamiczna cześć języka i nie można jej skompilować bez wiedzy, w jaki sposób chcemy jej użyć.

> [!IMPORTANT]
> w pliku [CMakeLists.txt](CMakeLists.txt) znajdują się informacje:
> 1. wersja cpp `set(CMAKE_CXX_STANDARD 23)`.
> 2. biblioteki, które użyłem.
>    - [`https://github.com/SFML/SFML.git`](https://www.sfml-dev.org/).
>    - [`https://github.com/marzer/tomlplusplus.git`](https://marzer.github.io/tomlplusplus/).

## Concepts[^2]
Wszystkie koncepty, które napisałem, znajdują się w [concepts.h](concepts.h).
Pierwszy problem, który rozwiązałem za pomocą konceptów to rozwiązanie skomplikowanych `#include`.
Ponieważ w pliku [Player.h](Player.h) chciałem includować [Robot.h](Robot.h) i odwrotnie co stwarzało problem przez **header guard**[^3]


[^1]:[template](https://en.cppreference.com/w/cpp/language/templates.html)
[^2]:[Concepts](https://en.cppreference.com/w/cpp/concepts.html)
[^3]:header guard występuje na początku pliki i dba, żeby plik nie wkleił sie dwa razy. 
```c++
#ifndef SPACE_INVADERS_ROBOT_H
#define SPACE_INVADERS_ROBOT_H
```

