#-------------------------------------------------------------
#
# Emilio Campuzano Mejia
# Daniel Fuentes Castro
# Santiago Ortiz Suzarte
# 
# Smart heater system
#
# You will need the mysql-connector-python library  
#
# This code only works if you are creating the db in the same device 
# where the db is. If you are trying to create the db from a different
# device, you will not be able to connect.
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