# NoisyProject working notes

## Idea

Measure ambient noise in the alley and provide a simple visual indication
of sustained noise level.

Initial goal is not calibrated/legal dBA measurement. The first prototype
should detect relative sound level and show something like:

quiet -> acceptable -> getting loud -> too loud

No audio recording or storage.

## Why

Small experimental project for:
- learning ESP32 / embedded programming
- sensors and physical-world inputs
- LED/display outputs
- eventual preparation for watermill monitoring/automation

## Hardware

### ESP32-S3
- ESP32-S3 N16R8
- 16 MiB flash
- 8 MiB PSRAM
- PlatformIO / Arduino framework
- onboard NeoPixel: GPIO 48
- BOOT button: GPIO 0

### Microphone
- 2 x INMP441
- I2S
- not yet tested

### Display
- 2 x 8-pixel WS2812B sticks
- not yet tested

### Other bits
- AHT20 temperature/humidity sensor
- DHT22 module
- Trinket M0
- Grove LED bar

## 20260910

### ESP32 start up
- PlatformIO build/upload works
- USB serial connection is currently slow/unreliable
- temporary long startup delay needed to see setup() output
- TODO investigate USB/serial configuration

### Memory
- 16 MiB flash detected
- ~388 KiB internal heap
- ~8 MiB PSRAM
- tested 1 MiB PSRAM allocation/free successfully
- experiments/memory_test.cpp

### GPIO
- BOOT button read successfully using INPUT_PULLUP
- pressed = LOW

### Onboard NeoPixel
- Adafruit NeoPixel library
- GPIO 48
- colour changing works
- button cycles through colours
- experiments/onboard_pixel_test.cpp

### Timing
- experimented with millis()
- non-blocking periodic work instead of delay(): heartbeat
- looked at button edge detection and debounce

### I2C / AHT20
- SDA GPIO 8
- SCL GPIO 9
- I2C scanner detects device at 0x38
- experiments/i2c_scan.cpp

### next time
- Read AHT20 temperature/humidity.
- Explore what the AHT20 library is doing over I2C.
- Investigate ESP32 USB/serial startup behaviour.
- Try Wokwi simulation.
- When soldering supplies arrive:
   - solder 8-pixel NeoPixel stick
   - solder INMP441
   - start actual sound-level experiments.

## 20260911
- okay stop fussing about the lack of soldering equipment, plenty to do in the meantime.
- these workingnotes started, first part by chat, but we'll use the same format that I used in migtool from here on in. That seems to work well
- today: read the aht20 and install wokwi

### notes
- aht20 gave me hell, but it was because it wasn't getting power correctly. using F-F connectors finally got it working
- but I learnt a bit more about my multimeter.
- big clue: Finally the little green light on the aht20 lit up

    -  Is it powered?
    -  Is ground common?
    -  Are the signal lines electrically sensible?
    -  Can the bus see the device?
    -  Does the library/protocol work?
    -  Is the application logic correct?

- messing around with wokwi, seems like it will be pretty useful
    - needed to add these flags to .ini to make it work (but now hangs on real board)
```
build_flags = 
    -DBOARD_HAS_PSRAM
    -DARDUINO_USB_MODE=1
    -DARDUINO_USB_CDC_ON_BOOT=1    
```

### eod
- managed to read the AHT20 from the ESP when directly connected. Prob need new breadboards?
- got a rudimentary pixel display for some fake data in wokwi
- needs improving
    - show-pixels() is off-by one. need to think it through properly, not all 8 lights shown when level 8. 
    - better algorithm to figure out the level-divisor (needs max, min and divisble into 8)

### next time
- prob starting to need some project organisation
- start a git repo
- fix the current version of NoisyDisplay
- Clean up show_pixels() so it always clears the strip first and then lights exactly level pixels.
- Replace the big if/else threshold ladder with a calculation from 0–100 to 1–8.
- Then make the fake input more realistic — values bouncing around like 48, 52, 49, 55, 53... — and watch the LEDs flicker annoyingly.

## 20260912
- fairly irritated attempt at making headpins with silver-filled wire. Hopefully cleaning the oxycons will solve my problems
- cleaned up neopixel_test 

