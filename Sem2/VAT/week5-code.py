import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from fcmeans import FCM
#from pyclustertend import ivat
from PGM_Func import MakeDataSet, LoadDataSets

datasets = LoadDataSets(os.getcwd(), '.PGM')

def fuzzy_cluster(data, clusters, fuzzifier):
    data_df = pd.DataFrame(np.rot90(data), columns=["feature1", "feature2"])
    
    model = FCM(n_clusters = clusters, m=fuzzifier) #m is the fuzzifier
    model.fit(np.rot90(data))
    centers = model.centers
    #print("The centers are:  ", centers)
    err = model.error
    #print("The Error is:  ", err)

    labels = model.predict(np.rot90(data))
    data_df['Cluster FCM'] = labels

    x = data_df['feature1']
    y = data_df['feature2']
    label = data_df["Cluster FCM"]

    fig = plt.figure(figsize=(5,4))
    plt.scatter(x, y, c=label)
