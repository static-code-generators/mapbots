#pragma once

struct location {
    float x;
    float y;
    float theta;
};

struct payload {
	int bot_id;
	float reading;
    struct location loc;
};
