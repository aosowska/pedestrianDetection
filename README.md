# pedestrianDetection

Celem aplikacji jest zliczanie przechodniów.

Program zlicza przechodniów przekraczających dwie skrajne linie oraz podaje liczbę już obecnych na obserwowanym polu - osób nie przekraczających linii.

Sposób opiera się na detekcji ruchu korzystając z metody izolowania tła (backGroundSubstraction). Odpowiednie usunięcie zakłóceń i progowanie wyklarowuje nam obraz do śledzenia postaci.

Funkcja licząca sprawdza odległość momentu ciężkości ruchomego elementu względem lini - In oraz Out.

Po skończeniu filmu program wyświetla ramke zawierającą statystyki.
