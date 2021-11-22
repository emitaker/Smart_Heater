#-------------------------------------------------------------
#
# Emilio Campuzano Mejia
#
# In this code I simulate the connection part to the DB of the
# smart heater system
#
#-------------------------------------------------------------

from getpass import getpass
from mysql.connector import connect, Error

try:
    with connect(
        host="localhost",#hostname where I'm going to connect
        user="emitaker",#username where I'm goint to connect
        password="password",#user password
        database="smart_heater",#DB name where I'm going to connect
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
            connection.commit()

except Error as e:
    print(e)
