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
# To establish a connection with the database, you have to run these queries:
# CREATE USER 'pi'@'pi ip address' IDENTIFIED BY 'password'
# GRANT ALL PRIVILEGES ON databasename.* TO 'pi'@'pi ip address'
# After that, you can run the code below 
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
except Error as e:
    print(e)
