import serial
from getpass import getpass
from mysql.connector import connect, Error

ser = serial.Serial('/dev/ttyACM0',9600)#serial port
ser.flushInput() 
ser_bytes = ser.readline()#reads incoming data from the serial port 
decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")#extracts the important data
print(decoded_bytes)

try:
    with connect(
        host="192.168.100.9",#ip address of the server
        port=3306,#db port
        user="pi",#username of the db
        password="password",#user password
        database="smart_heater",#db name
    ) as connection:
        print(connection)
        arduino_lec = 0
        insert_temp_query = """
        INSERT INTO temperatures (temperature)
        VALUES
            ({})
        """.format(arduino_lec)


        with connection.cursor() as cursor:
            cursor.execute(insert_temp_query)
            connection.commit()

except Error as e:
    print(e)