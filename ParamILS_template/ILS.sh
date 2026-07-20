#/bin/bash

dirInstances="Instances"
instance=$1
seed=$5
shift 5

#maximo de evaluaciones totales
#evaluactiones=1000

while [ $# != 0 ]; do
    flag="$1"
    case "$flag" in
        -max_gen) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              max_gen=$arg
            fi
            ;;
        -popsize) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              popsize=$arg
            fi
            ;;
        -cross_prob) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              cross_prob=$arg
            fi
            ;;
        -mut_prob) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              mut_prob=$arg
            fi
            ;;
        -mut_move_course_prob) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              mut_move_course_prob=$arg
            fi
            ;;
        -mut_branch_prob) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              mut_branch_prob=$arg
            fi
            ;;
        -mut_insert_prob) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              mut_insert_prob=$arg
            fi
            ;;
        *) echo "Unrecognized flag or argument: $flag"
            ;;
        esac
    shift
done

#max_iter=$(awk "BEGIN {printf \"%d\",(${evaluaciones}/${max_gen})}")

#max_gen popsize period_multiplier cross_prob mut_prob mut_move_course_prob mut_branch_prob mut_insert_prob

params="${max_gen} ${popsize} 1 ${cross_prob} ${mut_prob} ${mut_move_course_prob} ${mut_branch_prob} ${mut_insert_prob}"
screen=instance_output

rm -rf ${screen}

echo "./main ${dirInstances}/${instance} ${seed} ${params} > ${screen}"
./main ${dirInstances}/${instance} ${seed} ${params} > ${screen}

# get quality from the Fitness value printed by the program
quality=$(grep -E '^[[:space:]]*Fitness:' "${screen}" | tail -n 1 | awk '{print $NF}')

if [ -z "${quality}" ]; then
    echo "Could not find Fitness value in ${screen}" >&2
    exit 1
fi

#buscar optimo en archivo
optimo=""
found_optimo=0

while read line
do
    set -- $line
    name=$1
    if [[ ${instance} == ${name} ]];
    then
        optimo=$2
        found_optimo=1
        echo "nombre: ${name}, optimo: ${optimo} quality: ${quality}" 
        break
    fi
done < "optimos.txt"

if [ "${found_optimo}" -ne 1 ]; then
    echo "Could not find optimum for instance ${instance} in optimos.txt" >&2
    exit 1
fi

gap=$(awk "BEGIN {printf \"%.2f\",100.00*(${quality}-${optimo})/${optimo}}")
# replace first blank only
#bar=${foo/ /.}
runlength=${gap/,/.}

solved="SAT"
runtime=0
best_sol=0


echo "Result for ParamILS: ${solved}, ${runtime}, ${runlength}, ${best_sol}, ${seed}"




