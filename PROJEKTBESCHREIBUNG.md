# Projektbeschreibung Wortuhr12-24

## Überblick

Wortuhr12-24 ist ein PlatformIO/Arduino-Projekt für eine Wortuhr mit einer 24 x 12 LED-Matrix. Als Zielplattform ist ein `d1_mini` auf Basis des ESP8266 vorgesehen. Die Uhrzeit kommt über eine DS1307 RTC, die Anzeige erfolgt über eine NeoPixel/NeoMatrix-kompatible LED-Matrix.

Die Wortuhr besitzt ein festes Buchstabenlayout. Einzelne Wörter werden durch Koordinatenfunktionen auf die Matrix gezeichnet. Die Programmlogik entscheidet je nach aktivem Modus, welche Wörter, Zahlen oder Animationen angezeigt werden. Zusätzlich gibt es eine WLAN-Anbindung mit HTML5-Einstellungsseite, ein MQTT-Interface für Node-RED und EEPROM-Persistenz für die wichtigsten Einstellungen.

## Hardware und Bibliotheken

- Controller: WeMos D1 mini / ESP8266
- Matrix: 24 x 12 NeoPixel/NeoMatrix
- RTC: DS1307
- Framework: Arduino über PlatformIO
- Wichtige Bibliotheken:
  - `RTClib`
  - `FastLED`
  - `Adafruit GFX Library`
  - `Adafruit NeoMatrix`
  - `Adafruit NeoPixel`
  - `ESP8266WiFi`
  - `ESP8266WebServer`
  - `PubSubClient`
  - `EEPROM`

## Projektstruktur

- `src/main.cpp`: Hauptprogramm, Setup, Loop, Anzeige- und Moduslogik
- `src/wordclock.h`: zentrale Konstanten, Modus-Defines und gemeinsamer State
- `src/secrets.h`: lokale WLAN- und MQTT-Konfiguration, wird nicht committed
- `src/secrets.example.h`: commitbare Vorlage für `src/secrets.h`
- `src/wordclocklayout.h`: Funktionsdeklarationen für alle darstellbaren Wörter
- `src/wordclocklayout.cpp`: Koordinaten-Mapping der Wörter auf die 24 x 12 Matrix
- `platformio.ini`: PlatformIO-Konfiguration für den `d1_mini`
- `PROJEKTBESCHREIBUNG.md`: diese Projektbeschreibung
- `NETZWERK_MQTT_PLAN.md`: Planungsnotizen zur Netzwerk- und MQTT-Erweiterung

## Anzeigeprinzip

Die Anzeige wird intern als zweidimensionales Grid gespeichert:

- Breite: `WIDTH = 24`
- Höhe: `HEIGHT = 12`
- `State.grid[x][y]` enthält die aktuelle Farbe eines Pixels
- `State.last_grid[x][y]` merkt sich den vorherigen Zustand

Wörter wie `ES IST`, `UHR`, `KAFFEE`, `VIERTEL`, `ZWANZIG` oder Stunden- und Minutenwörter werden durch eigene `draw_*`-Funktionen gesetzt. Diese Funktionen schreiben direkt die passenden Pixelkoordinaten in das Grid. Anschließend wird das Grid auf die NeoMatrix übertragen.

## Hauptmodi

Die Hauptmodi werden über `State.Mode` ausgewählt.

### `MODE_WORD_CLOCK`

Normale Wortuhr-Anzeige. Die aktuelle Implementierung stellt die Zeit wortweise mit Stunde und genauer Minute dar, zum Beispiel sinngemäß:

`ES IST [STUNDE] UND [MINUTE] MINUTEN`

Die genaue sprachliche Darstellung kann später über die Untermodi weiter ausgebaut werden.

### `MODE_WORD_CLOCK_SECONDS`

