## Raspi network code

Compile using:
```
gcc client.c -o client
gcc server.c -o server
```
Then run `./server` on your host machine

From the client run `./client <server-ip>` and write the readings to stdin as:
```<bot_id> <reading>```

The server will write the readings to csv files for each bot: *bot_$id.csv*
