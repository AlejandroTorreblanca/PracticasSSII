@echo off

setlocal EnableDelayedExpansion

set "files=Solucion_Sudoku-A1.txt Solucion_Sudoku-A2.txt Solucion_Sudoku-A3.txt Solucion_Sudoku-A4.txt Solucion_Sudoku-A5.txt Solucion_1Sudoku.txt Solucion_2Sudoku.txt Solucion_3Sudoku.txt"

set "escribir=0"

for %%f in (%files%) do (
  echo %%f
  for /f "tokens=*" %%l in (%%f) do (
    if !escribir!==0 (
      echo %%l >> Datos_%%f
      set "escribir=9"
    ) else (
      set /a "escribir-=1"
    )
  )
)

exit