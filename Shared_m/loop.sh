#!/bin/bash
# Path: Shared_m\script.sh

#Se creará un bucle de 100 mensajes
#Se enviará una señal de interrupción al proceso con el PID y a los 4 procesos que ejecutan las operaciones
#Los 5 procesos deben estar en EJECUCIÓN
#Se ejecutará read luego de enviar la señal de interrupción y se enviará el número de iteración a read

#Obtener el PID del proceso 1. send, 2. resend_sum, 3. resend_sub, 4. resend_mul y 5. resend_div
PID1=$1
PID2=$2
PID3=$3
PID4=$4
PID5=$5

#Si hay un 6to parámetro, se matará todos los procesos
if [ $# -eq 6 ]
then
    kill -SIGTERM $PID1
    kill -SIGTERM $PID2
    kill -SIGTERM $PID3
    kill -SIGTERM $PID4
    kill -SIGTERM $PID5
fi

echo -e "Iter\tOper\tNumber"
for i in {1..100}
do
    #Enviar señal de interrupción a los 5 procesos
    kill -SIGINT $PID1
    kill -SIGINT $PID2
    kill -SIGINT $PID3
    kill -SIGINT $PID4
    kill -SIGINT $PID5
    #Enviar el número de iteración
    ./read $i
done
#Terminar los procesos
kill -SIGTERM $PID1
kill -SIGTERM $PID2
kill -SIGTERM $PID3
kill -SIGTERM $PID4
kill -SIGTERM $PID5