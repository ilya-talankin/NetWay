# NetWay
Репозиторий команды по правую руку

## Что работает: 
Клиент может подключаться к нескольким серверам
Сервер может слушать и обрабатывать несколько клиентов
Клиент и сервер при подключении жмут дург другу руки (говорят свой ID) и записывают отпечаток ладони в таблицу
Клиент отвечает на запрос КА (кип элайв) рукопожатием (но его пока никто не инициирует)
Узел ретрансляции ретранслирует согласно маршруту

### Формат обмена:
Узлы обмениваются строками следующего вида:
TYPE@[CONTENT]#N1#N2#N3

TYPE:  
KA - keep alive  
      SRCH - поиск кратчайшего пути (передаётся по всем узлам)  
      PKG - пакет (передаётся строго по маршруту)  
      RET - возврат кратчайшего пути или ошибки передачи
      
 N - id узлов, через которые нужно передать  
 
 На данные момент передаются PKG и SRCH

## Что не сделано:
Почти всё))) 
- Поиск кратчайшего пути
- Переодичные запросы keep alive
- Нет приёмника и передатчика 
- и ещё много всего
