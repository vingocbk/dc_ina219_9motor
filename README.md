# DC INA 219
## Program write with Platform IO

Begin start: board in MODE_WAIT_RUNNING: LED BLUE turn On.
In this mode, First in start, Board will wait for user use RX to control Mode ON or OFF.
MODE_RUNNING: Board is running. Led Green turn On.
MODE_CONFIG: Turn On Bluetooth to setup. Led Red turn On.

When Start. Board will check what motor is Running. If have Motor -> Led Green Blink 200ms