import plotly.graph_objects as go
passnum = []
error = []
errorNoise = []

with open('results.txt') as f:
    lines = f.readlines()

    i = 0
    for line in lines:
        if line:
            if i%6 == 0:
                newline = line.strip('\npass #')
                data = [int(val) for val in newline.split('\n')]
                passnum.append(data)
            elif i%6 == 4:
                newline = line.strip('\nNet recent average error: ')
                data = [float(val) for val in newline.split('\n')]
                error.append(data)
            elif i%6 == 5:
                newline = line.strip('\nNoise error: ')
                data = [float(val) for val in newline.split('\n')]
                errorNoise.append(data)
            i = i+1
passnum = [val for passnum in passnum for val in passnum]
print(passnum, '\n')
error = [val for error in error for val in error]
print(error, '\n')
errorNoise = [val for errorNoise in errorNoise for val in errorNoise]
print(errorNoise)

fig = go.Figure()
fig.add_trace(go.Scatter(x=passnum, y=error, mode='lines',name='error'))
fig.add_trace(go.Scatter(x=passnum, y=errorNoise, mode='lines',name='Noise error'))
fig.show()