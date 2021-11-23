#-------------------------------------------------------------
#
# Emilio Campuzano Mejia
#
# In this code I simulate the insert into command
# smart heater system
#
#-------------------------------------------------------------

from getpass import getpass
from mysql.connector import connect, Error

try:
    with connect(
        host="192.168.100.7",#ip address of the server
        port=3306,#db port
        user="pi",#username of the db
        password="password",#user password
        database="smart_heater",#db name
    ) as connection:
        print(connection)
        create_temp_table_query ="""
        CREATE TABLE temperatures(
            id INT AUTO_INCREMENT PRIMARY KEY,
            temperature FLOAT
        )
        """
        insert_temp_query = """
        INSERT INTO temperatures (temperature)
        VALUES
            (19.94),
            (20.09),
            (20.15)
        """


        with connection.cursor() as cursor:
            cursor.execute(create_temp_table_query)
            cursor.execute(insert_temp_query)
            connection.commit()

except Error as e:
    print(e)
