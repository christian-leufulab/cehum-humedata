#!/usr/bin/env python 1
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import sys
import MySQLdb
import json

# Abrir conexión con bases de datos
try:
    db = MySQLdb.connect("127.0.0.1","root","ittfly97","mqtt")
except:
    print("No se pudo conectar con la base de datos")
    print("Cerrando...")
    sys.exit()

# Preparando cursor
cursor = db.cursor()

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Conectado - Codigo de resultado: "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("v3/cehum-humedata@ttn/devices/humedata-atlas/up")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
#    print(msg.topic+" "+str(msg.payload))
#    print(msg.topic)
    response = json.loads(msg.payload.decode("utf-8"))
    dev_eui = response['end_device_ids']['dev_eui']
    response = response['uplink_message']['decoded_payload']
    print(response)
    lista = msg.topic.split("/")
    
    #query =  SELECT id FROM Humedata_devices WHERE dev_eui=dev_eui(variable);

    #sql = """INSERT INTO `mqtt`.`logs` (`ap`, `at`, `bl`, `do`, `ec`, `ih`, `ip`, `it`, `lat`, `long`, `orp`, `ph`, `rd`, `sal`, `tds`, `wt`) VALUES (""" + response['ap'] + """, """ + response['at'] + """, """ + response['bl'] + """, """ + response['do'] + """, """ + response['ec'] + """, """ + response['ih'] + """, """ + response['ip'] + """, """ + response['it'] + """, """ + response['lat'] + """, """ + response['lon'] + """, """ + response['orp'] + """, """ + response['ph'] + """, """ + response['rd'] + """, """ + response['sal'] + """, """ + response['tds'] + """, """ + response['wt'] + """);"""
    sql = """INSERT INTO `mqtt`.`logs` (`atmospheric_pressure`, `atmospheric_temperature`, `battery_level`, `dissolved_oxygen`, `electrical_conductivity`, `internal_humidity`, `internal_pressure`, `internal_temperature`, `latitude`, `longitude`, `oxide_reduction_potential`, `ph`, `relative_density`, `salinity`, `total_dissolved_solids`, `water_temperature`, `dev_id`) VALUES (""" + str(response['ap']) + """, """ + str(response['at']) + """, """ + str(response['bl']) + """, """ + str(response['do']) + """, """ + str(response['ec']) + """, """ + str(response['ih']) + """, """ + str(response['ip']) + """, """ + str(response['it']) + """, """ + str(response['lat']) + """, """ + str(response['lon']) + """, """ + str(response['orp']) + """, """ + str(response['ph']) + """, """ + str(response['rd']) + """, """ + str(response['sal']) + """, """ + str(response['tds']) + """, """ + str(response['wt']) + """, """ + str(1) + """);"""

    try:
        # Ejecutar un comando SQL
        cursor.execute(sql)
        db.commit()
        print("Guardando en base de datos...OK")
    except:
        db.rollback()
        print("Guardando en base de datos...Falló")
        
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set("cehum-humedata@ttn", "NNSXS.LU25SCKHR4SWWEB3FZ7AMQFSI55Z36MT5X6GVGQ.NKVALYYY4VB7KSJ4NXWTF3DE33AIPR6OBWP7NBV4EGAMQG7ZAFNQ")
try:
    client.connect("au1.cloud.thethings.network", 1883, 60)
except:
    print("No se pudo conectar con el MQTT Broker...")
    print("Cerrando...")
    db.close()
    sys.exit()   
    

try:
    client.loop_forever()
except KeyboardInterrupt:  #precionar Crtl + C para salir
    print("Cerrando...")
    db.close()
