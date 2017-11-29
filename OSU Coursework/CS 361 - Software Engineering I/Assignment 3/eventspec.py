
class EventSpec():

    def readfsm(self,file):
    # takes a filename, returns a finite state machine
    # fsm is (begin state, structure)
        fsm = {}
        with open(file) as f:
            s = None
            t = {}
            for l in f:
                ls = l.split()
                if (ls == []) or (ls[0][0] == "#"):
                    continue
                if ls[0] == "state:":
                    if s != None:
                        if s in fsm:
                            raise SyntaxError("Cannot define state " + s + " twice")
                        fsm[s] = t
                    s = ls[1]
                    t = {}
                elif ls[1] == "->":
                    t[ls[0]] = ls[2]
                elif ls[0] == "begin:":
                    beginState = ls[1]
                else:
                    raise SyntaxError(l + " is not a line in a finite state machine definition")
            if s != None:
                fsm[s] = t
            return (beginState, fsm)

    
    def __init__(self,file):
        (self.start, self.machine) = self.readfsm(file)
        self.state = self.start
        self.trace = []
        self.triggers = {}

    def onEvent(self, event, action):
        self.triggers[event] = action
        
    def reset(self):
        self.state = self.start
        self.trace = []

    def trace(self):
        return self.trace

    def state(self):
        return self.state

    def printLog(self):
        i = 0
        for (e,s) in self.trace:
            print "  STEP #"+str(i)+":",e,"-->",s
            i += 1
        
    def event(self, event):
        try:
            self.state = self.machine[self.state][event]
            self.trace.append((event,self.state))
            if event in self.triggers:
                self.triggers[event]()
        except KeyError:
            raise RuntimeError("From state " + self.state + ", transition " + event + " is not allowed (trace: " + str(self.trace) + ")")

