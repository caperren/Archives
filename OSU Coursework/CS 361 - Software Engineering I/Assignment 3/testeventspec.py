from assign3 import eventspec

es = eventspec.EventSpec("kwic.fsm")

es.event("a")
es.event("b")
es.event("b")
es.event("b")
es.event("b")
es.event("a")
es.printLog()

# Beatles code

def printHello():
    print "Hello"
def printGoodbye():
    print "Goodbye"
    
es.onEvent("a",printHello)
es.onEvent("b",printGoodbye)

es.event("a")
es.event("a")
es.event("a")
es.event("b")
es.event("b")
es.event("a")
es.printLog()
