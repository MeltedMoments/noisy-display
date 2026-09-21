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

