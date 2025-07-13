@echo off
setlocal

echo Enter input file (video or audio):
set /p input_file=

echo Enter start time (format hh:mm:ss):
set /p start_time=

echo Enter end time (format hh:mm:ss):
set /p end_time=

echo Enter output file name:
set /p output_file=

ffmpeg -i "%input_file%" -ss %start_time% -to %end_time% -c copy "%output_file%"

echo Done!
pause
