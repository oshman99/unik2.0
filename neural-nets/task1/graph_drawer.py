import plotly.graph_objects as go
passnum = []
error = []

with open('result.txt') as f:
    lines = f.readlines()

    i = 0
    for line in lines:
        if line:
            if i%5 == 0:
                newline = line.strip('\npass #')
                data = [int(val) for val in newline.split('\n')]
                passnum.append(data)
            if i%5 == 4:
                newline = line.strip('\nNet recent average error: ')
                data = [float(val) for val in newline.split('\n')]
                error.append(data)
            i = i+1
passnum = [val for passnum in passnum for val in passnum]
print(passnum, '\n')
error = [val for error in error for val in error]
print(error)

fig = go.Figure()
fig.add_trace(go.Scatter(x=passnum, y=error, mode='lines',name='error'))
fig.show()