import serial

ser = serial.Serial("/dev/tty.HC-05-DevB", 9600)
ser.close()
ser.open()
outfile = open("data.csv", "wb+")

try:
    while True:
        line = ser.readline()
        outfile.write(line)
        print(line)
except KeyboardInterrupt:
    print "kaise madarchod ho tum"
    print "rukta hai hum"
    ser.close()
    outfile.close();
