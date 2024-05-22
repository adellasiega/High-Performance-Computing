#!/bin/bash
#SBATCH --job-name=e1HPC
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --time=02:00:00
#SBATCH --partition THIN
#SBATCH --exclusive
#SBATCH --exclude fat[001-002]

module load openMPI/4.1.5/gnu/12.2.1

echo "Processes,Size,Latency" > reduce1_core_thin.csv

# Numero di ripetizioni per ottenere una media
reps=10000

# Ciclo esterno per il numero di processori
for procs in {2..48..2}
do
    # Ciclo interno per la dimensione del messaggio da 2^1 a 2^20
    for size_power in {1..20}
    do
        # Calcola la dimensione come 2 elevato alla potenza corrente
        size=$((2**size_power))

        # Esegui osu_reduce con numero di processi, dimensione fissa e numero di ripetizioni su due nodi
        result=$(mpirun --map-by socket -np $procs --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_reduce_algorithm 4 osu_reduce -m $size -x $reps -i $reps | tail -n 1 | awk '{print $2}')
	
        # Scrivi i risultati nel file CSV
        echo "$procs,$size,$result" >> reduce4_core_thin.csv
    done
done
