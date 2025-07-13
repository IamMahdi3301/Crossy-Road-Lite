@echo off
setlocal

echo Enter input video file (with extension):
set /p input_file=

echo Choose output audio format (mp3 / ogg / wav):
set /p format=

echo Enter output file name (without extension):
set /p output_name=

ffmpeg -i "%input_file%" "%output_name%.%format%"

echo Done!
pause
