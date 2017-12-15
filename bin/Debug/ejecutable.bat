@echo off

set "files=sudo1.txt sudo2.txt sudo3.txt "
set "pob=100 150"
set "op=Ruleta Torneo"	
set "pcruce=0.8 0.85 0.9 0.95"
set "pmuta=0.01 0.05 0.1 0.125 0.15"

for %%f in (%files%) do (
  for %%p in (%pob%) do (
    for %%s in (%sel%) do (
      for %%c in (%pcruce%) do (
        for %%m in (%pmuta%) do (
          sudoku.exe  %%p %%s %%c %%m %%f>> Solucion_%%f
          echo(%%f %%p %%s %%c %%m )))))

exit