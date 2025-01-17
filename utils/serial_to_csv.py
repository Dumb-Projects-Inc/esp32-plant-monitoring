import serial
import argparse

parser = argparse.ArgumentParser(
                    prog='Serial to JSON',
                    description='This program reads the ESP32 DTU course 02112',
                    epilog='Use at you own risk')
parser.add_argument("COM", help="The COM port to read from")
parser.add_argument("output_file", help="The output file to be written to")

def main(args):
    port = args.COM
    out = args.output_file
    with open(out, 'a') as f:
        f.write("light, temp, humidity, soil_humidity, soil_temperature\n")
        with serial.Serial(port, 115200, timeout=1) as ser:
            while True:
                line = ser.readline().decode('utf-8')
                line = line.strip()
                
                if "SENSOR_VALS" in line:
                    line = line.split(": ")[1]
                    print(line)
                    f.write(line)
                    f.write('\n')

if __name__ == '__main__':
    args = parser.parse_args()
    main(args)