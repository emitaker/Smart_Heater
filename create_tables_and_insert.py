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
        """#query that creates the temperatures table
        arduino_lec = 0
        insert_temp_query = """
        INSERT INTO temperatures (temperature)
        VALUES
            ({})
        """.format(arduino_lec)


        with connection.cursor() as cursor:
            cursor.execute(create_temp_table_query)
            cursor.execute(insert_temp_query)
            connection.commit()

except Error as e:
    print(e)
