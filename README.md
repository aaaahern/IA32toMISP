# IA32toMISP
Translate IA32 assembly to MISP assembly.

## Environment
* Machine: energon1.cims.nyu.edu
* GCC Version: gcc-6.2.0

## How to Compile
`module load gcc-6.2.0`

`make`

## Usage
`./IA32toMIPS <input_file> <output_path>`

## Test
`./run.sh` will translate all test cases in `tst` and generate output in `out`