### eod
- played around with smoothing the data
- simple moving average, 4 readings neopixel-sma-test.cpp
- current one using exponential moving average with Rise and Fall factors 

### next time
- prob starting to need some project organisation
- start a git repo
- figure out breadboard: how to measure
- shopping list
    - krimpkous
    - breadboard?

## 20260913
- a few hours spare
- implement hysteresis 
- project org
- then grove 16x2 display

## 20260914
- yesterday got the aht20 and 16x2 display working together on the same I2C bus
- needed to remember how to power and use a breadboard!
- esp doesn't need to live on a BB, but use a BB as a bridge
- grove's is connected by duponts, connections are a bit wobbly

- started using arduino String, and also std::min and max()
- heartbeat now displays on the lcd :)

### notes
- oled 
```
OLED signal   ESP32-S3       Purpose        (wire)
---------------------------------------------
VCC           3V3            power          red
GND           GND            ground         black
DIN           GPIO 11        SPI MOSI/data  orange
CLK           GPIO 12        SPI clock      green
CS            GPIO 10        chip select    grey 
DC            GPIO 9         data/command   blue
RST           GPIO 8         reset          purple
```

### eod
- okay nice, oled hooked in as well, now updates temps on oled and lcd
- tiny 8x8 bitmap to actually display a heart as the heartbeat on the oled :)

### next time
- Add history to eg temperature  and display small graph 

## 20260915
- stuff from reichelt should arrive today, and Herman, and BAM too
- anyway, try to implement a "rolling history graph".

### eod
- decided to split the code instead, much saner
- soldering stuff arrived so will hopefully wire stuff up next time

### next time
- move experiments to src/
- try runnable neopixel wokwi experiment, sep from main working code
- get neopixel and microphone working 
- 

## 20260916
- finally managed to change vscode project colours so I know where I am (though colours still need improvement)
- right first thing is to reorg stuff so that I can run the neopixel wokwi demo
- then solder a neopixel
- and try it for real :)

### notes
- moved heartbeat code to heartbeat.cpp, that completes the refactoring
    - hmm, one more improvement: return bool from setup-<display>, and only try to display if we've got one, but maybe next time. 
- moved things around in platformio.ini
    - main project is 'esp32'
    - can run (wokwi) sub-projects neopixel-test (strip_fill_test) and neopixel-wokwi (hysteresis-test)
    ```
    # in project root
    ./wokwi-build neopixel-wokwi  (then open wokwi simulator)
    ```

    - program in the esp32 env is set as default in PIO, so can build and upload as normal
- Start soldering! 

- microphone
```
INMP441       ESP32-S3   (Colour)
VDD    ─────> 3V3         red      
GND    ─────> GND         black       
SCK    ─────> GPIO 17     blue    I²S bit clock
WS     ─────> GPIO 18     yellow  I²S word-select / LR clock
SD     ─────> GPIO 16     green   microphone data -> ESP
L/R    ─────> GND
```

### eod
- neopixel soldered and tested
- microphone soldered and tested
- now need to hook the two together

## 20260917
- Neat new magnetic third hand :)
- okay first thing is to sort out what the min,max numbers mean
    - 16ms samples of the peak/troughs, deltas are what we're interested in

- Apply Root Mean Squared: calculated as the square root of the mean of the squares of a set of values
    - Squaring places a heavier weight on larger spikes or errors, making RMS very sensitive to extreme values

```
RMS
  └─ size of the numbers coming from our microphone

dBFS = decibels relative to Full Scale
  └─ how large that digital signal is relative to
     the largest digital signal the system can represent

dB SPL = decibels Sound Pressure Level
  └─ how large the actual pressure fluctuations in the air are
```

- Wonderful, first working version of noise -> light display! Very pleased. 
    - Soldering works, wiring works, code works.
    - And I understand (more or less) what's going on, chat's in Super Teacher Mode, so isn't just feeding me code. 

- Time to start a new repo
    - TBD: these working-notes are in the embedded-playground repo
    - semi-solved, copied to noisy-display repo, this is now the main file

### eod
- Oh very satisfactory. Basic thing works 
- created new repo embedded/noisy-display
- split off microphone and noise_display
- tried to mess with config.h but things got confused, so that still needs sorting out

