## Raspi network code

Compile using:
```bash
make
make client # only client
make server # only server
```
Then do `cd build` and run `./server` on your host machine

From the client run `./client <server-ip>` and write the readings to stdin as:
```<bot_id> <reading>```

The server will write the readings to csv files for each bot: *bot_$id.csv*
