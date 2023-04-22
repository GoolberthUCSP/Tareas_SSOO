#Se creará un bucle de 100 mensajes
#Se enviará una señal de interrupción al proceso con el PID pasado como parámetro
#Se ejecutará rec luego print y se enviará el número de iteración a print

#!/bin/bash
# Path: Msg\script.sh
#Obtener el PID
PID=$1
echo "Iter\tOper\tNumber"
for i in {1..100}
do
    #Enviar señal de interrupción
    kill -SIGINT $PID
    ./rec
    #Enviar el número de iteración
    ./print $i
done