```
# Future steps
1. Improve the measurement/display behaviour. Play with the working device and decide what feels right. We already have hysteresis; perhaps add fast-rise/slow-fall behaviour if the LEDs are too twitchy. More importantly, decide what “persistent noise” means. A quarter-second measurement is good input, but a café warning probably shouldn’t turn red because somebody drops a glass.
1. Do the real-world alley experiment. On an evening when there’s useful noise, put the microphone somewhere sensible and record what you hear versus what it measures: quiet alley, ordinary café conversation, intrusive noise, very loud noise. That’s when we establish useful thresholds. We can also compare its estimated SPL with a phone sound-meter app as a rough sanity check. Proper calibration can come later.
1. Then revisit the signal processing. At that point we’ll know whether the estimated SPL conversion is good enough and whether we want proper calibration and perhaps A-weighting. A-weighting is especially relevant to environmental noise because it approximates the frequency sensitivity of human hearing. This is also where I’d probably separate the current microphone responsibility into acquisition versus sound-level processing—but only once there’s enough code to justify it.
1. Make it autonomous. Get rid of its dependence on the Mac/Serial Monitor. Give it sensible startup behaviour, perhaps use the onboard LED as heartbeat/error indication, power it from a standalone USB supply, and have it sit there happily doing sound → lights indefinitely. That’s quite an important embedded milestone: the computer is no longer part of the device.
```

## 20260918
- right, Herman's fiddling around with the breadboard, what am I doing today?
- start looking at the persistent noise problem

### eod
- setting up first set of unit tests to test the signal processing part
- working well

### next time
- continue messing around with hysteresis
- move main test code into assert_xxx()
- and continue

## 20260919
- nice got a whole day to fool around
- move code around
- figure out the boundaries yourself, set up a few more tests
- continue

### notes
- oled 
```
OLED signal   ESP32-S3       Purpose        (wire)
---------------------------------------------
VCC           3V3            power          red
GND           GND            ground         blue
DIN           GPIO 11        SPI MOSI/data  orange
CLK           GPIO 12        SPI clock      green
CS            GPIO 10        chip select    grey 
DC            GPIO 9         data/command   yellow
RST           GPIO 8         reset          purple
```
## 20260920
- No coding today, but we did a street ('field') test last night:
    - Hacked together a baffle box from the spongy part of a couple of dishwasher sponges. Seem to work fairly well
    - It was pretty noisy last night.
    - Subjectively the lights seem to react well to the diff sounds. And the levels more or less corresponded to our subjective feelings.
    - The lights are now [greenx2 amberx3 redx3].
    - min-db: 55 max-db: 85

 - One thing we noticed is that it very rarely fell much below 4 lights (2 amber). Which is prob correct, as it was noisy. But it led us to thinking that a way to dial in some 'sensitivity' may be useful. 

 - Best implementation is prob to use the oled KEY0 button to cycle through "sensitivities": low, medium, high, [chat noisydisp 1230]. eg define them as

```
LOW sensitivity       65 ───────── 90
MED sensitivity       60 ───────── 85
HIGH sensitivity      55 ───────── 80
                      0             8 lights
```

- Alternatively: "There is another possibility worth keeping in mind: perhaps you don’t actually want three sensitivities. You might instead want one configurable baseline—“below this level, don’t light much”—while leaving the upper/red threshold fixed. That may correspond more closely to the real question: when does ordinary background street noise become noise worth drawing attention to? Your next couple of street tests can tell us which interpretation feels useful."

- Successful test!

### notes
- ah well managed to squeeze a bit it. 
- connected KEY1 of the oled to the esp
    - ha but don't use gpio 19 or 20 as they are the built-in usb pins
- reading the button is the same as the small experiment that I did to read the onboard button (see chat 20260910 1830). There's nothing special about it being on the oled. 
- next time implement
    - reading the button
    - debouncing the input (properly using 2 vars)
    ```
    int raw_state = digitalRead(BOOT_BUTTON_PIN);

    if (raw_state != last_raw_button_state) {
        last_change_time = now;
        last_raw_button_state = raw_state;
    }

    if (now - last_change_time >= DEBOUNCE_MS) {

        if (raw_state != stable_button_state) {
            stable_button_state = raw_state;

            if (stable_button_state == LOW) {
                colour_index = (colour_index + 1) % 3;
                show_colour();
            }
        }
    }    
    ```    

