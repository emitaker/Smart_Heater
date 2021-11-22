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
except Error as e:
    print(e)
