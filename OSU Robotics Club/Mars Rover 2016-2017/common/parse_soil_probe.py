import sys

to_parse = ""

def __parse_and_show_science_data(string):
    split_data = string[3:].split(",")
    TempC = float(split_data[0])
    Moisture = float(split_data[2])
    Cond = float(split_data[4])
    PermR = float(split_data[6])
    PermI = float(split_data[8])
    Salinity = Cond * 6.4

    print("Temp in C: " + str(TempC))
    print("Moisture: " + str(Moisture * 100) + "%")
    print("Salinity: " + str(Salinity) + "g/L")

if __name__ == '__main__':
    __parse_and_show_science_data(sys.argv[1])