### eod
- button connected and being read
    - without debouncing the button seems to react quicker
    - with debouncing it sometimes seems to miss it, or you have to press and hold 
    - but it works!

### next time
- figure out whether debouncing is useful or not, or figure out how to tune it
- then implement sensitity, cycling and display on the oled

- but make beads first and do some of the other stuff

## 20260921
- beads made. house-howto started
- oh, go do a bit of shopping
- so a few hours to fool around. 
- try get the button working? 

### notes
- ah, okay, debouncing works well, but the microphone is hogging the cycles.
- so need to read in smaller chunks, and process only when the array is full

### eod 
- okay, chunked buffer is working and now the button press is very reactive
- sensitivy enum is set up
- key1 is hooked up and clicking it cycles through the sensitivites (and displays it as well)
- had to fiddle witht eh display, but have also parameterised it a bit, so it makes it easier to change
- next part is deciding what the values mean and how to implement it

### next time
- implement sensitivity ranges


## 20260922
- slowly should be turning my mind to other things. 
- Need to setup TravelCat. That will prob take a few days (esp email client)
- so today, try the sensitivity implementation which changes the lower bound. 

### eod
- sensitivity is implemented. Turned out to be fairly simple. Now waiting for more field tests
- decided to dabble around with wifi in noisy-display/src/experiments
    - wifi_test: basic wifi connection
    - wifi_scanner: scan for all wifi networks
    - wifi_events: wifi connection using wifi events
    - web_server: tiny web-server, serving root (/) and /uptime

- sends json response (application/json) instead of text/plain
    - first manually assembled
    - then using JsonDocument (need lib_deps = bblanchon/ArduinoJson )
- served at /api/status

### next time
- if enough time (should be) try to add web-server to noisy-display
- hmm, ordered extra esp. Prob set that up instead (for wokwi etc too)

## 20260924
- few hours to fool around. still waiting on 2nd esp.
- do a few more wifi experiments
    - send back dynamic data
    - fool around with hotspot on/off
    - accept http input eg GET /api/hello?name=Jennie. Return {"message":"Hello Jennie"}

### eod
- okay those basic tests work
- started trying to get wokwi working as well. split secrets into config, with build flag in PIO.ini. But seem to have lost serial output? 

## 20260925
- new dev board has arrived! It's an official one in an official box. 
- https://documentation.espressif.com/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html
- got into a mess yesterday, but learnt today 
    - diff betw [env:xxx] and [xxx]
    - the former defines a buildable PIO environment
    - the latter is a named section
- So it's better to mostly use sections. But realise that overlapping properties can be a problem, so split the responsibility something like:

```
esp_base       → platform/framework/common serial config
board_n32r16v  → flash/PSRAM/hardware
web_server     → source files + ArduinoJson dependency
wifi_home      → network-selection define
```

### notes
- okay the boards have names now:
    - wren: is the original board, hooked up to noisy-display
    - finch: is the new board, available for experiments
- poss to hook up both to crazycat, makes use of platformio-local.ini (must be specified in the [platformio] section!) to define the ports

- clues on how to define neopixel colours: https://forums.adafruit.com/viewtopic.php?t=80363
```
• Red: strip.Color(255, 0, 0) or 0xFF0000
• Green: strip.Color(0, 255, 0) or 0x00FF00
• Blue: strip.Color(0, 0, 255) or 0x0000FF
• Yellow: strip.Color(255, 255, 0) or 0xFFFF00
• Cyan: strip.Color(0, 255, 255) or 0x00FFFF
• Purple: strip.Color(255, 0, 255) or 0xFF00FF
• White: strip.Color(255, 255, 0) or 0xFFFFFF
• Off (Black): strip.Color(0, 0, 0) or 0x000000
```

### eod
- wren is now running noisy-display again
- can send finch web-server a POST request to turn the onboard led on/off
- add naughtily ordered yet more breadboards (from eleshop)

