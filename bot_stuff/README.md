# mapbots/bot_stuff

## Code

Most of the sketches here are present for historial and testing purposes.
The following folders are the important ones:

* `bot_final`: The final Arduino sketch; does obstacle-avoidance, moves around and
collects readings by rotating servo, sends readings via Bluetooth in format required
by server-side code.
* `Libraries/`: Contains the following:
    * `Direction.h`: Functions for movement
    * `Constants.h`: Constant pin definitions
    * `Sensor.h`: Functions for taking readings from an array of ultrasonic sensors

## Hardware

<img src="https://3.bp.blogspot.com/-9X0Zeg9Yaig/Vyf9BvbrNTI/AAAAAAAAEew/NfobdMTVeGY3W1fpQS2nMPPd8JefnnlbgCLcB/s1600/photo_2016-05-03_06-50-50.jpg" alt="schematic" height="500">
<img src="https://3.bp.blogspot.com/-ZeWhXZpw8ys/Vyfc2ddf4MI/AAAAAAAAEeE/55zDGMC1nWMUJPjn8B_DkksUMb3HbMGSQCLcB/s1600/photo_2016-05-03_04-31-10.jpg" alt="bot" height="400">
