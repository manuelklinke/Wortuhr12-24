# Netzwerk- und MQTT-Plan

## Einschätzung

Eine WLAN-Anbindung mit kleinem Webserver und MQTT-Interface ist auf dem D1 mini / ESP8266 realistisch machbar.

Der aktuelle Build liegt bei ungefähr:

- Flash: 283423 von 1044464 Bytes, etwa 27 %
- RAM: 29976 von 81920 Bytes, etwa 37 %

Damit ist noch ausreichend Platz für WLAN, einen einfachen Webserver, MQTT und Einstellungslogik vorhanden. Wichtig ist, die Umsetzung bewusst schlank zu halten:

- kein schweres Webframework
- kein TLS/MQTT over SSL auf dem ESP8266
- keine großen JSON-Dokumente
- keine langen blockierenden Animationen
- feste Puffer statt vieler dynamischer `String`-Operationen

Die LED-Matrix hat 24 x 12 = 288 LEDs. Ein `matrix.show()` blockiert kurzzeitig, weil NeoPixel-Daten timingkritisch ausgegeben werden. Das ist bei 288 LEDs noch gut handhabbar, aber Webserver und MQTT müssen regelmäßig in der `loop()` bedient werden.

## Zielbild

Die Uhr soll drei Bedienwege bekommen:

1. Lokale Anzeige auf der LED-Matrix
2. Weboberfläche für Einstellungen
3. MQTT-Interface für Node-RED

Alle Wege greifen auf denselben zentralen Zustand zu. Änderungen über Web oder MQTT wirken sofort auf die Anzeige.

## Vorgeschlagene Bibliotheken

### WLAN

Arduino-ESP8266 Standardbibliotheken:

- `ESP8266WiFi.h`
- optional `ESP8266mDNS.h`

### Webserver

Empfohlen:

- `ESP8266WebServer.h`

Begründung:

- Teil des ESP8266-Arduino-Ökosystems
- klein genug für dieses Projekt
- einfache Formulare und REST-artige Endpunkte möglich

Nicht empfohlen für den ersten Schritt:

- Async Webserver
- WebSockets
- große JavaScript-Frontends

Diese Varianten sind möglich, erhöhen aber RAM- und Komplexitätsverbrauch.

### MQTT

Empfohlen:

- `PubSubClient`

Begründung:

- etabliert und klein
- ausreichend für Node-RED-Steuerung
- einfache Topic-Struktur

### Persistenz

Für Einstellungen:

- zunächst `EEPROM`
- später optional `LittleFS`, wenn mehr Text oder mehrere Profile gespeichert werden sollen

EEPROM reicht für:

- Modus
- Farbmodus
- Farbe
- Helligkeit
- MQTT/WLAN-Optionen
- kurzer freier Lauftext

## Zentrale Architektur

Der aktuelle `State` sollte um einen Konfigurationszustand ergänzt werden. Sinnvoll wäre eine Trennung zwischen flüchtigem Anzeigezustand und gespeicherter Konfiguration.

Beispielhafte Struktur:

```cpp
typedef struct
{
  uint8_t mode;
  uint8_t wordClockMode;
  uint8_t colorMode;
  uint16_t color;
  uint8_t brightnessDay;
  uint8_t brightnessNight;
  char marqueeText[96];
  bool mqttEnabled;
} configType_t;
```

Der bestehende `stateType_t` bleibt für laufende Anzeigezustände zuständig:

- aktueller Modus
- aktueller Farbmodus
- Grid
- vorheriges Grid
- letzte Farbe
- Animation-Counter

Langfristig könnte `State.Mode`, `State.Word_Clock_Mode`, `State.Color_Mode` und `State.Color` aus der gespeicherten Konfiguration initialisiert werden.

## Weboberfläche

Die Weboberfläche sollte eine einzige Einstellungsseite bekommen.

### Seite `/`

Funktionen:

- aktuellen Modus auswählen
- Wortuhr-Untermodus auswählen
- Uhrzeit setzen
- Farbe auswählen
- Farbmodus auswählen
- Helligkeit setzen
- freien Lauftext eingeben
- MQTT-Verbindungsstatus anzeigen
- Speichern/Übernehmen

Die Seite kann als einfaches HTML-Formular direkt aus dem Flash ausgeliefert werden. Für den Anfang ist kein separates CSS/JS nötig.

### HTTP-Endpunkte

Vorschlag:

- `GET /`
  - Einstellungsseite
- `GET /api/state`
  - aktueller Zustand als kleines JSON
- `POST /api/mode`
  - Hauptmodus ändern
- `POST /api/time`
  - RTC-Zeit setzen
- `POST /api/color`
  - Farbe und Farbmodus ändern
- `POST /api/text`
  - freien Lauftext setzen
- `POST /api/brightness`
  - Helligkeit setzen
