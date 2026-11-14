# Home Automation System (ESP8266 + Blynk)

A WiFi-based home automation controller built on an ESP8266 board. It switches two lights and
a fan from a Blynk mobile dashboard, toggles the lights from physical wall buttons, and turns
the fan on automatically when the room gets too warm.


## Features

- **Remote control over WiFi** — two bulbs and one fan are switched from the Blynk app
  (`V3`, `V4`, `V5`), and the app stays in sync when the physical buttons are used.
- **Automatic fan control** — the DHT11 temperature is sampled every second; if it rises above
  **33 °C** the fan turns on, and it turns off again once the temperature drops back below the
  threshold (only while `AutoMode` is enabled).
- **Temperature telemetry** — the latest reading is pushed to Blynk (`V1`) once per second, so
  the dashboard can show a live value/gauge.
- **Physical wall buttons** — momentary push buttons on `D6` and `D7` toggle each bulb with a
  200 ms debounce, and mirror the new state back to the app.
- **Auto / Manual mode flag** — `AutoMode` defaults to `true` and is published to `V2`. While
  manual mode is active, the sketch is written to release all outputs instead of driving them.
- **Safe start-up** — all three relays are driven off and all app switches are reset to `0`
  before the first Blynk connection completes.
- **On-board status LED** — `LED_BUILTIN` is configured and held off in `setup()`.

---

## Hardware Components

| # | Component | Quantity | Purpose |
|---|-----------|----------|---------|
| 1 | ESP8266 development board (NodeMCU / Wemos D1 mini) | 1 | Main controller with WiFi |
| 2 | DHT11 temperature & humidity sensor | 1 | Room temperature sensing |
| 3 | 3-channel (or 3 x 1-channel) relay module, 5 V | 1 | Switching the two bulbs and the fan |
| 4 | Push button, momentary, normally open | 2 | Manual wall switches for the bulbs |
| 5 | 10 kΩ resistor | 1 | Pull-up on the DHT11 data line |
| 6 | 5 V power supply | 1 | Board and relay coil supply |
| 7 | Bulbs, fan, and lamp holders / wiring | — | The actual loads being controlled |

### Pin mapping

| Function | Board pin | GPIO | Notes |
|----------|-----------|------|-------|
| DHT11 data | `D5` | GPIO14 | 10 kΩ pull-up to 3V3 |
| Bulb 1 relay | `D1` | GPIO5 | Active **LOW** (relay on = `LOW`) |
| Bulb 2 relay | `D2` | GPIO4 | Active **LOW** |
| Fan relay | `D3` | GPIO0 | Active **LOW** — must be `HIGH` at boot for flashing |
| Button 1 | `D6` | GPIO12 | Wired to GND, internal pull-up |
| Button 2 | `D7` | GPIO13 | Wired to GND, internal pull-up |
| Status LED | `D4` (`LED_BUILTIN`) | GPIO2 | Set `HIGH` (off) in `setup()` |
| — | `D0` | GPIO16 | Declared as `OUTPUT` but not used |



## How It Works

1. `setup()` configures the relay, button and LED pins, switches every relay off, resets the
   app switches, starts the DHT11, connects to Blynk, then registers `measureTemp()` on a
   1000 ms timer and publishes the initial mode and fan state.
2. `BLYNK_WRITE(V3/V4/V5)` handlers set the matching relay according to the app switch.
3. `measureTemp()` runs every second: it reads temperature and humidity, applies the
   `> 33 °C` fan rule while `AutoMode` is on, and pushes the temperature to `V1`.
4. `loop()` keeps `Blynk.run()` and `timer.run()` alive, and polls the two push buttons. Each
   press flips the bulb state, drives the relay, and writes the new value back to the app.