Sekundenanzeige als Worte. Dieser Modus nutzt die Minuten-Wortlogik, interpretiert den Wert aber als Sekunden. Er eignet sich gut als Testmodus, weil sich die Anzeige jede Sekunde ändert.

### `MODE_WORD_CLOCK_COUNT_DOWN`

Countdown der verbleibenden Sekunden bis zur nächsten vollen Minute. Bei Sekunde 1 wird 59 angezeigt, bei Sekunde 59 wird 1 angezeigt, bei Sekunde 0 wird 0 angezeigt.

### `MODE_WORD_CLOCK_DAYTIME`

Tageszeit-Anzeige in Worten. Vorhandene Wortbausteine deuten auf Ausgaben wie:

- `ES IST MORGEN`
- `ES IST VORMITTAG`
- `ES IST MITTAGS`
- `ES IST PRAEABEND`
- `ES IST ABEND`
- `ES IST NACHT`

Der Modus ist in der Hauptschleife eingebunden. Für `LAYOUT2` werden passende vorhandene Wortfunktionen verwendet. `PRAEABEND` ist als augenzwinkernde Anspielung auf The Big Bang Theory vorgesehen.

### `MODE_WORD_CLOCK_EXACT_DAYTIME`

Wortuhr-Anzeige mit genauer Stunde, genauer Minute und Tageszeit-Zusatz. Beispiel:

`ES IST EIN UHR FUENF UND ZWANZIG IM MITTAG`

Die Tageszeit wird aus der Stunde abgeleitet: Morgen, Vormittag, Mittag, Tag, Präabend oder Abend. Im aktuellen Matrixlayout ist für den Zusatz das Wort `IM` vorhanden; die bestehende Zeichenfunktion heißt historisch `draw_AM`.

### `MODE_MARQUEE_TIME`

Digitale Uhrzeit als Lauftext. Die Zeit wird als Text im Format `Stunde:Minute:Sekunde` über die Matrix geschoben.

### `MODE_MARQUEE_TEXT`

Freier Lauftext. Der Text wird über die Weboberfläche oder über MQTT gesetzt und anschließend über die Matrix geschoben. Der Text wird im EEPROM gespeichert.

### `MODE_BINARY_CLOCK`

Binäruhr. Stunde und Minute werden über große 3 x 5 Darstellungen von `0` und `1` angezeigt.

### `MODE_RESISTOR_COLOR_CODE_CLOCK`

Vorgesehener Modus für eine Uhrzeitdarstellung über Widerstands-Farbcodes. Die genaue Interpretation ist noch offen.

### `MODE_GAME_OF_LIFE`

Animationsmodus für Conway's Game of Life. Lebende Zellen werden in der ausgewählten Anzeigefarbe dargestellt.

### `MODE_GAME_OF_LIFE_AGING`

Variante von Conway's Game of Life mit alternden Pixeln. Lebende Zellen ändern mit ihrem Alter die Farbe von rot über gelb und grün nach blau.

### `MODE_MATRIX_RAIN`

Animationsmodus für einen Matrix-artigen Regeneffekt. Pro Spalte fällt eine Kette aus Pixeln nach unten. Das erste Zeichen ist am hellsten, das zweite deutlich dunkler, danach blendet die Kette stufenweise aus. Kettenlänge, Spaltenreihenfolge und Starthelligkeit variieren zufällig.

### `MODE_TEMPERATURE`

Vorgesehener Modus zur Temperaturanzeige.

### `MODE_RATE_TEMPERATURE`

Vorgesehener Modus zur Bewertung der Temperatur in Worten, zum Beispiel mit Begriffen wie `WARM`, `KALT`, `ZU` oder `GRAD`.

### `MODE_TIME_FOR_FACTS`

Spaß- oder Spruchmodus. In diesem Modus werden zufällige kurze Aussagen angezeigt, zum Beispiel:

