import serial
from getpass import getpass
from mysql.connector import connect, Error

ser = serial.Serial('/dev/ttyACM0',9600)#serial port
ser.flushInput() 


def main():
    
    while True:

        ser_bytes = ser.readline()#reads incoming data from the serial port 
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")#extracts the important data
        nice_bytes=float(decoded_bytes)
        with connect(
            host="192.168.100.9",#ip address of the server
            port=3306,#db port
            user="pi",#username of the db
            password="password",#user password
            database="smart_heater",#db name
        ) as connection:
            print(connection)
            insert_temp_query = """
            INSERT INTO temperatures (temperature)
            VALUES
                ({})
            """.format(nice_bytes)
            with connection.cursor() as cursor:
                cursor.execute(insert_temp_query)
                connection.commit()
        
        if nice_bytes == False:#no se si jala
            connection.close()
            break

main()