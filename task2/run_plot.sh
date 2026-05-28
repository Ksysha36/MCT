#!/bin/bash

# Путь к исполняемому файлу
EXE="./build/poisson2d"

# CSV-файл для результатов
CSV="results.csv"

# Заголовки CSV
echo "h,C_norm,L2_norm,Assembly_time,Solve_time" > $CSV

# Запускаем программу, фильтруем строки DATA и добавляем в CSV
$EXE | while read line; do
    if [[ $line == DATA:* ]]; then
        # Убираем префикс "DATA: " и заменяем пробелы на запятые
        echo "${line#DATA: }" | tr ' ' ',' >> $CSV
    fi
done

echo "Данные сохранены в $CSV"

# Построение графиков (Python)
python3 bin/plot_results.py