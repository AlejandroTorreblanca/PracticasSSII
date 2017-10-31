#!/bin/bash


FILES=(Sudoku-A1.txt Sudoku-A2.txt Sudoku-A3.txt Sudoku-A4.txt Sudoku-A5.txt 1Sudoku.txt 2Sudoku.txt 3Sudoku.txt)

tamPOB=(100 150)
OP=(Ruleta Torneo) # GARouletteWheelSelector GATournamentSelector
probCRUCE=(0.8 0.85 0.9 0.95)
probMUTACION=(0.01 0.05 0.1 0.125 0.15)


for file in ${FILES[@]}; do
        for tam in ${tamPOB[@]}; do
        for op in ${OP[@]}; do
            for cruce in ${probCRUCE[@]}; do
                for mut in ${probMUTACION[@]}; do

                    ./Sudoku.out $file $tam $op $cruce $mut &>> ../Solucion_$file
                    echo "" >>Solucion_$file
                    echo "$file $tam $op $cruce $mut"
                done          
            done 
        done    
    done
done
