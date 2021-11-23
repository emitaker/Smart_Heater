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
        host="192.168.100.7",#ip address of the server
        port=3306,#db port
        user="pi",#username of the db
        password="password",#user password
    ) as connection:
        print(connection)
        create_db_query = "CREATE DATABASE smart_heater"#smart_heater is the name of the db
        with connection.cursor() as cursor:
            cursor.execute(create_db_query)

except Error as e:
    print(e)