# Servicio de subscripción MQTT para sistema humedat@

El presente repositorio contiene el código necesario para implementar un servicio en Python3 para subscribirse al broker MQTT de humedat@.

## Requisitos previos

Instalar previamente los siguientes programas y librerías:

* Python 3: https://www.python.org/downloads/
* MySQL (server y client): 
  * https://dev.mysql.com/downloads/
  * entorno para XAMPP para Windows: https://www.apachefriends.org/es/index.html
* Eclipse Paho: https://www.eclipse.org/paho/index.php?page=clients/python/docs/index.php#installation
* PyMySQL mysqlclient: https://github.com/PyMySQL/mysqlclient

Usar el gestor de paquetes [pip](https://pip.pypa.io/en/stable/) para instalar las siguientes bibliotecas Python.

```bash
pip install paho-mqtt mysqlclient
```

### Preparación previa

Una vez instalado MySQL, importar la base de datos del archivo `db.sql`.

## Uso

Ejecutar archivo `subscribe_and_insert_to_db.py`

En linux:
```bash 
python3 subscribe_and_insert_to_db.py
```

El script está diseñado para ejecutarse manualmente, pero con ligeras modificaciones y configuraciones de sistema se podrá ejecutar como servicio.