- `ES IST ZEIT FUER KAFFEE`
- `ES IST ZEIT FUER BIER`
- `ES IST ZEIT FUER URLAUB`
- `ES IST ZEIT FUER PRAEABEND`
- `ES IST NOCH KAFFEE DA`
- `ES IST NOCH BIER DA`
- `ES IST NOCH WEIN DA`
- `ES IST NOCH EIS DA`
- `NERV NICHT`

### `MODE_DISPLAY_SPECIAL_EVENTS`

Vorgesehener Modus für besondere Ereignisse. Denkbar wären Geburtstage, Feiertage, Urlaub, Wochenende oder frei definierte Anlässe.

## Wortuhr-Untermodi

Die Untermodi werden über `State.Word_Clock_Mode` beschrieben. Sie sind als Varianten der sprachlichen Uhrzeitdarstellung zu verstehen.

### `WORD_CLOCK_MODE_SHORT`

Kurze klassische Wortuhr-Darstellung. Denkbar sind Formen wie:

- `FUENF NACH`
- `VIERTEL NACH`
- `HALB`
- `ZEHN VOR`

### `WORD_CLOCK_MODE_LONG`

Genaue ausgeschriebene Zeit mit Stunde und Minute. Dieser Modus entspricht am ehesten der aktuell vorhandenen Minutenlogik.

### `WORD_CLOCK_MODE_EAST`

Alternative ostdeutsche Zeitdarstellung. Denkbar sind Formen wie:

- `VIERTEL DREI`
- `HALB DREI`
- `DREIVIERTEL DREI`

Die genaue Formulierung sollte noch bewusst festgelegt werden.

### `WORD_CLOCK_MODE_COUNTDOWN_SECOND_HALF_MINUTES`

Darstellung der zweiten Minutenhälfte als Countdown zur nächsten Stunde. Statt `vierzig Minuten nach zwei` wäre zum Beispiel eine Form wie `zwanzig vor drei` denkbar.

## Farbmodi

Die Farbmodi werden über `State.Color_Mode` gewählt.

### `COLOR_MODE_MONO`

Alle aktiven Wörter werden in einer festen Farbe dargestellt. Diese Farbe liegt in `State.Color`.

### `COLOR_MODE_MIXED`

Wörter erhalten wechselnde Farben aus der definierten Farbpalette. Die aktuelle Implementierung wählt eine Farbe aus `colors[]` und vermeidet nach Möglichkeit dieselbe Farbe wie beim vorherigen Wort.

### `COLOR_MODE_RAINBOW`

Regenbogen-Farbmodus. Der Grundmodus ist im Code angebunden und erzeugt wechselnde Farben über ein Farbrad. Die folgenden Varianten sind als eigene Defines vorbereitet:

1. Ganze Matrix

   Die gesamte Matrix erhält einen gemeinsamen Regenbogenverlauf. Die Farbe eines Pixels ergibt sich aus seiner Position und einem globalen Zeit- oder Farbzähler. Wörter erscheinen dadurch als Ausschnitt eines zusammenhängenden Regenbogens. Define: `RAINBOW_MODE_MATRIX`.

2. Verlauf von einer Seite zur anderen

   Der Regenbogen läuft gezielt über eine Achse der Matrix, zum Beispiel von links nach rechts, rechts nach links, oben nach unten oder unten nach oben. Dadurch entsteht ein klar gerichteter Farbverlauf über die aktiven Wörter hinweg. Define: `RAINBOW_MODE_SIDE_GRADIENT`.

3. Unterschiedliche Startpunkte für jedes Wort

   Jedes Wort bekommt einen eigenen Startpunkt im Regenbogen. Dadurch können mehrere Wörter gleichzeitig unterschiedliche Farbbereiche oder Phasen des Regenbogens anzeigen. Optional kann jedes Wort mit leicht versetzter Geschwindigkeit animiert werden. Define: `RAINBOW_MODE_WORD_STARTS`.

## Weboberfläche

