import matplotlib.pyplot as plt
import numpy as np 
import os 

#create function to make datasets
def MakeDataSet(filename):
    Img = plt.imread(filename)
    [R, C] = np.where(Img == 0)  
    X = np.array([R, C])
    plt.figure()
    plt.scatter(X[0], X[1])  

    return X

#create function to use MadeDataSet function only for PGM files
def LoadDataSets():
    datasets = {}
    i = 1 

    path = os.getcwd()

    for file in os.listdir(path):
        # check the extension of files
        if file.endswith('.PGM'):
          datasets[i] =  MakeDataSet(file)
          i = i + 1

    return datasets

#Run the function to create a dictionary of datasets
datasets = LoadDataSets()

#import preclustering packages
from pyclustertend import vat
from pyclustertend import ivat

#run VAT for each dataset
for set in datasets:
    try:
        print("VAT for Dataset #", set, "all good.")
        vat(np.rot90(datasets[set]))
    except IndexError:
        print("Dataset #", set, "not rotated. VAT not completed")

#run iVAT for each dataset.
for set in datasets:
    try:
        print("iVAT for Dataset #", set, "all good")
        ivat(np.rot90(datasets[set]))
    except IndexError:
        print("Dataset #", set, "not rotated. iVAT not completed.")