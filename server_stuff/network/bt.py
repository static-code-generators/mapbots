import serial

serial.Serial("/dev/tty.HC-05-DevB", 9600);
outfile = open("data.csv", "wb+")

with open("data.csv", "wb+") as outfile:
    while True:
        line = ser.readline()
        outfile.writeline(line)
        print(line)
