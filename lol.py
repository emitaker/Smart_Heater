import serial

ser = serial.Serial('COM12',9600)#serial port
ser.flushInput() 

f = 12
mystring = """
Lol
{}
chale
 """.format(f)

a = 0

while True:

    ser_bytes = ser.readline()#reads incoming data from the serial port 
    decoded_bytes = ser_bytes[0:len(ser_bytes)-1].decode("utf-8")#extracts the important data
    nice_bytes = float(decoded_bytes)
    print(nice_bytes)

    if ser_bytes == False:#no se si jala
        print("not a float")

        break