Die Uhr startet einen schlanken HTTP-Server auf Port 80. Die Seite `/` liefert eine HTML5-Einstellungsseite aus.

Aktuell einstellbar:

- Hauptmodus
- Farbmodus
- Farbe
- Helligkeit
- Uhrzeit der RTC
- freier Lauftext

Zusätzliche Endpunkte:

- `GET /api/state`: aktueller Zustand als kleines JSON
- `POST /api/save`: aktuelle Einstellungen im EEPROM speichern

Die Weboberfläche speichert Änderungen beim Übernehmen automatisch persistent.

## MQTT

Das MQTT-Interface nutzt `PubSubClient`. Die MQTT-Konfiguration steht in `src/secrets.h`.

Topic-Basis:

`wordclock/wortuhr-12-24`

Die Topic-Basis ist über `MQTT_TOPIC_BASE` konfigurierbar.

Steuer-Topics:

- `<base>/mode/set`
- `<base>/color_mode/set`
- `<base>/color/set`
- `<base>/brightness/set`
- `<base>/text/set`
- `<base>/time/set`
- `<base>/save`
- `<base>/reboot`

Status-Topics:

- `<base>/status`
- `<base>/mode`
- `<base>/color_mode`
- `<base>/color`
- `<base>/brightness`
- `<base>/text`
- `<base>/ip`

Wenn `MQTT_HOST` leer ist, bleibt MQTT deaktiviert. WLAN und Webserver können trotzdem verwendet werden.

## Persistenz

Die wichtigsten Einstellungen werden im EEPROM gespeichert:

- Hauptmodus
- Wortuhr-Untermodus
- Farbmodus
- Rainbow-Variante
- Farbe
- Helligkeit
- freier Lauftext

Beim Start setzt die Firmware zuerst Defaults, lädt anschließend gespeicherte Einstellungen und schreibt bei leerem oder ungültigem EEPROM einmal die Defaults. Die gespeicherte Struktur besitzt Magic- und Versionsfelder, damit ungültige oder alte Daten erkannt werden.

## Lokale Konfiguration

Private Netzwerkdaten liegen in:

`src/secrets.h`

Diese Datei ist in `.gitignore` eingetragen und soll nicht committed werden.

Die Vorlage liegt in:

`src/secrets.example.h`

Für eine neue Installation wird die Vorlage nach `src/secrets.h` kopiert und dort angepasst.

## Aktueller Entwicklungsstand

Bereits umgesetzt oder teilweise umgesetzt:

- normale Wortuhr
- Sekundenanzeige in Worten
- Sekunden-Countdown
- Binäruhr
- Lauftext-Uhrzeit
- freier Lauftext
- zufällige Sprüche im `TIME_FOR_FACTS`-Modus
- Tageszeitmodus
- Game of Life
- Game of Life Aging
- Matrix-Regen
- HTML5-Weboberfläche
- MQTT-Grundinterface
- EEPROM-Persistenz
- lokale `secrets.h` für WLAN/MQTT-Daten

Noch offen oder als Idee vorbereitet:

- vollständige Integration der Wortuhr-Untermodi
- vollständige Umsetzung der Rainbow-Varianten
- Temperaturanzeige
- Temperaturbewertung
- besondere Ereignisse
- Widerstands-Farbcodes
- WLAN-Konfiguration per Access Point
- optional NTP-Zeitsynchronisation

## Hinweise zur Weiterentwicklung

Die Modus-Defines bilden bereits eine gute Roadmap. Sinnvoll wäre als nächster Schritt, die Modi stärker zu strukturieren:

- Hauptmodus-Auswahl zentralisieren
- Wortuhr-Untermodi in eigene Funktionen aufteilen
- Farbmodi weiter von der Zeitlogik trennen
- noch nicht implementierte Modi entweder dokumentiert lassen oder als Platzhalterfunktionen anlegen
- `README.md` später aus dieser Projektbeschreibung ableiten oder auf diese Datei verweisen
