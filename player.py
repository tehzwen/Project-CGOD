class Player:
    def __init__(self, active, x, y, username):
        self.Active = active
        self.X = x
        self.Y = y
        self.userName = username

    def getUserName(self):
        return self.userName

    def getCoords(self):
        return (self.X,self.Y)