### next time 
- run web-server on wren, see if finch can request /api/status

## 20260927
- asked chat to suggest some travel experiments
```
Yes. Given Finch + Mac + USB cable only, I’d make the trip mostly about the ESP itself and networking/software rather than peripherals. You can get surprisingly far without attaching a single component.

And since you’ll have Wokwi as well, there’s a nice division: learn/test generic ideas in Wokwi; use Finch when the real hardware or real network matters.

Here are the experiments I’d put on the menu, roughly from small/easy to increasingly interesting.

1. Finish the HTTP server properly. You’re already most of the way there. Add GET /api/status, a POST that changes some internal state, sensible 404 responses, query parameters, and HTTP status codes such as 200/400/404. You could make /api/status report useful real information:

{
  "name": "finch",
  "uptime_ms": 123456,
  "wifi": {
    "ssid": "hotel-wifi",
    "rssi": -57,
    "ip": "192.168.1.42"
  },
  "heap": {
    "free": 342112
  }
}

That would teach you quite a lot about designing a tiny API without becoming a project.

2. Make Finch an HTTP client. So far you’ve mostly done:

Mac ──HTTP──> Finch

Turn it around:

Finch ──HTTP──> server

Use HTTPClient to GET a simple public endpoint and inspect the status code, headers and body. Then try JSON and parse the response with ArduinoJson. That introduces a pattern you’ll almost certainly want later.

3. Run both client and server. Finch can happily be both:

                 GET /api/status
Mac ─────────────────────────────> Finch
                                     │
                                     │ GET something
                                     ▼
                                  Internet

This starts making the ESP feel less like an Arduino with Wi-Fi glued on and more like a small networked computer.

4. Explore Wi-Fi failure properly. You’ve already started this and it’s particularly suitable while travelling because you’ll encounter different networks anyway. Experiment with connection timeout, disconnect events, reconnect, wrong credentials, network disappearing, network returning, and perhaps falling back from one known SSID to another.

You could give Finch a little connection state machine:

DISCONNECTED
     ↓
CONNECTING
     ↓
CONNECTED
     ↓
LOST
     ↓
RECONNECTING

Don’t necessarily build an elaborate framework around it; just observe and understand the behaviour.

5. Multiple known networks. This follows naturally from your home/phone/Wokwi configuration work. Instead of compiling Finch for one network:

HOME
PHONE
WOKWI

let the firmware know several networks and choose one that’s available. That begins answering the earlier question of what a real NoisyDisplay should do when moved somewhere else.

6. Wi-Fi scanning. Very small and quite fun:

Finch scans
   ↓
HotelWifi       -42 dBm
JenniesPhone    -61 dBm
SomebodyElse    -78 dBm
...

Then experiment with RSSI. Walk Finch around the room and see what happens to signal strength.

That also gives you some real numbers behind the vague Wi-Fi-bars concept.

7. Try mDNS. Instead of remembering:

192.168.1.42

try reaching:

finch.local

That’s a particularly useful little experiment for NoisyDisplay because DHCP addresses change. A human-friendly device name is much nicer than hunting for its IP address.

8. Give Finch a tiny web page. You’ve been returning text and JSON. Return HTML instead:

Finch
Uptime: 3h 42m
Wi-Fi: connected
RSSI: -53 dBm
Free heap: 351 KB

Don’t turn this into frontend development. 😄 Just hand-write perhaps 15 lines of HTML and serve them.

Then perhaps have that page use JavaScript to fetch /api/status. Suddenly you’ve got:

browser
   │
   ├── GET /
   │       ↓
   │      HTML
   │
   └── GET /api/status
           ↓
          JSON

That’s directly relevant to the eventual idea of giving NoisyDisplay — and even MigTool, conceptually — a simple web interface.

9. Server-Sent Events or polling. Once the status page exists, make it update automatically. Start with JavaScript polling:

every 2 sec
    ↓
GET /api/status
    ↓
update page

That’s very easy and teaches the principle. Later you can investigate pushing updates from ESP → browser.

10. Explore non-volatile storage. Finch has plenty of flash. Learn ESP32 Preferences/NVS:

preferences.putString("name", "Finch");

Reboot:

preferences.getString("name");

Then use the web API to change a value, reboot Finch, and discover that it’s still there.

That’s a useful milestone:

POST setting
     ↓
store in NVS
     ↓
power off
     ↓
power on
     ↓
setting survives

Now you’re starting to have an actual configurable appliance.

11. Wi-Fi configuration portal. This is a bigger experiment, but very relevant eventually. If Finch can’t connect to a known Wi-Fi network:

No known Wi-Fi
      ↓
Finch creates its own access point
      ↓
Mac connects to Finch
      ↓
configuration page
      ↓
enter SSID/password
      ↓
Finch stores credentials
      ↓
restart/connect

I wouldn’t start there. But several of the earlier experiments—web server, NVS, scanning, connection state—lead naturally towards it.

12. OTA firmware update. Another very useful ESP trick: upload new firmware over Wi-Fi rather than USB.

That’s probably towards the end of the holiday menu, because you want to understand what you’re doing before deliberately teaching Finch to rewrite its own firmware. But it’s a nice goal:

pio run -t upload
        │
       Wi-Fi
        │
        ▼
      Finch

No USB upload cable required except for recovery.

And there are a few hardware-free ESP experiments that aren’t networking at all: timers, FreeRTOS tasks, queues, watchdog behaviour, PSRAM allocation, measuring heap fragmentation, deep sleep/wakeup timers, and examining what survives different kinds of reset. Those are useful, but I wouldn’t rush into FreeRTOS just because it’s there.

What I’d actually take with me

I’d resist packing a travelling electronics laboratory. Your proposed kit sounds just right:

Finch
USB cable
travel Mac
breadboard purely as physical protection

You don’t even really need the breadboard electrically.

Finch itself already gives you:

Wi-Fi
Bluetooth
RGB LED
32 MB flash
16 MB PSRAM
timers
NVS
USB/UART
two CPU cores

That’s an enormous playground without a jumper wire in sight.

A nice loose progression

If you feel like programming while away, I’d roughly wander through:

HTTP server          ← you're here
     ↓
POST + state changes
     ↓
better /api/status
     ↓
mDNS (finch.local)
     ↓
tiny HTML status page
     ↓
page fetches JSON
     ↓
HTTP client
     ↓
Wi-Fi scanning/reconnection
     ↓
NVS/preferences
     ↓
configuration survives reboot
     ↓
perhaps configuration portal

None of those needs to become part of NoisyDisplay. Keep experiments/ exactly for this purpose. If you get bored halfway through one, abandon it.

And I’d definitely keep Wokwi Wi-Fi as a separate unfinished problem, rather than making it a prerequisite. Finch is now known-good, portable between both Macs, and can connect to real Wi-Fi. You don’t need to spend your holiday debugging a simulator in order to learn ESP networking.
```

