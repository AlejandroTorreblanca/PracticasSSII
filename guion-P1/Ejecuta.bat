@echo off

set "files=Sudoku-A1.txt Sudoku-A2.txt Sudoku-A3.txt Sudoku-A4.txt Sudoku-A5.txt 1Sudoku.txt 2Sudoku.txt 3Sudoku.txt"
set "pob=100 150"
set "op=Ruleta Torneo"	
set "pcruce=0.8 0.85 0.9 0.95"
set "pmuta=0.01 0.05 0.1 0.125 0.15"

for %%f in (%files%) do (
  for %%p in (%pob%) do (
    for %%o in (%op%) do (
      for %%c in (%pcruce%) do (
        for %%m in (%pmuta%) do (
          Sudoku.exe %%f %%p %%o %%c %%m >> Solucion_%%f
          echo(%%f %%p %%o %%c %%m )))))

exit