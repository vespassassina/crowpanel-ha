# Decisions

Newest first.

## 2026-10-26: Public release

- Generic version of a private bedroom panel. Hardware, audio and voice are split into
  packages; the example dashboard takes its entities from substitutions.
- MIT licence. CI builds the example for both chip kinds on every push.
- Tested by flashing the example to a V1.2 board (P4 revision v1.3): boots, joins HA,
  reads its HA entities, wake word running, screen sleeps after 60 s.

## Voice package does not touch LVGL

The voice package only sets `voice_phase` and calls a `voice_ui` script. The screen side
(resume LVGL, redraw) lives in the main config, so the package also works without a UI.

## Whole-degree thermostat buttons, one call after the last press

Each press used to wait for HA's echo, and the next press built on the old value. Now the
target changes on screen at once, HA gets one `climate.set_temperature` 600 ms after the
last press, and HA's value is ignored for 10 s after a press.

## Wake word restart waits for the assistant

`on_end` fires before the voice assistant reports idle. Restart micro_wake_word then and the
start is skipped; the panel stops listening for good. So `on_end` waits for
`not voice_assistant.is_running` first.

## Microphone gain 8, auto gain, noise suppression 2

The PDM mic is quiet: talking at 1 m lifted the 1 s RMS only about 7 dB. Elecrow's samples
multiply by 10. Gain 8 with `auto_gain: 31dBFS` and noise suppression 2 gives reliable wake
word and speech-to-text from a few metres.

## Amplifier on only while playing

The NS4168 hisses when enabled and idle. GPIO30 (low = on) is switched by the media player:
on with sound, off 500 ms after idle.

## Hosted Wi-Fi buffers in PSRAM

On engineering-sample P4s about 147 KB of internal RAM is free before the scheduler starts,
and `esp_hosted_init` took 125 KB of it for SDIO buffers. Any growth in the build then
aborted boot in `vApplicationGetTimerTaskMemory`. `CONFIG_ESP_HOSTED_MEMPOOL_PREFER_SPIRAM`
moves the buffers to PSRAM, which P4 DMA reaches. Early free heap went from 7.8 KB to
95.6 KB.

## Audio straight on I2S, not the codec

The V1.2 schematic shows the ES8311/ES7210 I2S lines behind unfitted resistors. The P4
reaches a PDM mic and the NS4168 amp directly. GPIO20 ("Mic R-L") high adds noise and kills
the mic, so it is held low.

## SDIO data lines from the V1.2 schematic

V1.0 wires D0-D3 to GPIO14-17; V1.1 and V1.2 reverse them (D0 = GPIO17). Elecrow's ESPHome
example has the V1.0 order, and a V1.1 board boot-loops on it.

## Waveshare 7B display model

ESPHome has no Elecrow model. The Waveshare ESP32-P4 7B uses the same EK79007 1024 x 600
panel, and its timings work.

## Stock C6 firmware

ESPHome's `esp32_hosted` works with the firmware the C6 ships with. No C6 reflash.

## ESPHome with LVGL

Native HA API, boots in seconds, config in YAML, OTA updates. ESPHome 2026.10 supports the
P4, MIPI-DSI, esp32_hosted and the voice components.
