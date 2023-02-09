#import libraries
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#load dataset
stars = pd.read_csv("Stars.csv")

#create list of colors from dataset
color = ['Red', 'Blue White', 'White', 'Yellowish White', 'Blue white',
       'Pale yellow orange', 'Blue', 'Blue-white', 'Whitish',
       'yellow-white', 'Orange', 'White-Yellow', 'white', 'yellowish',
       'Yellowish', 'Orange-Red', 'Blue-White']

#create list of spectarl classes in dataset
spectral_class = ['O', 'B', 'A', 'F', 'G', 'K', 'M']
#https://en.wikipedia.org/wiki/Asteroid_spectral_types

#create dictionary for star types
type_dict = {0: 'Red Dwarf', 1: "Brown Dwarf", 2: "White Dwarf", 3: "Main Sequence",
             4: "Super Giants", 5: "Hyper Giants"}

#replace color strings with number as index from color list
for clr in stars['Color']:
    stars['Color'] = stars['Color'].replace(clr, color.index(clr))
    
#replace spectral class category character with number as index from spectral class list
for sp_cla in stars['Spectral_Class']:
    stars['Spectral_Class'] = stars['Spectral_Class'].replace(sp_cla, spectral_class.index(sp_cla))


#remove the Type classification feature from the data set for PCA analysis
PCA_stars = stars[['Temperature', 'L', 'R', 'A_M', 'Color', 'Spectral_Class']]

#calculate the eigenvalues and eigenvectors
eigen_val, eigen_vec = np.linalg.eig(PCA_stars.cov())

#sort the eigenvalues from highest to lowest
sorted_index = np.argsort(eigen_val)[::-1]
sorted_eigenvalue = eigen_val[sorted_index]

#sort the eigenvectors to match values
sorted_eigenvec = eigen_vec[:,sorted_index]

#select eigenvector to reduce the data set to 1D
w1 = sorted_eigenvec[:, 0:1]

#dot productor of eigenvector and dataset
stars1D = np.dot(w1.transpose(), PCA_stars.transpose()).transpose()

#create plot
plot1 = plt.hist(stars1D, density=True)

#select eigenvector to reduce the data set to 2D
w2 = sorted_eigenvec[:, 0:2]

#dot productor of eigenvector and dataset
stars2D = np.dot(w2.transpose(), PCA_stars.transpose()).transpose()

#create plot
plot2 = plt.scatter(stars2D[:,0], stars2D[:,1])

#select eigenvector to reduce the data set to 3D
w3 = sorted_eigenvec[:, 0:3]

#dot productor of eigenvector and dataset
stars3D = np.dot(w3.transpose(), PCA_stars.transpose()).transpose()

#create plot
plot3 = plt.figure()
ax = plot3.add_subplot(projection='3d')
ax.scatter(stars3D[:,0], stars3D[:,1], stars3D[:,2])
plt.show()


#LDA 
LDA_stars = stars[['Temperature', 'L', 'A_M', 'Type']]

#define features 
X1 = stars['Temperature']
X2 = stars['L']
X3 = stars['A_M']

target = stars['Type']

Red_Dwarf = LDA_stars[LDA_stars['Type']==0]
#Brown_Dwarf = LDA_stars[LDA_stars['Type']==1]
#White_Dwarf = LDA_stars[LDA_stars['Type']==2]
Main_Sequence = LDA_stars[LDA_stars['Type']==3]
#Super_Giant = LDA_stars[LDA_stars['Type']==4]
Hyper_Giant = LDA_stars[LDA_stars['Type']==5]

#find class averages
mu1 = np.average(Red_Dwarf)
mu2 = np.average(Main_Sequence)
mu3 = np.average(Hyper_Giant)

mu = np.average(LDA_stars)

#calcualte covariance matrix
S1 = np.cov(Red_Dwarf)
S2 = np.cov(Main_Sequence)
S3 = np.cov(Main_Sequence)

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

#create plot
plot4 = plt.figure()
ax = plot4.add_subplot(projection='3d')
ax.scatter(y1, y2, y3)
plt.show()