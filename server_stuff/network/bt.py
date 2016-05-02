import serial

serial.Serial("/dev/tty.HC-05-DevB", 9600);
outfile = open("data.csv", "wb+")

while True:
    line = ser.readline()
    outfile.writeline(line)
    print(line)
