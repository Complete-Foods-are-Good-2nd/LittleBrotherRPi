import serial
import sys

if len(sys.argv) != 2:
    print("Error: The number of arguments is not 1", file=sys.stderr)
    sys.exit(1)

print("Connect " + sys.argv[1])
ser = serial.Serial(sys.argv[1], 9600, timeout=0)

while(1):
    print(">> ", end="")
    cmd = input()
    ser.write(cmd.encode())