## 20260928
- installing docs on travelcat (see embedded/offline-docs)
- $ brew install ripgrep

```
cd offline-docs/
rg "onNotFound" arduino-esp32/
rg "HTTPClient" arduino-esp32/libraries/
rg "Preferences" arduino-esp32/libraries/

rg "class WebServer" ~/.platformio/packages/framework-arduinoespressif32/

# case-insensitive
rg -i "webserver"

# only C++ headers
rg "HTTPClient" -g "*.h"

# line numbers are normally shown automatically
rg "onNotFound" arduino-esp32/

# list matching filenames rather than matching text
rg -l "Preferences" arduino-esp32/
```

# ===> I AM HERE MARKER HERE AM I <===
# ===> I AM HERE MARKER HERE AM I <===
# ===> I AM HERE MARKER HERE AM I <===
# ===> I AM HERE MARKER HERE AM I <===
# ===> I AM HERE MARKER HERE AM I <===


## Things learned / reminders

- setup() runs once after boot/reset; loop() then repeats.
- Serial monitor may miss early output even though setup() ran.
- millis() is useful for scheduling without blocking.
- I2C devices share SDA/SCL and have addresses.
- NeoPixel setPixelColor() updates the buffer; show() sends it.
- Store experimental programs outside src/ if they contain their own
  setup()/loop(), because PlatformIO compiles all .cpp files in src/.

