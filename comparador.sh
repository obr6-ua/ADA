#!/bin/bash

# Directorio donde están los archivos .map para las pruebas
TEST_DIR="/home/omar/Escritorio/ADA/pruebas"

EXEC2="./pracFinalOptimizada"


RESULTS2="results_pracFinalOptimizada.txt"


echo "" > $RESULTS2

# Definir límite de tiempo en segundos
TIME_LIMIT=30

# Bucle sobre los archivos .map en el directorio de pruebas
for test_file in $TEST_DIR/*.map; do
    # Ignorar archivos .sol_bb
    if [[ $test_file == *".sol_bb" ]]; then
        continue
    fi

    echo "Testing with file: $test_file"

    # Correr segunda versión del programa con timeout
    echo "Running $EXEC2 on $test_file"
    { time timeout $TIME_LIMIT $EXEC2 -f $test_file; } 2>&1 | grep real | awk '{print $2}' > temp_time.txt
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
        echo "$test_file: EXCEEDED TIME LIMIT" >> $RESULTS2
        echo "WARNING: $EXEC2 exceeded 30 seconds on $test_file"
    else
        duration=$(cat temp_time.txt)
        echo "$test_file: $duration" >> $RESULTS2
    fi
    rm temp_time.txt
done

# Comparar los tiempos de ambas ejecuciones
echo "Results for $EXEC2"
cat $RESULTS2
