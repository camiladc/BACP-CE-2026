#!/bin/bash
set -e

maxEvaluations=10000
algo=ILS
respaldos="respaldos${algo}"
rm -rf "${respaldos}"
mkdir -p "${respaldos}"

maxSeeds=2
scenario="All.scn"
seed=1
while [ "${seed}" -lt "${maxSeeds}" ]; do
    outputTuner="ParamILS_A${algo}_IAll_S${seed}.out"
    echo "Running ParamILS seed ${seed}"
    time ruby paramils2.3.8-source/param_ils_2_3_run.rb -numRun "${seed}" -approach focused -userunlog 1 -validN 0 -pruning 0 -maxEvals "${maxEvaluations}" -scenariofile "${scenario}" > "${outputTuner}"
    mv out "${respaldos}/outA${algo}_IAll_S${seed}"
    seed=$((seed + 1))
done
