## List of commands to run
```
AT+RST
AT+CWJAP="SSID","PASSWORD"
AT+CWMODE=1
AT+CIPMUX=1
AT+CIPSTART=0,"TCP","<server-ip>",<port>
AT+CIPSEND=0,10 followed by data
AT+CIPCLOSE=0
```
