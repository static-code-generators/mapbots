## Raspi network code

Compile using:
```bash
make
```
Then do `cd build` and run `./server` on your host machine

`telnet <ip> <port>` from the client pc to connect to the host, and send the readings

The server will write the readings to csv files for each bot: *bot_$id.csv*
