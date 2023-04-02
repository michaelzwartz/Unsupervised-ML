#import libraries
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#load dataset
data = pd.read_csv("batStats_clean20.csv")

id_columns = ['IDfg', 'Season', 'Name', 'Team']
data_columns = ['Age','IBB','SO','SB','BU','BABIP','IFH%','WAR','Spd','PH','CB%',
    'O-Contact%','wGDP','Oppo%','OBP+','SLG+','Pull%+','Soft%+','Hard%+','L-WAR']

#remove the Type classification feature from the data set for PCA analysis
PCA_batStats = data[data_columns]

#calculate the eigenvalues and eigenvectors
eigen_val, eigen_vec = np.linalg.eig(PCA_batStats.cov())

#sort the eigenvalues from highest to lowest
sorted_index = np.argsort(eigen_val)[::-1]
sorted_eigenvalue = eigen_val[sorted_index]

#sort the eigenvectors to match values
sorted_eigenvec = eigen_vec[:,sorted_index]

#select eigenvector to reduce the data set to 1D
w1 = sorted_eigenvec[:, 0:1]

#dot productor of eigenvector and dataset
batStats1D_PCA = np.dot(w1.transpose(), PCA_batStats.transpose()).transpose()

#create plot
plot1 = plt.hist(batStats1D_PCA, density=True)

#select eigenvector to reduce the data set to 2D
w2 = sorted_eigenvec[:, 0:2]

#dot productor of eigenvector and dataset
batStats2D_PCA = np.dot(w2.transpose(), PCA_batStats.transpose()).transpose()

#create plot
plot2 = plt.scatter(batStats2D_PCA[:,0], batStats2D_PCA[:,1])

#select eigenvector to reduce the data set to 3D
w3 = sorted_eigenvec[:, 0:3]

#dot productor of eigenvector and dataset
batStats3D_PCA = np.dot(w3.transpose(), PCA_batStats.transpose()).transpose()

#create plot
plot3 = plt.figure()
ax = plot3.add_subplot(projection='3d')
ax.scatter(batStats3D_PCA[:,0], batStats3D_PCA[:,1], batStats3D_PCA[:,2])
plt.show()


#LDA 
LDA_batStats = data[['SO','BABIP', 'OBP+','WAR']]

#define features 
X1 = data['SO']
X2 = data['BABIP']
X3 = data['OBP+']

target = data['WAR']

#create 3 classes based on WAR. Bottom 25%, Middle 50%, Top 25%. 
bottom25 = LDA_batStats.loc[LDA_batStats["WAR"] < 0.248447]
middle50 = LDA_batStats.loc[(LDA_batStats["WAR"] > 0.248447) 
                            & (LDA_batStats["WAR"]<0.403727)]
top25 = LDA_batStats.loc[LDA_batStats["WAR"] > 0.403727]

#pull sample of 1000 players from each group
bottom_tier = bottom25.sample(n=1000)
mid_tier = middle50.sample(n=1000)
top_tier = top25.sample(n=1000)

#find class averages
mu1 = np.average(bottom_tier)
mu2 = np.average(mid_tier)
mu3 = np.average(top_tier)

mu = np.average(LDA_batStats)

#calcualte covariance matrix
S1 = np.cov(bottom_tier)
S2 = np.cov(mid_tier)
S3 = np.cov(top_tier)

#sum covariance matricies
Sw = S1 + S2 + S3

#square the sum 
Sb = (mu1-mu2-mu3)*(mu1-mu2-mu3)

#take the inverse 
invSw = np.linalg.inv(Sw)
invSw2 = invSw * Sb 

#calculate eigenvalues
v, d = np.linalg.eig(invSw2)
w = v[:1]
#multiply featurs by eigenvectors 
y1 = X1*w
y2 = X2*w
y3 = X3*w

#ignore copy warning 
import warnings
warnings.filterwarnings('ignore')

#combine clases into one data series 
batStats1D_LDA1 = y1.append([y2, y3], ignore_index=True)

#plot series 
plot1 = plt.hist(batStats1D_LDA1, density=True)