Se solicitó crear un bucle de 100 iteraciones.
El comando para realizar los bucles se encuentra en:
    - loop.sh

Pasos para la ejecución:
    - Ejecutar el make.
    - Ejecutar ./send
    - Ejecutar ./resend 0
    - Ejecutar ./resend 1
    - Ejecutar ./resend 2
    - Ejecutar ./resend 3
        # Éstos resend se ejecutan para cada operación.
    - Ejecutar . loop.sh pid_send pid_resend0 pid_resend1 pid_resend2 pid_resend3
        # En total se le pasará 5 ID de los procesos que se están ejecutando.

Luego de realizar las iteraciones, el loop.sh terminará los procesos.