#-------------------------------------------------------------
#
# Emilio Campuzano Mejia
#
# In this code I simulate the connection part to the DB of the
# smart heater system
#
#-------------------------------------------------------------
#CREATE USER 'pi'@'192.168.100.35' IDENTIFIED BY 'password';
#GRANT ALL PRIVILEGES ON databasename.* TO 'pi'@'192.168.100.35';

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
except Error as e:
    print(e)
