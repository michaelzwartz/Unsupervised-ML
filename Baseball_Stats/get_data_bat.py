# -*- coding: utf-8 -*-
"""
Created on Thu Feb  2 19:34:08 2023

@author: Michael
"""

#from pybaseball import batting_stats

#START = 2002
#END = 2022

#batting = batting_stats(START, END, qual=200)
#batting.to_csv("batting.csv")

import os
import pandas as pd
import numpy as np

data = pd.read_csv("batting.csv")

batting = data.groupby("IDfg", group_keys=False).filter(lambda x: x.shape[0]>1)

def next_season(player):
    player=player.sort_values("Season")
    player["Next_WAR"] = player['WAR'].shift(-1)
    return player

batting= batting.groupby("IDfg", group_keys=False).apply(next_season)

null_count = batting.isnull().sum()
complete_cols = list(batting.columns[null_count == 0])
batting = batting[complete_cols + ["Next_WAR"]].copy()

#batting.dtypes[batting.dtypes == "object"]

del batting["Dol"]
del batting["Age Rng"]

#assign team codes into a set of unique numbers for each team
batting["team_code"] = batting["Team"].astype("category").cat.codes

batting_full = batting.copy()
batting = batting.dropna().copy()

from sklearn.linear_model import Ridge
from sklearn.feature_selection import SequentialFeatureSelector
from sklearn.model_selection import TimeSeriesSplit

#play around with aplha: higher --> reduces overfitting
rr = Ridge(alpha=1)

split = TimeSeriesSplit(n_splits=3)

#n_jobs uses multiple threads - direction selects the best 20
sfs = SequentialFeatureSelector(rr, n_features_to_select=20,
                                direction="forward", cv=split, n_jobs=4)

#remove columns that won't work in the machine learning model
removed_columns = ["Next_WAR", "Name", "Team", "IDfg", "Season"]
selected_columns = batting.columns[~batting.columns.isin(removed_columns)]


from sklearn.preprocessing import MinMaxScaler

#normalize the data? 
scaler = MinMaxScaler()
batting.loc[:, selected_columns] = scaler.fit_transform(batting[selected_columns])

sfs.fit(batting[selected_columns], batting["Next_WAR"])

#create a list of best features based on the ridge regression model
predictors = list(selected_columns[sfs.get_support()])

#create list of ID infomation
batIDs = ['Team','Name','Season','IDfg']
#create new list of predictor column titles
good20 = predictors.copy()

#add ID info to beginning of list
for a in batIDs:
    good20.insert(0,a)

#create new df to include ID info and normalized batting stats
batStats_clean20 = batting[good20]

batStats_clean20.to_csv("batStats_clean20.csv", index=False)