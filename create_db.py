#-------------------------------------------------------------
#
# Emilio Campuzano Mejia
#
# In this code I simulate the connection part to the DB of the  
# smart heater system
# This is only the DB creation step
#
#-------------------------------------------------------------

from getpass import getpass
from mysql.connector import connect, Error

try:
    with connect(
        host="localhost",#hostname where I'm going to connect
        user="emitaker",#username where I'm goint to create the DB
        password="password",#user password
    ) as connection:
        print(connection)
        create_db_query = "CREATE DATABASE smart_heater"#smart_heater is the name of the db
        with connection.cursor() as cursor:
            cursor.execute(create_db_query)

except Error as e:
    print(e)