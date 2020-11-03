### Data Collection
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

df=pd.read_csv(r"C:\\Users\\Dhruv\\Desktop\\Shortcuts\\pro\\AAPL.csv")

print("\nThe First 5 Entries: \n")
print(df.head())

# df.tail()

df1=df.reset_index()['close'] #Reset values would be incorporated along with their indices

print("\nWe're Going to Predict Based on Closing prices of the stocks\n")
print("\nThe Close Column (First 5 Entries): \n")

print(df1.head())

print("\n\nTotal Number of Entries: ")
print(df1.shape)

input("\nPress Enter Key to View the Plot of Closing Prices wrt Time....")

# plt.style.use('seaborn')
# fig = plt.figure()
# ax = fig.add_subplot(1,1,1)


# def animation(i):
#   cp = pd.read_csv(r"C:\\Users\\Dhruv\\Desktop\\Shortcuts\\pro\\close_prices.csv")
#   #x = []
#   y = []

#   #x = cp[0:i]['date']
#   y = cp[0:i]['close']
#   ax.clear()
#   ax.plot(y)

# animation = FuncAnimation(fig, func=animation, interval=1)
# plt.show()

#Normal Plot Without any Animations
plt.plot(df1)
plt.title("Variation of Close Prices with time")
plt.xlabel("Entries")
plt.ylabel("Close Prices")
plt.show()



### LSTM are sensitive to the scale of the data. so we apply MinMax scaler



from sklearn.preprocessing import MinMaxScaler
'''To Scale The Values Between 0 to 1'''
scaler=MinMaxScaler(feature_range=(0,1))
df1=scaler.fit_transform(np.array(df1).reshape(-1,1))
'''After This Step it converts into an array with values scaled from 0 to 1 '''

# print(df1)

#splitting dataset into train and test split (dividing the data based on date since the data is dependent to previous data)

training_size=int(len(df1)*0.65)
test_size=len(df1)-training_size
train_data,test_data=df1[0:training_size,:],df1[training_size:len(df1),:1]

training_size,test_size

'''we'll basically train the data so that it can predict the next value based on the previous values'''

import numpy
# convert an array of values into a dataset matrix
def create_dataset(dataset, time_step=1):
	dataX, dataY = [], []
	for i in range(len(dataset)-time_step-1):
		a = dataset[i:(i+time_step), 0]   ###i=0, 0,1,2,3-----99   100 steps to be made
		dataX.append(a)
		dataY.append(dataset[i + time_step, 0])
	return numpy.array(dataX), numpy.array(dataY)

# reshape into X=t,t+1,t+2,t+3 and Y=t+4
time_step = 100
X_train, y_train = create_dataset(train_data, time_step)
X_test, ytest = create_dataset(test_data, time_step)

print(X_train.shape), print(y_train.shape)

print(X_test.shape), print(ytest.shape)

# reshape input to be [samples, time steps, features] which is required for LSTM
X_train =X_train.reshape(X_train.shape[0],X_train.shape[1] , 1)
X_test = X_test.reshape(X_test.shape[0],X_test.shape[1] , 1)

### Create the Stacked LSTM model
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import LSTM

model=Sequential()
model.add(LSTM(50,return_sequences=True,input_shape=(100,1)))
model.add(LSTM(50,return_sequences=True))
model.add(LSTM(50))
model.add(Dense(1))
model.compile(loss='mean_squared_error',optimizer='adam')

model.summary()

model.fit(X_train,y_train,validation_data=(X_test,ytest),epochs=100,batch_size=64,verbose=1)

import tensorflow as tf

### Lets Do the prediction and check performance metrics
train_predict=model.predict(X_train)
test_predict=model.predict(X_test)

##Transformback to original form
train_predict=scaler.inverse_transform(train_predict)
test_predict=scaler.inverse_transform(test_predict)

### Calculate RMSE performance metrics
import math
from sklearn.metrics import mean_squared_error
math.sqrt(mean_squared_error(y_train,train_predict))

### Test Data RMSE
math.sqrt(mean_squared_error(ytest,test_predict))

### Plotting 
# shift train predictions for plotting
look_back=100
trainPredictPlot = numpy.empty_like(df1)
trainPredictPlot[:, :] = np.nan
trainPredictPlot[look_back:len(train_predict)+look_back, :] = train_predict
# shift test predictions for plotting
testPredictPlot = numpy.empty_like(df1)
testPredictPlot[:, :] = numpy.nan
testPredictPlot[len(train_predict)+(look_back*2)+1:len(df1)-1, :] = test_predict
# plot baseline and predictions
plt.plot(scaler.inverse_transform(df1))
plt.plot(trainPredictPlot)
plt.plot(testPredictPlot)
plt.show()

# len(test_data)

x_input=test_data[341:].reshape(1,-1)
x_input.shape

temp_input=list(x_input)
temp_input=temp_input[0].tolist()

temp_input

# demonstrate prediction for next 30 days
from numpy import array

lst_output=[]
n_steps=100
i=0
while(i<30):
    
    if(len(temp_input)>100):
        #print(temp_input)
        x_input=np.array(temp_input[1:])
        # print("{} day input {}".format(i,x_input))
        x_input=x_input.reshape(1,-1)
        x_input = x_input.reshape((1, n_steps, 1))
        #print(x_input)
        yhat = model.predict(x_input, verbose=0)
        # print("{} day output {}".format(i,yhat))
        temp_input.extend(yhat[0].tolist())
        temp_input=temp_input[1:]
        #print(temp_input)
        lst_output.extend(yhat.tolist())
        i=i+1
    else:
        x_input = x_input.reshape((1, n_steps,1))
        yhat = model.predict(x_input, verbose=0)
        # print(yhat[0])
        temp_input.extend(yhat[0].tolist())
        # print(len(temp_input))
        lst_output.extend(yhat.tolist())
        i=i+1
    

# print(lst_output)

day_new=np.arange(1,101)
day_pred=np.arange(101,131)

# len(df1)

plt.plot(day_new,scaler.inverse_transform(df1[1158:]))
plt.plot(day_pred,scaler.inverse_transform(lst_output))
plt.show()

df3=df1.tolist()
df3.extend(lst_output)
plt.plot(df3[1200:])

df3=scaler.inverse_transform(df3).tolist()
# df3.to_csv('final.csv')
plt.plot(df3)
plt.show()

