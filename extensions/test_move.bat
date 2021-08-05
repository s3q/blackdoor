@echo off
copy test_virus.bat C:\Users\%USERNAME%\Desktop
powershell.exe -noexi "cd C:\Users\%USERNAME%\Desktop; test_virus.bat"
exit