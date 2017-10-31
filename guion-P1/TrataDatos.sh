#!/bin/bash

FILES=(Solucion_Sudoku-A1.txt Solucion_Sudoku-A2.txt Solucion_Sudoku-A3.txt Solucion_Sudoku-A4.txt Solucion_Sudoku-A5.txt Solucion_1Sudoku.txt Solucion_2Sudoku.txt Solucion_3Sudoku.txt)

for file in ${FILES[@]}; do
	escribir=0
	while read linea; do
		if [ $escribir -eq 0 ]
		then 
			echo $linea >> Datos_$file
			escribir=10
		else
			let escribir=$escribir-1
		fi
	done < $file
done
