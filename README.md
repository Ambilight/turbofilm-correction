turbofilm-correction
====================

Простой модуль коррекции названий сериалов по словарю.

Модуль задействует нативное апи v8, чтобы ускорить поиск подходящей коррекции.

Модуль экспериментальный, и написан в худших традициях говнокодинга, но свою задачу он выполняет довольно сносно: скорость поиска коррекции по словарю из 6000 строк выполняется за ~0,4мс, против нескольких секунд чистой javascript-реализации.

Установка
---------
```bash
npm install turbofilm-correction
```

апи
---

```javascript
var tfc = require ('turbofilm-correction');
tfc.add ('house m.d.'); // Добавляем наименование в словарь
tfc.query ('house'); // -> ["house m.d."]
```

Есть еще один момент.
---------------------

Поскольку на начало написания еще не было понятно, как это все должно работать, есть две функции, которые используются следующим образом.
```javascript
tfc.levenshtein ("abc", "abf"); // Расстояние левенштейна между двумя словами
tfc.dice ("def", "xef"); // Коэффициент дайса
```
Обе эти функции реализованиы на c++, поэтому выполняются достаточно быстро, но кроме этого, их (или любые другие функции с двумя строковыми аргументами) можно передавать в функцию query следующими за первым параметрами.
```javascript
tfc.query ('porn', tfc.dice, tfc.levenshtein) // -> [["porno", 0.15, 3],[]...], где 0.15 – коэффициент дайсе, а 3 – расстояние левенштейна.
```