- `POST /api/save`
  - Konfiguration dauerhaft speichern

Für den ersten Schritt kann man auch alles über ein Formular `POST /settings` lösen. Die API-Endpunkte werden besonders dann interessant, wenn später Node-RED oder andere Tools HTTP statt MQTT nutzen sollen.

## Uhrzeit Setzen

Über Web:

- Datum und Uhrzeit per Formular senden
- RTC mit `RTC.adjust(DateTime(...))` setzen

Über MQTT:

- ISO-ähnliche Payload senden, zum Beispiel `2026-05-08T18:35:00`
- Parser im ESP8266 bewusst einfach halten

Optional später:

- NTP-Zeit holen, wenn WLAN verfügbar ist
- RTC beim Start aus NTP korrigieren
- Zeitzone/Sommerzeit sauber behandeln

Für den Anfang ist manuelles Setzen über Web und MQTT einfacher und robuster.

## Freier Lauftext

Der freie Lauftext sollte als eigener Modus oder als Parameter für `MODE_MARQUEE_TIME` umgesetzt werden.

Optionen:

1. `MODE_MARQUEE_TIME` zeigt immer Uhrzeit
2. Neuer Modus `MODE_MARQUEE_TEXT` zeigt freien Text
3. `MODE_MARQUEE_TIME` bekommt eine Unteroption: Uhrzeit oder freier Text

Empfehlung:

- neuen Modus `MODE_MARQUEE_TEXT` ergänzen
- Text in einem festen Char-Puffer speichern, zum Beispiel `char marqueeText[96]`
- keine unbegrenzten `String`-Texte verwenden

## MQTT-Interface

### Topic-Basis

Vorschlag:

`wordclock/<deviceId>/...`

Beispiel:

`wordclock/wortuhr-wohnzimmer/mode/set`

Die `deviceId` kann fest im Code stehen oder später konfigurierbar werden.

### Steuer-Topics

- `wordclock/<deviceId>/mode/set`
  - Payload: `word_clock`, `seconds`, `countdown`, `daytime`, `marquee_time`, `marquee_text`, `binary`, `facts`
- `wordclock/<deviceId>/wordclock_mode/set`
  - Payload: `short`, `long`, `east`, `countdown_second_half`
- `wordclock/<deviceId>/color_mode/set`
  - Payload: `mono`, `mixed`, `rainbow_matrix`, `rainbow_side`, `rainbow_words`
- `wordclock/<deviceId>/color/set`
  - Payload: Hex-Farbe, zum Beispiel `#00FF00`
- `wordclock/<deviceId>/brightness/set`
  - Payload: `0` bis `255`
- `wordclock/<deviceId>/text/set`
  - Payload: freier Lauftext
- `wordclock/<deviceId>/time/set`
  - Payload: `YYYY-MM-DDTHH:MM:SS`
- `wordclock/<deviceId>/save`
  - Payload: `1`
- `wordclock/<deviceId>/reboot`
  - Payload: `1`

### Status-Topics

- `wordclock/<deviceId>/status`
  - Payload: `online` oder `offline`
  - Retained Message empfohlen
- `wordclock/<deviceId>/mode`
  - aktueller Modus
- `wordclock/<deviceId>/wordclock_mode`
  - aktueller Wortuhr-Untermodus
- `wordclock/<deviceId>/color_mode`
  - aktueller Farbmodus
- `wordclock/<deviceId>/color`
  - aktuelle Farbe als Hex
- `wordclock/<deviceId>/brightness`
  - aktuelle Helligkeit
- `wordclock/<deviceId>/text`
  - aktueller Lauftext
- `wordclock/<deviceId>/time`
  - aktuelle RTC-Zeit
- `wordclock/<deviceId>/ip`
  - aktuelle IP-Adresse

### MQTT-Verhalten

- beim Start WLAN verbinden
- MQTT verbinden
- `status = online` retained publishen
- Last Will auf `status = offline` setzen
- Steuer-Topics abonnieren
- nach jeder Änderung Status zurückpublishen
- Reconnect nicht blockierend durchführen

## WLAN-Konfiguration

Für den ersten Schritt gibt es zwei sinnvolle Varianten.

### Variante A: feste WLAN-Daten im Code

Vorteile:

- schnell umgesetzt
- wenig Code
- wenig Fehlerquellen

Nachteile:

- WLAN-Daten liegen im Code
- Änderung erfordert neuen Upload

### Variante B: WLAN-Setup per Access Point

Vorteile:

- komfortabler
- Uhr kann ohne Neucompilieren umziehen

Nachteile:

- deutlich mehr Code
- mehr Zustände
- mehr Testaufwand

Empfehlung:

Mit Variante A starten. Wenn Webserver und MQTT stabil laufen, WLAN-Konfiguration per Access Point ergänzen.

## Loop-Struktur

