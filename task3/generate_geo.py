import csv

kolomna_x = 32.549019607843135
kolomna_y = 14.880952380952387

points = []
with open('Dataset.csv', 'r') as f:
    reader = csv.reader(f, delimiter=';')
    next(reader)
    for row in reader:
        x_str = row[0].strip().replace(',', '.')
        y_str = row[1].strip().replace(',', '.')
        x = float(x_str)
        y = float(y_str)
        points.append((x, y))


if points[0] != points[-1]:
    points.append(points[0])

n = len(points)
n_lines = n - 1

with open('mosreg_colomna.geo', 'w') as geo:
    geo.write("SetFactory(\"OpenCASCADE\");\n\n")
    
    for i, (x, y) in enumerate(points, start=1):
        geo.write(f"Point({i}) = {{{x}, {y}, 0, 1.0}};\n")
    geo.write("\n")
    
    for i in range(1, n_lines+1):
        geo.write(f"Line({i}) = {{{i}, {i+1}}};\n")
    geo.write("\n")
    
    line_list = ", ".join(str(i) for i in range(1, n_lines+1))
    geo.write(f"Curve Loop(1) = {{{line_list}}};\n")
    geo.write("Plane Surface(1) = {1};\n\n")
    
    geo.write(f"Point(1000) = {{{kolomna_x}, {kolomna_y}, 0, 0.05}};\n")
    geo.write("Point{1000} In Surface{1};\n")
    
print("Saved in mosreg_colomna.geo")