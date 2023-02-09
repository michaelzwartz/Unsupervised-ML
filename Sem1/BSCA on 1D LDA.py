# -*- coding: utf-8 -*-
"""
Created on Mon Mar 28 23:00:59 2022

@author: oabuomar
"""

import pandas as pd
import numpy as np
from statistics import mean
import plotly.graph_objects as go
import plotly.express as px

#USER PRARAMETERS DEFINED
#m = max number of clusters set by user
m=3
#a = alpha, threshold of dissimilarity set by user
a=0.9

#load data set
df=pd.read_csv('LDA_Iris.csv')
#t = counter, counts number of iterations or clusters created
t=1

#initialize cluster dictionary
clusters = dict(zip(range(m), [[] for i in range(m)]))

#assign the first data point to cluster 1
clusters[0].append(df.LD1[0])

#iterate through data points after the first one
for row in df.LD1[1:]:
#add at least 1 data point to each cluster   
#if counter is less than max clusters aka not all clusters have been instantiated
    c=0 
    currentclusters=t
    distance = dict(zip(range(currentclusters), [[] for i in range(currentclusters)]))
    for c in range(currentclusters):
        distance[c].append(abs(row-mean(clusters[c])))
    shortest_dist=min(distance.values())
    closest_cluster=list(distance.keys())[list(distance.values()).index(shortest_dist)]
    if t<m and shortest_dist[0] >a:
        print('cluster ' + str(c+1) + ' is made')
        clusters[currentclusters].append(row)
        t=t+1
#after all clusters have at least one data point sort the rest of the data point by choosing cluster with closest centroid
    else:  
        clusters[closest_cluster].append(row)
print(clusters)

c_df=pd.DataFrame()
for c in clusters:
    for i in clusters[c]:
        c_df=c_df.append({'Value': i, 'Cluster': c}, ignore_index=True)

fig=go.Figure()

fig=px.scatter(x=c_df.Value, color=c_df.Cluster, color_continuous_scale=['red','green','blue'], title="BSCA with LDA Iris Data alpha=" + str(a) + ", max clusters=" + str(m))
fig.update_layout(coloraxis_colorbar=dict(
    title="Clusters",
    tickvals=[0,1,2,3, 4],
    ticktext=["c1","c2","c3","c4", "c5"],
    lenmode="pixels", len=500,
))
#fig.add_trace(go.scatter(x=clusters[1], y=range(len(clusters[0]))))
fig.write_html('result2.html', auto_open=True)