Die `loop()` sollte in kleine, regelmäßig aufgerufene Aufgaben zerlegt werden:

```cpp
void loop() {
  updateTime();
  handleWifi();
  handleMqtt();
  handleWebServer();
  updateDisplay();
}
```

Wichtig:

- keine langen `delay()`-Zeiten
- Animationen über `millis()` takten
- Webserver `handleClient()` in jedem Durchlauf aufrufen
- MQTT `client.loop()` in jedem Durchlauf aufrufen
- MQTT-Reconnect nur alle paar Sekunden versuchen

Der aktuelle `delay(100)` ist wahrscheinlich noch vertretbar. Für eine reaktionsfähige Web-/MQTT-Steuerung wäre ein millis-basierter Takt aber besser.

## Rainbow-Modi

Der bestehende `COLOR_MODE_RAINBOW` sollte intern in Varianten aufgeteilt werden.

Vorschlag für zusätzliche Defines:

```cpp
#define RAINBOW_MODE_MATRIX 0
#define RAINBOW_MODE_SIDE_GRADIENT 1
#define RAINBOW_MODE_WORD_STARTS 2
```

### `RAINBOW_MODE_MATRIX`

Ein zusammenhängender Regenbogen über die gesamte Matrix. Die Farbe hängt von Pixelposition und globalem Rainbow-Counter ab.

### `RAINBOW_MODE_SIDE_GRADIENT`

Ein gerichteter Verlauf von einer Seite zur anderen. Zusätzlich könnte eine Richtung gespeichert werden:

- links nach rechts
- rechts nach links
- oben nach unten
- unten nach oben

### `RAINBOW_MODE_WORD_STARTS`

Jedes Wort bekommt einen eigenen Startpunkt im Regenbogen. Dafür wäre es hilfreich, Wörter nicht nur direkt ins Grid zu zeichnen, sondern beim Zeichnen optional einen Wortindex oder Farboffset mitzugeben.

## Risiken und Gegenmaßnahmen

### RAM-Verbrauch

Risiko:

- Webserver, MQTT und dynamische Strings können Heap fragmentieren.

Gegenmaßnahmen:

- feste Char-Puffer
- kleine MQTT-Payloads
- keine großen HTML-Strings im RAM
- HTML aus Flash mit `F()` oder `PROGMEM`

### Blockierende Anzeige

Risiko:

- Lauftext oder Animationen blockieren Webserver/MQTT.

Gegenmaßnahmen:

- keine langen `delay()`-Aufrufe
- Animationen mit `millis()`
- Netzwerkfunktionen in jedem Loop-Durchlauf bedienen

### WLAN-Ausfälle

Risiko:

- Uhr hängt beim Reconnect.

Gegenmaßnahmen:

- nicht blockierende Reconnect-Logik
- Uhr läuft offline weiter
- MQTT/Web sind optionale Steuerkanäle

### Komplexität der Modi

Risiko:

- Moduslogik wächst unübersichtlich in `loop()`.

Gegenmaßnahmen:

- je Modus eigene `update...()`-Funktion
- zentrale `applyMode()`-Funktion
- ein gemeinsamer Config-State

## Umsetzung in Etappen

### Etappe 1: Struktur vorbereiten

- Config-Struktur anlegen
- Modusnamen und Konvertierungsfunktionen ergänzen
- freie Lauftextvariable ergänzen
- `loop()` in kleinere Handler aufteilen
- Build prüfen

### Etappe 2: Webserver

- WLAN-Verbindung mit festen Zugangsdaten
- einfachen Webserver starten
- Einstellungsseite ausliefern
- Modus, Farbe, Helligkeit und Lauftext per Formular ändern
- Uhrzeit über Formular setzen
- Build und Test auf Controller

### Etappe 3: MQTT

- `PubSubClient` einbinden
- MQTT-Verbindung herstellen
- Last Will und Status-Topic einrichten
- Steuer-Topics abonnieren
- Status-Topics publishen
- Node-RED-Testflow erstellen

### Etappe 4: Persistenz

- Einstellungen speichern
- Einstellungen beim Start laden
- sinnvolle Defaults definieren
- Speichern über Web und MQTT auslösen

### Etappe 5: Verbesserungen

- NTP-Zeit optional ergänzen
- Rainbow-Varianten implementieren
- Weboberfläche schöner strukturieren
- WLAN-Konfiguration per Access Point prüfen
- OTA-Update optional prüfen

## Empfehlung

Die Erweiterung ist machbar. Der ESP8266 hat nach aktuellem Build genug Flash und RAM, solange die Netzwerkfunktionen schlank bleiben. Ich würde zuerst Webserver und MQTT ohne Persistenz integrieren, damit die Steuerung schnell testbar ist. Danach sollten Einstellungen dauerhaft gespeichert und die Anzeige-Loop auf `millis()` umgestellt werden.
