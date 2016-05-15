# mapbots

An army of midget bots for mapping an indoor area.

We've referred to [these][1] [two][2] sources extensively for implementing the algorithm. Thanks!

* [`examples/`](https://github.com/static-code-generators/mapbots/tree/master/examples): Outputs of running our bot and algorithm on different sample spaces
* [`bot_stuff/`](https://github.com/static-code-generators/mapbots/tree/master/bot_stuff): Bot-side code and schematics
* [`server_stuff/`](https://github.com/static-code-generators/mapbots/tree/master/server_stuff): Server-side code for the map generation algorithm(s)

Go to the above sub-directories for more details!

## Results

### Pentagonal Area

<img src="https://raw.githubusercontent.com/static-code-generators/mapbots/master/examples/pentagon_source.jpg" alt="enclosure" height="200">
<img src="https://raw.githubusercontent.com/static-code-generators/mapbots/master/examples/pentagon.png" alt="result" height="400">

### Rectangular Area

<img src="https://raw.githubusercontent.com/static-code-generators/mapbots/master/examples/rect_source.jpg" alt="enclosure" height="200">
<img src="https://raw.githubusercontent.com/static-code-generators/mapbots/master/examples/rect.png" alt="result" height="400">

[1]: http://acoustics.mit.edu/GOATS/2002publications/8.pdf "Robust Mapping and Localization in Indoor Environments Using Sonar Data"
[2]: http://www.iri.upc.edu/people/jsola/JoanSola/objectes/curs_SLAM/SLAM2D/SLAM%20course.pdf "Simulataneous Localization and Mapping with the Extended Kalman Filter"
