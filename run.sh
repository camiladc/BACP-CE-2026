#!/bin/bash
#seed=$1
seeds=(121 255 11 123 501 135 108 268 151 326)

# Directorios de input y output
instances_dir="benchmark-instances"
output_dir="benchmark-best-outputs"

# Parámetros del algoritmo
max_gen=600
popsize=25
period_multiplier=1
cross_prob=0.2
mut_prob=0.7
mut_move_course_prob=0.2
mut_branch_prob=0.6
mut_insert_prob=0.1

mkdir -p "${output_dir}"

# Ciclo por cada semilla
for seed in "${seeds[@]}"; do
  # Ciclo para cada instancia
  for instance_path in "${instances_dir}"/*; do

    instance=$(basename "${instance_path}" .txt)
    output_file="${output_dir}/best_${instance}_${seed}.csv"

    ./main "${instance_path}" "${seed}" "${max_gen}" "${popsize}" \
          "${period_multiplier}" "${cross_prob}" "${mut_prob}" \
          "${mut_move_course_prob}" "${mut_branch_prob}" "${mut_insert_prob}" \
      > "${output_file}"

    echo "Generado: ${output_file}"
  done
done