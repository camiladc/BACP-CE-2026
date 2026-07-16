# ParamILS template

This folder is a reusable ParamILS template for tuning a custom algorithm executable against a new set of instances.

## Purpose of each file

- All.params: defines the parameter space that ParamILS will explore. Each line describes one parameter, its possible values, and its default value in brackets.
- All.inst: lists the instances that the solver will be evaluated on during tuning.
- All.scn: is the main scenario file. It tells ParamILS which launcher script to use, which parameter file to read, which instance file to use, and what objective to optimize.
- ILS.sh: is the wrapper that executes your algorithm. It receives the selected parameter values from ParamILS, runs the solver, and returns a performance value.
- ToDoParamILS.sh: starts the tuning process. It launches one or more ParamILS runs and stores the output folders.
- optimos.txt: contains reference or optimal values for each instance so ParamILS can compute a quality gap.
- main: is a simple example executable. Replace it with your real solver program or update ILS.sh to call a different binary.
- README.md: explains how to adapt this template to your own project.

## What to adapt

- Replace the example (main) executable with your own compiled program.
- Place your instance files in the Instances directory.
- Edit All.params to declare the parameters to tune.
- Update ILS.sh so it invokes your executable with the correct argument parameters and order.
- Optionally edit optimos.txt with known optimal values to compute gaps.

## Typical workflow

1. Spcify your executable in ILS.sh.
2. Add your instance files to Instances/.
3. Create or edit All.inst and All.params.
4. Run ./ToDoParamILS.sh. If you want to run the process in the background you can execute it with nohup (linix)

```bash
nohup ./ToDoParamILS.sh > tmp_std_output &
```

## Analyzing the results

The ParamILS execution will take a while, however the best configuration found so will be available in the filed ended with *Algo-traj_1.txt in the folder respaldosILS/outAILS_IAll_S1/ that will be creted once the tunning proocess has started.

The final configuration will be found in the same folder but in the file ended with *Algo-result_1.txt or in the file ParamILS_AILS_IAll_S1.out at the root level.