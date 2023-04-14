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
def LoadDataSets(path, filetype):
    datasets = {}
    i = 1 

    for file in os.listdir(path):
        # check the extension of files
        if file.endswith(filetype):
          datasets[i] =  MakeDataSet(file)
          i = i + 1

    return datasets

#find current directory
#path = os.getcwd()

#create datasets using .PGM files and LoadDataSets function
#datasets = LoadDataSets(path, '.PGM')
